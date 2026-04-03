#include "mapsnapshotservice.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlQuery>
#include <QSet>

#include "dataaccess.h"
#include "mapobjectsrepository.h"
#include "pollingservice.h"

MapSnapshotService *MapSnapshotService::s_instance = nullptr;

MapSnapshotService::MapSnapshotService(QObject *parent)
    : QObject(parent)
{
    if (s_instance == nullptr) {
        s_instance = this;
    }
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
    const QList<int> types = supportedMapTypes();
    for (int objectType : types) {
        updateCacheForType(objectType, 5000);

        QDateTime maxUpdatedUtc;
        int rowCount = 0;
        if (readGeometryMeta(objectType, maxUpdatedUtc, rowCount)) {
            m_lastGeometryUpdatedByType[objectType] = maxUpdatedUtc;
            m_lastGeometryRowsByType[objectType] = rowCount;
        }
    }
    m_deltaBaselineInitialized = true;
    qint64 lastChangeId = 0;
    if (readLastChangeId(lastChangeId)) {
        m_lastProcessedChangeId = lastChangeId;
    }
    m_lastUpdated = QDateTime::currentDateTimeUtc();
    emit snapshotUpdated();
    return featureCollectionAll();
}

QVariantList MapSnapshotService::refreshByDelta(int limit)
{
    QVariantList changedTypes;
    const QList<int> types = supportedMapTypes();

    if (!m_deltaBaselineInitialized) {
        refreshAll();
        for (int objectType : types) {
            changedTypes.append(objectType);
        }
        return changedTypes;
    }

    QSet<int> changedTypesSet;
    qint64 maxChangeId = m_lastProcessedChangeId;
    if (readChangedTypesFromChangeLog(changedTypesSet, maxChangeId)) {
        for (int objectType : changedTypesSet) {
            updateCacheForType(objectType, limit);
            changedTypes.append(objectType);
        }
        m_lastProcessedChangeId = maxChangeId;
        if (!changedTypes.isEmpty()) {
            m_lastUpdated = QDateTime::currentDateTimeUtc();
            emit snapshotUpdated();
        }
        return changedTypes;
    }

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
            updateCacheForType(objectType, limit);
            changedTypes.append(objectType);
        }
    }

    if (!changedTypes.isEmpty()) {
        m_lastUpdated = QDateTime::currentDateTimeUtc();
        emit snapshotUpdated();
    }
    return changedTypes;
}

QString MapSnapshotService::refreshType(int objectType, int limit)
{
    updateCacheForType(objectType, limit);
    m_lastUpdated = QDateTime::currentDateTimeUtc();
    emit snapshotUpdated();
    return m_geoJsonByType.value(objectType);
}

QString MapSnapshotService::featureCollectionForType(int objectType)
{
    if (!m_geoJsonByType.contains(objectType)) {
        updateCacheForType(objectType, 5000);
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
        const QString geoJson = featureCollectionForType(objectType);
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
            refreshByDelta(5000);
        },
        Qt::UniqueConnection);
    m_pollingBound = true;
    emit pollingBindingChanged();
}

void MapSnapshotService::updateCacheForType(int objectType, int limit)
{
    MapObjectsRepository *repo = MapObjectsRepository::instance();
    const QString geoJson = repo->buildTypeGeoJson(objectType, limit);
    m_geoJsonByType[objectType] = geoJson;
    m_featureCountByType[objectType] = countFeaturesInCollection(geoJson);
    m_refreshedByType[objectType] = QDateTime::currentDateTimeUtc();
    emit typeUpdated(objectType);
}

int MapSnapshotService::countFeaturesInCollection(const QString &geoJson)
{
    const QJsonDocument doc = QJsonDocument::fromJson(geoJson.toUtf8());
    if (!doc.isObject()) {
        return 0;
    }
    return doc.object().value("features").toArray().size();
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
