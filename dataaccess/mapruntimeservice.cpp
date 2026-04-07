#include "mapruntimeservice.h"

#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "dataaccess.h"
#include "mapsnapshotservice.h"
#include "mapeditingservice.h"

namespace {
QString toCollectionJson(const QJsonArray &features)
{
    QJsonObject collection;
    collection.insert("type", "FeatureCollection");
    collection.insert("features", features);
    return QString::fromUtf8(QJsonDocument(collection).toJson(QJsonDocument::Compact));
}
}

MapRuntimeService *MapRuntimeService::s_instance = nullptr;

MapRuntimeService::MapRuntimeService(QObject *parent)
    : QObject(parent)
{
    if (s_instance == nullptr) {
        s_instance = this;
    }

    m_rebuildDebounceTimer.setSingleShot(true);
    m_rebuildDebounceTimer.setInterval(m_rebuildDebounceMs);
    connect(
        &m_rebuildDebounceTimer,
        &QTimer::timeout,
        this,
        [this]() {
            rebuildMergedSourcesFromTypeCaches();
        });

    m_refreshDebounceTimer.setSingleShot(true);
    m_refreshDebounceTimer.setInterval(m_refreshDebounceMs);
    connect(
        &m_refreshDebounceTimer,
        &QTimer::timeout,
        this,
        [this]() {
            flushPendingTypeRefreshes();
        });

    bindSnapshot();
    bindEditing();
    if (DataAccess::instance()->connected()) {
        refreshNow();
    }
}

MapRuntimeService *MapRuntimeService::qmlSingleton(QQmlEngine*, QJSEngine*)
{
    if (s_instance == nullptr) {
        s_instance = new MapRuntimeService();
    }
    s_instance->bindSnapshot();
    s_instance->bindEditing();
    return s_instance;
}

MapRuntimeService *MapRuntimeService::instance()
{
    if (s_instance == nullptr) {
        s_instance = new MapRuntimeService();
    }
    s_instance->bindSnapshot();
    s_instance->bindEditing();
    return s_instance;
}

QString MapRuntimeService::pointsSource() const
{
    return m_pointsSource;
}

QString MapRuntimeService::linesSource() const
{
    return m_linesSource;
}

QString MapRuntimeService::polygonsSource() const
{
    return m_polygonsSource;
}

QString MapRuntimeService::lastRuntimeUpdateIso() const
{
    return m_lastRuntimeUpdateIso;
}

void MapRuntimeService::refreshNow()
{
    MapSnapshotService *snapshot = MapSnapshotService::instance();
    snapshot->refreshAll();
    rebuildAllFromSnapshotCache();
}

QString MapRuntimeService::sourceForGeometry(const QString &geometryFamily) const
{
    const QString key = geometryFamily.trimmed().toLower();
    if (key == "points" || key == "point") {
        return m_pointsSource;
    }
    if (key == "lines" || key == "line") {
        return m_linesSource;
    }
    if (key == "polygons" || key == "polygon" || key == "fills") {
        return m_polygonsSource;
    }
    return QString();
}

QVariantList MapRuntimeService::updateMeta() const
{
    QVariantList meta;

    QVariantMap pointsRow;
    pointsRow.insert("geometryFamily", "points");
    pointsRow.insert("featureCount", m_pointsCount);
    meta.append(pointsRow);

    QVariantMap linesRow;
    linesRow.insert("geometryFamily", "lines");
    linesRow.insert("featureCount", m_linesCount);
    meta.append(linesRow);

    QVariantMap polygonsRow;
    polygonsRow.insert("geometryFamily", "polygons");
    polygonsRow.insert("featureCount", m_polygonsCount);
    meta.append(polygonsRow);

    return meta;
}

void MapRuntimeService::bindSnapshot()
{
    if (m_snapshotBound) {
        return;
    }
    MapSnapshotService *snapshot = MapSnapshotService::instance();
    connect(
        snapshot,
        &MapSnapshotService::typeUpdated,
        this,
        [this, snapshot](int objectType) {
            updateTypeCaches(objectType, snapshot->featureCollectionForType(objectType));
            scheduleMergedRebuild();
        });
    connect(
        snapshot,
        &MapSnapshotService::snapshotUpdated,
        this,
        [this]() {
            // Fallback sync point (for full refresh paths).
            scheduleMergedRebuild();
        });
    m_snapshotBound = true;
}

void MapRuntimeService::bindEditing()
{
    if (m_editingBound) {
        return;
    }

    MapEditingService *editing = MapEditingService::instance();
    connect(
        editing,
        &MapEditingService::geometryChanged,
        this,
        [this](int objectType, int, const QString &) {
            // Coalesced refresh by object type to avoid duplicate snapshot calls
            // during multi-role/batch edit operations.
            scheduleTypeRefresh(objectType);
        });
    m_editingBound = true;
}

void MapRuntimeService::rebuildSources(const QString &fullCollectionJson)
{
    const QJsonDocument fullDoc = QJsonDocument::fromJson(fullCollectionJson.toUtf8());
    QJsonArray allFeatures;
    if (fullDoc.isObject()) {
        allFeatures = fullDoc.object().value("features").toArray();
    }

    QJsonArray points;
    QJsonArray lines;
    QJsonArray polygons;

    for (const QJsonValue &featureValue : allFeatures) {
        if (!featureValue.isObject()) {
            continue;
        }
        const QJsonObject feature = featureValue.toObject();
        const QJsonObject geometry = feature.value("geometry").toObject();
        const QString type = geometry.value("type").toString();

        if (type == "Point" || type == "MultiPoint") {
            points.append(feature);
            continue;
        }
        if (type == "LineString" || type == "MultiLineString") {
            lines.append(feature);
            continue;
        }
        if (type == "Polygon" || type == "MultiPolygon") {
            polygons.append(feature);
            continue;
        }
    }

    m_pointsSource = toCollectionJson(points);
    m_linesSource = toCollectionJson(lines);
    m_polygonsSource = toCollectionJson(polygons);
    m_pointsCount = points.size();
    m_linesCount = lines.size();
    m_polygonsCount = polygons.size();
    m_lastRuntimeUpdateIso = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    emit sourcesUpdated();
}

void MapRuntimeService::updateTypeCaches(int objectType, const QString &typeCollectionJson)
{
    QJsonArray points;
    QJsonArray lines;
    QJsonArray polygons;
    splitCollectionByGeometry(typeCollectionJson, points, lines, polygons);
    m_pointsByType[objectType] = points;
    m_linesByType[objectType] = lines;
    m_polygonsByType[objectType] = polygons;
}

void MapRuntimeService::scheduleMergedRebuild()
{
    m_rebuildDebounceTimer.start();
}

void MapRuntimeService::scheduleTypeRefresh(int objectType)
{
    if (objectType <= 0) {
        return;
    }
    m_pendingRefreshTypes.insert(objectType);
    m_refreshDebounceTimer.start();
}

void MapRuntimeService::flushPendingTypeRefreshes()
{
    if (m_pendingRefreshTypes.isEmpty()) {
        return;
    }

    MapSnapshotService *snapshot = MapSnapshotService::instance();
    const QList<int> objectTypes = m_pendingRefreshTypes.values();
    m_pendingRefreshTypes.clear();

    for (int objectType : objectTypes) {
        snapshot->refreshType(objectType, 1200);
    }
}

void MapRuntimeService::rebuildMergedSourcesFromTypeCaches()
{
    QJsonArray points;
    QJsonArray lines;
    QJsonArray polygons;

    for (auto it = m_pointsByType.constBegin(); it != m_pointsByType.constEnd(); ++it) {
        for (const QJsonValue &feature : it.value()) {
            points.append(feature);
        }
    }
    for (auto it = m_linesByType.constBegin(); it != m_linesByType.constEnd(); ++it) {
        for (const QJsonValue &feature : it.value()) {
            lines.append(feature);
        }
    }
    for (auto it = m_polygonsByType.constBegin(); it != m_polygonsByType.constEnd(); ++it) {
        for (const QJsonValue &feature : it.value()) {
            polygons.append(feature);
        }
    }

    m_pointsSource = toCollectionJson(points);
    m_linesSource = toCollectionJson(lines);
    m_polygonsSource = toCollectionJson(polygons);
    m_pointsCount = points.size();
    m_linesCount = lines.size();
    m_polygonsCount = polygons.size();
    m_lastRuntimeUpdateIso = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    emit sourcesUpdated();
}

void MapRuntimeService::rebuildAllFromSnapshotCache()
{
    MapSnapshotService *snapshot = MapSnapshotService::instance();
    const QList<int> types = extractTypesFromSnapshotMeta(snapshot->snapshotMeta());
    for (int objectType : types) {
        updateTypeCaches(objectType, snapshot->featureCollectionForType(objectType));
    }
    rebuildMergedSourcesFromTypeCaches();
}

void MapRuntimeService::splitCollectionByGeometry(
    const QString &collectionJson,
    QJsonArray &points,
    QJsonArray &lines,
    QJsonArray &polygons)
{
    const QJsonDocument doc = QJsonDocument::fromJson(collectionJson.toUtf8());
    if (!doc.isObject()) {
        return;
    }

    const QJsonArray features = doc.object().value("features").toArray();
    for (const QJsonValue &featureValue : features) {
        if (!featureValue.isObject()) {
            continue;
        }
        const QJsonObject feature = featureValue.toObject();
        const QJsonObject geometry = feature.value("geometry").toObject();
        const QString type = geometry.value("type").toString();

        if (type == "Point" || type == "MultiPoint") {
            points.append(feature);
            continue;
        }
        if (type == "LineString" || type == "MultiLineString") {
            lines.append(feature);
            continue;
        }
        if (type == "Polygon" || type == "MultiPolygon") {
            polygons.append(feature);
            continue;
        }
    }
}

QList<int> MapRuntimeService::extractTypesFromSnapshotMeta(const QVariantList &meta)
{
    QList<int> types;
    for (const QVariant &rowValue : meta) {
        const QVariantMap row = rowValue.toMap();
        const int objectType = row.value("objectType").toInt();
        if (objectType > 0) {
            types.append(objectType);
        }
    }
    return types;
}
