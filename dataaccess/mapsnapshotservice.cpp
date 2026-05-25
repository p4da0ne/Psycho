#include "mapsnapshotservice.h"

#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSet>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QThread>
#include <QUuid>
#include <QVariantMap>
#include <QtConcurrent/QtConcurrentRun>

#include <algorithm>

#include "dataaccess.h"
#include "mapobjectsrepository.h"
#include "pollingservice.h"

namespace {
constexpr int kDefaultSnapshotLimit = 1200;
constexpr int kDefaultQueuedLimit = 5000;

struct SnapshotDbConfig
{
    QString driver;
    QString host;
    int port = 5432;
    QString dbName;
    QString user;
    QString password;
};

SnapshotDbConfig captureDbConfig()
{
    DataAccess *db = DataAccess::instance();
    SnapshotDbConfig config;
    config.driver = db->driver();
    config.host = db->host();
    config.port = db->port();
    config.dbName = db->dbName();
    config.user = db->user();
    config.password = db->password();
    return config;
}

QSqlDatabase openWorkerConnection(const SnapshotDbConfig &config, const QString &connectionName, QString &error)
{
    const QString driver = config.driver.trimmed();
    if (driver.isEmpty()) {
        error = QStringLiteral("DB driver is empty");
        return QSqlDatabase();
    }

    QSqlDatabase db = QSqlDatabase::addDatabase(driver, connectionName);
    db.setHostName(config.host);
    db.setPort(config.port);
    db.setDatabaseName(config.dbName);
    db.setUserName(config.user);
    db.setPassword(config.password);
    if (driver.compare(QStringLiteral("QPSQL"), Qt::CaseInsensitive) == 0) {
        db.setConnectOptions(QStringLiteral("connect_timeout=5"));
    } else {
        db.setConnectOptions(QString());
    }

    if (!db.open()) {
        error = db.lastError().text();
        return QSqlDatabase();
    }
    return db;
}

int featureCountFromCollection(const QString &geoJson)
{
    const QJsonDocument doc = QJsonDocument::fromJson(geoJson.toUtf8());
    if (!doc.isObject()) {
        return 0;
    }
    return doc.object().value("features").toArray().size();
}

bool readGeometryMetaWithDb(const QSqlDatabase &db, int objectType, QDateTime &maxUpdatedUtc, int &rowCount)
{
    if (!db.isValid() || !db.isOpen()) {
        return false;
    }

    QSqlQuery query(db);
    query.prepare(
        "SELECT MAX(updated_at) AS max_updated, COUNT(*) AS row_count "
        "FROM object_geometry "
        "WHERE object_type = :object_type");
    query.bindValue(":object_type", objectType);
    if (!query.exec() || !query.next()) {
        return false;
    }

    rowCount = query.value("row_count").toInt();
    const QVariant maxUpdatedValue = query.value("max_updated");
    if (!maxUpdatedValue.isNull()) {
        maxUpdatedUtc = maxUpdatedValue.toDateTime().toUTC();
    } else {
        maxUpdatedUtc = QDateTime();
    }
    return true;
}

QVariantMap buildSnapshotPayload(
    const SnapshotDbConfig &config,
    const QList<int> &types,
    int limit,
    bool fullRefresh)
{
    QVariantMap result;
    result.insert("ok", false);
    result.insert("fullRefresh", fullRefresh);
    result.insert("limit", limit);

    QVariantList typeValues;
    QVariantMap collections;
    QVariantMap featureCounts;
    QVariantMap maxUpdatedByType;
    QVariantMap rowsByType;

    if (types.isEmpty()) {
        result.insert("ok", true);
        result.insert("types", typeValues);
        result.insert("collections", collections);
        result.insert("featureCounts", featureCounts);
        result.insert("maxUpdatedByType", maxUpdatedByType);
        result.insert("rowsByType", rowsByType);
        return result;
    }

    const QString connectionName =
        QStringLiteral("saturn_snapshot_worker_%1_%2")
            .arg(reinterpret_cast<quintptr>(QThread::currentThreadId()))
            .arg(QUuid::createUuid().toString(QUuid::WithoutBraces));

    QString openError;
    {
        QSqlDatabase db = openWorkerConnection(config, connectionName, openError);
        if (!db.isValid() || !db.isOpen()) {
            result.insert("error", openError);
            if (QSqlDatabase::contains(connectionName)) {
                QSqlDatabase::removeDatabase(connectionName);
            }
            return result;
        }

        for (int objectType : types) {
            const QString key = QString::number(objectType);
            const QString geoJson = MapObjectsRepository::buildTypeGeoJsonWithDb(objectType, limit, db);
            collections.insert(key, geoJson);
            featureCounts.insert(key, featureCountFromCollection(geoJson));
            typeValues.append(objectType);

            QDateTime maxUpdatedUtc;
            int rowCount = 0;
            if (readGeometryMetaWithDb(db, objectType, maxUpdatedUtc, rowCount)) {
                maxUpdatedByType.insert(key, maxUpdatedUtc.toUTC().toString(Qt::ISODate));
                rowsByType.insert(key, rowCount);
            }
        }

        db.close();
    }
    QSqlDatabase::removeDatabase(connectionName);

    result.insert("ok", true);
    result.insert("types", typeValues);
    result.insert("collections", collections);
    result.insert("featureCounts", featureCounts);
    result.insert("maxUpdatedByType", maxUpdatedByType);
    result.insert("rowsByType", rowsByType);
    return result;
}
}

MapSnapshotService *MapSnapshotService::s_instance = nullptr;

MapSnapshotService::MapSnapshotService(QObject *parent)
    : QObject(parent)
{
    if (s_instance == nullptr) {
        s_instance = this;
    }

    connect(
        &m_workerWatcher,
        &QFutureWatcher<QVariantMap>::finished,
        this,
        &MapSnapshotService::handleWorkerFinished);

    bindPollingIfNeeded();
}

MapSnapshotService *MapSnapshotService::qmlSingleton(QQmlEngine*, QJSEngine*)
{
    if (s_instance == nullptr) {
        s_instance = new MapSnapshotService();
    }
    s_instance->bindPollingIfNeeded();
    return s_instance;
}

MapSnapshotService *MapSnapshotService::instance()
{
    if (s_instance == nullptr) {
        s_instance = new MapSnapshotService();
    }
    s_instance->bindPollingIfNeeded();
    return s_instance;
}

QString MapSnapshotService::lastUpdatedIso() const
{
    if (!m_lastUpdated.isValid()) {
        return QString();
    }
    return m_lastUpdated.toUTC().toString(Qt::ISODate);
}

bool MapSnapshotService::pollingBound() const
{
    return m_pollingBound;
}

QString MapSnapshotService::refreshAll()
{
    return refreshAllWithLimit(kDefaultSnapshotLimit);
}

QString MapSnapshotService::refreshAllWithLimit(int limit)
{
    const int effectiveLimit = limit > 0 ? limit : kDefaultSnapshotLimit;
    requestWorkerBuild(supportedMapTypes(), effectiveLimit, true);
    return featureCollectionAll();
}

QVariantList MapSnapshotService::refreshByDelta(int limit)
{
    QVariantList changedTypes;
    const QList<int> types = supportedMapTypes();

    if (!m_deltaBaselineInitialized) {
        for (int objectType : types) {
            changedTypes.append(objectType);
        }
        requestWorkerBuild(types, limit > 0 ? limit : kDefaultSnapshotLimit, true);
        return changedTypes;
    }

    QSet<int> changedTypesSet;
    qint64 maxChangeId = m_lastProcessedChangeId;
    if (readChangedTypesFromChangeLog(changedTypesSet, maxChangeId)) {
        QList<int> pendingTypes = changedTypesSet.values();
        std::sort(pendingTypes.begin(), pendingTypes.end());
        for (int objectType : pendingTypes) {
            changedTypes.append(objectType);
        }
        if (!pendingTypes.isEmpty()) {
            requestWorkerBuild(pendingTypes, limit > 0 ? limit : kDefaultSnapshotLimit, false);
        }
        m_lastProcessedChangeId = maxChangeId;
        return changedTypes;
    }

    QList<int> pendingTypes;
    for (int objectType : types) {
        QDateTime currentMaxUpdatedUtc;
        int currentRows = 0;
        if (!readGeometryMeta(objectType, currentMaxUpdatedUtc, currentRows)) {
            continue;
        }

        const bool hasPrev = m_lastGeometryRowsByType.contains(objectType);
        const QDateTime prevMaxUpdatedUtc = m_lastGeometryUpdatedByType.value(objectType);
        const int prevRows = m_lastGeometryRowsByType.value(objectType, -1);

        bool changed = !hasPrev;
        if (!changed && prevRows != currentRows) {
            changed = true;
        }
        if (!changed && prevMaxUpdatedUtc != currentMaxUpdatedUtc) {
            changed = true;
        }

        m_lastGeometryUpdatedByType[objectType] = currentMaxUpdatedUtc;
        m_lastGeometryRowsByType[objectType] = currentRows;

        if (changed) {
            pendingTypes.append(objectType);
            changedTypes.append(objectType);
        }
    }

    if (!pendingTypes.isEmpty()) {
        requestWorkerBuild(pendingTypes, limit > 0 ? limit : kDefaultSnapshotLimit, false);
    }
    return changedTypes;
}

QString MapSnapshotService::refreshType(int objectType, int limit)
{
    if (objectType <= 0) {
        return QString();
    }
    requestWorkerBuild(QList<int>{objectType}, limit > 0 ? limit : kDefaultSnapshotLimit, false);
    return m_geoJsonByType.value(objectType);
}

QString MapSnapshotService::featureCollectionForType(int objectType)
{
    if (!m_geoJsonByType.contains(objectType) && objectType > 0) {
        requestWorkerBuild(QList<int>{objectType}, kDefaultSnapshotLimit, false);
    }
    return m_geoJsonByType.value(objectType);
}

QString MapSnapshotService::featureCollectionAll()
{
    QJsonObject collection;
    collection.insert("type", "FeatureCollection");
    QJsonArray features;

    const QList<int> types = supportedMapTypes();
    for (int objectType : types) {
        const QString geoJson = m_geoJsonByType.value(objectType);
        if (geoJson.isEmpty()) {
            continue;
        }
        const QJsonDocument doc = QJsonDocument::fromJson(geoJson.toUtf8());
        if (!doc.isObject()) {
            continue;
        }
        const QJsonArray oneFeatures = doc.object().value("features").toArray();
        for (const QJsonValue &feature : oneFeatures) {
            features.append(feature);
        }
    }

    collection.insert("features", features);
    return QString::fromUtf8(QJsonDocument(collection).toJson(QJsonDocument::Compact));
}

QVariantList MapSnapshotService::snapshotMeta() const
{
    QVariantList meta;
    const QList<int> types = supportedMapTypes();
    for (int objectType : types) {
        QVariantMap row;
        row.insert("objectType", objectType);
        row.insert("featureCount", m_featureCountByType.value(objectType, 0));
        row.insert(
            "refreshedAtIso",
            m_refreshedByType.contains(objectType)
                ? m_refreshedByType.value(objectType).toUTC().toString(Qt::ISODate)
                : QString());
        meta.append(row);
    }
    return meta;
}

void MapSnapshotService::bindPollingIfNeeded()
{
    if (m_pollingBound) {
        return;
    }
    PollingService *polling = PollingService::instance();
    connect(
        polling,
        &PollingService::tick,
        this,
        [this]() {
            refreshByDelta(kDefaultSnapshotLimit);
        });
    m_pollingBound = true;
    emit pollingBindingChanged();
}

void MapSnapshotService::requestWorkerBuild(const QList<int> &objectTypes, int limit, bool fullRefresh)
{
    QSet<int> deduplicated;
    for (int objectType : objectTypes) {
        if (objectType > 0) {
            deduplicated.insert(objectType);
        }
    }
    if (deduplicated.isEmpty()) {
        return;
    }

    const int effectiveLimit = limit > 0 ? limit : kDefaultSnapshotLimit;
    QList<int> normalizedTypes = deduplicated.values();
    std::sort(normalizedTypes.begin(), normalizedTypes.end());

    if (m_workerBusy) {
        m_pendingLimit = (std::max)(m_pendingLimit, effectiveLimit);
        if (fullRefresh) {
            m_pendingFullRefresh = true;
            m_pendingTypes.clear();
        } else if (!m_pendingFullRefresh) {
            m_pendingTypes.unite(deduplicated);
        }
        return;
    }

    startWorkerBuild(normalizedTypes, effectiveLimit, fullRefresh);
}

void MapSnapshotService::startWorkerBuild(const QList<int> &objectTypes, int limit, bool fullRefresh)
{
    if (objectTypes.isEmpty() || m_workerBusy) {
        return;
    }

    m_workerBusy = true;

    const SnapshotDbConfig config = captureDbConfig();
    const QFuture<QVariantMap> future = QtConcurrent::run(
        [config, objectTypes, limit, fullRefresh]() {
            return buildSnapshotPayload(config, objectTypes, limit, fullRefresh);
        });
    m_workerWatcher.setFuture(future);
}

void MapSnapshotService::handleWorkerFinished()
{
    m_workerBusy = false;

    const QVariantMap result = m_workerWatcher.result();
    const bool ok = result.value("ok").toBool();
    const bool fullRefresh = result.value("fullRefresh").toBool();
    if (ok) {
        const QVariantList typeValues = result.value("types").toList();
        const QVariantMap collections = result.value("collections").toMap();
        const QVariantMap featureCounts = result.value("featureCounts").toMap();
        const QVariantMap maxUpdatedByType = result.value("maxUpdatedByType").toMap();
        const QVariantMap rowsByType = result.value("rowsByType").toMap();

        QList<int> updatedTypes;
        for (const QVariant &typeValue : typeValues) {
            const int objectType = typeValue.toInt();
            if (objectType <= 0) {
                continue;
            }

            const QString key = QString::number(objectType);
            const QString geoJson = collections.value(key).toString();
            m_geoJsonByType[objectType] = geoJson;
            m_featureCountByType[objectType] = featureCounts.value(key).toInt();
            m_refreshedByType[objectType] = QDateTime::currentDateTimeUtc();

            const QString maxUpdatedIso = maxUpdatedByType.value(key).toString();
            if (!maxUpdatedIso.isEmpty()) {
                m_lastGeometryUpdatedByType[objectType] = QDateTime::fromString(maxUpdatedIso, Qt::ISODate).toUTC();
            }
            if (rowsByType.contains(key)) {
                m_lastGeometryRowsByType[objectType] = rowsByType.value(key).toInt();
            }
            updatedTypes.append(objectType);
        }

        if (fullRefresh) {
            m_deltaBaselineInitialized = true;
            qint64 lastChangeId = 0;
            if (readLastChangeId(lastChangeId)) {
                m_lastProcessedChangeId = lastChangeId;
            }
        }

        if (!updatedTypes.isEmpty()) {
            m_lastUpdated = QDateTime::currentDateTimeUtc();
            for (int objectType : updatedTypes) {
                emit typeUpdated(objectType);
            }
            emit snapshotUpdated();
        } else if (fullRefresh) {
            m_lastUpdated = QDateTime::currentDateTimeUtc();
            emit snapshotUpdated();
        }
    }

    if (m_pendingFullRefresh) {
        const int nextLimit = m_pendingLimit > 0 ? m_pendingLimit : kDefaultSnapshotLimit;
        m_pendingFullRefresh = false;
        m_pendingTypes.clear();
        m_pendingLimit = kDefaultQueuedLimit;
        startWorkerBuild(supportedMapTypes(), nextLimit, true);
        return;
    }

    if (!m_pendingTypes.isEmpty()) {
        QList<int> nextTypes = m_pendingTypes.values();
        std::sort(nextTypes.begin(), nextTypes.end());
        const int nextLimit = m_pendingLimit > 0 ? m_pendingLimit : kDefaultSnapshotLimit;
        m_pendingTypes.clear();
        m_pendingLimit = kDefaultQueuedLimit;
        startWorkerBuild(nextTypes, nextLimit, false);
    } else {
        m_pendingLimit = kDefaultQueuedLimit;
    }
}

int MapSnapshotService::countFeaturesInCollection(const QString &geoJson)
{
    return featureCountFromCollection(geoJson);
}

bool MapSnapshotService::readGeometryMeta(int objectType, QDateTime &maxUpdatedUtc, int &rowCount) const
{
    DataAccess *db = DataAccess::instance();
    if (!db->connected() && !db->connectToDatabase()) {
        return false;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT MAX(updated_at) AS max_updated, COUNT(*) AS row_count "
        "FROM object_geometry "
        "WHERE object_type = :object_type");
    query.bindValue(":object_type", objectType);
    if (!query.exec() || !query.next()) {
        return false;
    }

    rowCount = query.value("row_count").toInt();
    const QVariant maxUpdatedValue = query.value("max_updated");
    if (!maxUpdatedValue.isNull()) {
        maxUpdatedUtc = maxUpdatedValue.toDateTime().toUTC();
    } else {
        maxUpdatedUtc = QDateTime();
    }
    return true;
}

bool MapSnapshotService::readChangedTypesFromChangeLog(QSet<int> &changedTypes, qint64 &maxChangeId) const
{
    DataAccess *db = DataAccess::instance();
    if (!db->connected() && !db->connectToDatabase()) {
        return false;
    }

    const QList<int> mapTypes = supportedMapTypes();
    const QSet<int> mapTypeSet = QSet<int>(mapTypes.begin(), mapTypes.end());

    QSqlQuery query;
    query.prepare(
        "SELECT id_object_geometry_change, object_type "
        "FROM object_geometry_changes "
        "WHERE id_object_geometry_change > :last_change_id "
        "ORDER BY id_object_geometry_change");
    query.bindValue(":last_change_id", m_lastProcessedChangeId);
    if (!query.exec()) {
        return false;
    }

    qint64 localMaxChangeId = m_lastProcessedChangeId;
    while (query.next()) {
        const qint64 changeId = query.value("id_object_geometry_change").toLongLong();
        const int objectType = query.value("object_type").toInt();
        if (changeId > localMaxChangeId) {
            localMaxChangeId = changeId;
        }
        if (mapTypeSet.contains(objectType)) {
            changedTypes.insert(objectType);
        }
    }

    maxChangeId = localMaxChangeId;
    return true;
}

bool MapSnapshotService::readLastChangeId(qint64 &lastChangeId) const
{
    DataAccess *db = DataAccess::instance();
    if (!db->connected() && !db->connectToDatabase()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT COALESCE(MAX(id_object_geometry_change), 0) AS max_change_id FROM object_geometry_changes");
    if (!query.exec() || !query.next()) {
        return false;
    }

    lastChangeId = query.value("max_change_id").toLongLong();
    return true;
}

QList<int> MapSnapshotService::supportedMapTypes()
{
    // SMI (10) intentionally excluded from map snapshot: non-geometry entity.
    return QList<int>{1, 2, 3, 4, 5, 6, 7, 8, 9};
}
