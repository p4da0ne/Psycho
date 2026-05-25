#include "mapruntimeservice.h"

#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>

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

QString sideByType(int objectType)
{
    if (objectType == 3 || objectType == 4 || objectType == 5 || objectType == 6 || objectType == 9) {
        return QStringLiteral("infrastructure");
    }
    if (objectType == 8) {
        return QStringLiteral("foreign");
    }
    return QStringLiteral("friendly");
}

QString kindByType(int objectType)
{
    if (objectType == 6) {
        return QStringLiteral("region");
    }
    if (objectType == 7) {
        return QStringLiteral("personnel");
    }
    if (objectType == 8) {
        return QStringLiteral("event");
    }
    if (objectType == 3 || objectType == 4 || objectType == 5) {
        return QStringLiteral("lbs");
    }
    return QStringLiteral("unit");
}

QString structurePathByType(int objectType)
{
    if (objectType == 6 || objectType == 7 || objectType == 8) {
        return QStringLiteral("countries/russia/moscow");
    }
    return QStringLiteral("countries/russia");
}

QVariantMap pointToVariant(const QJsonValue &value)
{
    const QJsonArray pair = value.toArray();
    if (pair.size() < 2) {
        return QVariantMap();
    }

    QVariantMap point;
    point.insert(QStringLiteral("lon"), pair.at(0).toDouble());
    point.insert(QStringLiteral("lat"), pair.at(1).toDouble());
    return point;
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

QVariantList MapRuntimeService::mapObjects() const
{
    return m_mapObjects;
}

QVariantList MapRuntimeService::mapLines() const
{
    return m_mapLines;
}

QVariantList MapRuntimeService::mapPolygons() const
{
    return m_mapPolygons;
}

QVariantList MapRuntimeService::locationLabels() const
{
    return m_locationLabels;
}

void MapRuntimeService::refreshNow()
{
    refreshNowLimited(1200);
}

void MapRuntimeService::refreshNowLimited(int limit)
{
    MapSnapshotService *snapshot = MapSnapshotService::instance();
    snapshot->refreshAllWithLimit(limit);
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
    rebuildViewModelsFromGeometryArrays(points, lines, polygons);
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
    rebuildViewModelsFromGeometryArrays(points, lines, polygons);
    m_pointsCount = points.size();
    m_linesCount = lines.size();
    m_polygonsCount = polygons.size();
    m_lastRuntimeUpdateIso = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    emit sourcesUpdated();
}

void MapRuntimeService::rebuildViewModelsFromGeometryArrays(
    const QJsonArray &points,
    const QJsonArray &lines,
    const QJsonArray &polygons)
{
    QMap<QString, QVariantMap> objectsByKey;
    QVariantList lineItems;
    QVariantList polygonItems;

    for (const QJsonValue &featureValue : points) {
        if (!featureValue.isObject()) {
            continue;
        }
        const QJsonObject feature = featureValue.toObject();
        const QJsonObject geometry = feature.value(QStringLiteral("geometry")).toObject();
        if (geometry.value(QStringLiteral("type")).toString() != QStringLiteral("Point")) {
            continue;
        }

        const QVariantMap point = pointToVariant(geometry.value(QStringLiteral("coordinates")));
        if (point.isEmpty()) {
            continue;
        }

        const QJsonObject props = feature.value(QStringLiteral("properties")).toObject();
        const int objectType = props.value(QStringLiteral("objectType")).toInt();
        const int objectId = props.value(QStringLiteral("objectId")).toInt();
        if (objectType <= 0 || objectId <= 0) {
            continue;
        }

        const QString role = props.value(QStringLiteral("geometryRole")).toString().toLower();
        const double legacyScore = props.contains(QStringLiteral("legacyScoreNormalized"))
            ? props.value(QStringLiteral("legacyScoreNormalized")).toDouble()
            : 0.5;
        const double heatValue = props.contains(QStringLiteral("heatValue"))
            ? props.value(QStringLiteral("heatValue")).toDouble()
            : (1.0 - legacyScore);

        QVariantMap row;
        row.insert(QStringLiteral("id"), QStringLiteral("%1-%2").arg(objectType).arg(objectId));
        row.insert(QStringLiteral("objectType"), objectType);
        row.insert(QStringLiteral("objectId"), objectId);
        row.insert(QStringLiteral("name"), props.value(QStringLiteral("title")).toString(QStringLiteral("Object %1").arg(objectId)));
        row.insert(QStringLiteral("side"), sideByType(objectType));
        row.insert(QStringLiteral("kind"), kindByType(objectType));
        row.insert(QStringLiteral("role"), role);
        row.insert(QStringLiteral("lat"), point.value(QStringLiteral("lat")));
        row.insert(QStringLiteral("lon"), point.value(QStringLiteral("lon")));
        row.insert(QStringLiteral("mpps"), qRound(legacyScore * 100.0));
        if (props.contains(QStringLiteral("speed"))) {
            row.insert(QStringLiteral("speed"), props.value(QStringLiteral("speed")).toDouble());
        }
        if (props.contains(QStringLiteral("course"))) {
            row.insert(QStringLiteral("course"), props.value(QStringLiteral("course")).toDouble());
        }
        row.insert(QStringLiteral("source"), props.value(QStringLiteral("source")).toString());
        row.insert(QStringLiteral("notes"), props.value(QStringLiteral("subtitle")).toString());
        row.insert(QStringLiteral("structurePath"), structurePathByType(objectType));
        row.insert(QStringLiteral("calcSource"), props.value(QStringLiteral("calcSource")).toString());
        row.insert(QStringLiteral("idSign"), props.value(QStringLiteral("idSign")).toInt());
        row.insert(QStringLiteral("signKey"), props.value(QStringLiteral("signKey")).toString());
        row.insert(QStringLiteral("legacySemantic17501"), props.value(QStringLiteral("legacySemantic17501")).toInt());
        row.insert(QStringLiteral("legacySemantic17502"), props.value(QStringLiteral("legacySemantic17502")).toInt());
        row.insert(QStringLiteral("legacySemantic18"), props.value(QStringLiteral("legacySemantic18")).toDouble());
        row.insert(QStringLiteral("legacySemantic19"), props.value(QStringLiteral("legacySemantic19")).toString());
        row.insert(QStringLiteral("legacySemantic105"), props.value(QStringLiteral("legacySemantic105")).toString());
        row.insert(QStringLiteral("legacySemantic32811"), props.value(QStringLiteral("legacySemantic32811")).toDouble());
        row.insert(QStringLiteral("legacySemantic32852"), props.value(QStringLiteral("legacySemantic32852")).toDouble());
        row.insert(QStringLiteral("countryId"), props.value(QStringLiteral("countryId")).toInt());
        row.insert(QStringLiteral("countryScope"), props.value(QStringLiteral("countryScope")).toString());
        row.insert(QStringLiteral("legacyScoreRaw"),
                   props.contains(QStringLiteral("legacyScoreRaw"))
                       ? props.value(QStringLiteral("legacyScoreRaw")).toDouble()
                       : legacyScore);
        row.insert(QStringLiteral("legacyScoreNormalized"), legacyScore);
        row.insert(QStringLiteral("heatValue"), heatValue);
        row.insert(QStringLiteral("heatClass"), props.value(QStringLiteral("heatClass")).toInt());

        const QString key = QStringLiteral("%1-%2").arg(objectType).arg(objectId);
        const QVariantMap existing = objectsByKey.value(key);
        const bool existingIsPosition = existing.value(QStringLiteral("role")).toString() == QStringLiteral("position");
        const bool currentIsPosition = role == QStringLiteral("position");
        if (existing.isEmpty() || (currentIsPosition && !existingIsPosition)) {
            objectsByKey.insert(key, row);
        }
    }

    auto appendGeometryItems = [](const QJsonArray &features, bool polygonMode, QVariantList &out) {
        for (const QJsonValue &featureValue : features) {
            if (!featureValue.isObject()) {
                continue;
            }
            const QJsonObject feature = featureValue.toObject();
            const QJsonObject geometry = feature.value(QStringLiteral("geometry")).toObject();
            const QString geometryType = geometry.value(QStringLiteral("type")).toString();
            const QJsonObject props = feature.value(QStringLiteral("properties")).toObject();

            const int objectType = props.value(QStringLiteral("objectType")).toInt();
            const int objectId = props.value(QStringLiteral("objectId")).toInt();
            // Region borders are taken from external GADM files, not from DB geometry.
            if (objectType == 6) {
                continue;
            }
            const QString baseId = QStringLiteral("%1-%2").arg(objectType).arg(objectId);

            QVariantMap base;
            base.insert(QStringLiteral("id"), baseId);
            base.insert(QStringLiteral("objectType"), objectType);
            base.insert(QStringLiteral("objectId"), objectId);
            base.insert(QStringLiteral("name"), props.value(QStringLiteral("title")).toString(QStringLiteral("Object %1").arg(objectId)));
            base.insert(QStringLiteral("side"), sideByType(objectType));
            base.insert(QStringLiteral("kind"), kindByType(objectType));
            base.insert(QStringLiteral("role"), props.value(QStringLiteral("geometryRole")).toString());
            base.insert(QStringLiteral("calcSource"), props.value(QStringLiteral("calcSource")).toString());
            base.insert(QStringLiteral("legacyScoreRaw"),
                        props.contains(QStringLiteral("legacyScoreRaw"))
                            ? props.value(QStringLiteral("legacyScoreRaw")).toDouble()
                            : 0.5);
            base.insert(QStringLiteral("legacyScoreNormalized"),
                        props.contains(QStringLiteral("legacyScoreNormalized"))
                            ? props.value(QStringLiteral("legacyScoreNormalized")).toDouble()
                            : 0.5);
            base.insert(QStringLiteral("heatValue"),
                        props.contains(QStringLiteral("heatValue"))
                            ? props.value(QStringLiteral("heatValue")).toDouble()
                            : 0.5);
            base.insert(QStringLiteral("heatClass"), props.value(QStringLiteral("heatClass")).toInt());

            if (!polygonMode && geometryType == QStringLiteral("LineString")) {
                QVariantList path;
                const QJsonArray coords = geometry.value(QStringLiteral("coordinates")).toArray();
                for (const QJsonValue &coord : coords) {
                    const QVariantMap point = pointToVariant(coord);
                    if (!point.isEmpty()) {
                        path.append(point);
                    }
                }
                if (path.size() >= 2) {
                    QVariantMap row = base;
                    row.insert(QStringLiteral("path"), path);
                    out.append(row);
                }
                continue;
            }

            if (!polygonMode && geometryType == QStringLiteral("MultiLineString")) {
                const QJsonArray segments = geometry.value(QStringLiteral("coordinates")).toArray();
                for (int i = 0; i < segments.size(); ++i) {
                    QVariantList path;
                    const QJsonArray segment = segments.at(i).toArray();
                    for (const QJsonValue &coord : segment) {
                        const QVariantMap point = pointToVariant(coord);
                        if (!point.isEmpty()) {
                            path.append(point);
                        }
                    }
                    if (path.size() >= 2) {
                        QVariantMap row = base;
                        row.insert(QStringLiteral("id"), QStringLiteral("%1-l%2").arg(baseId).arg(i));
                        row.insert(QStringLiteral("path"), path);
                        out.append(row);
                    }
                }
                continue;
            }

            if (polygonMode && geometryType == QStringLiteral("Polygon")) {
                const QJsonArray rings = geometry.value(QStringLiteral("coordinates")).toArray();
                if (!rings.isEmpty()) {
                    QVariantList path;
                    const QJsonArray outer = rings.at(0).toArray();
                    for (const QJsonValue &coord : outer) {
                        const QVariantMap point = pointToVariant(coord);
                        if (!point.isEmpty()) {
                            path.append(point);
                        }
                    }
                    if (path.size() >= 3) {
                        QVariantMap row = base;
                        row.insert(QStringLiteral("path"), path);
                        out.append(row);
                    }
                }
                continue;
            }

            if (polygonMode && geometryType == QStringLiteral("MultiPolygon")) {
                const QJsonArray multi = geometry.value(QStringLiteral("coordinates")).toArray();
                for (int i = 0; i < multi.size(); ++i) {
                    const QJsonArray onePolygon = multi.at(i).toArray();
                    if (onePolygon.isEmpty()) {
                        continue;
                    }
                    QVariantList path;
                    const QJsonArray outer = onePolygon.at(0).toArray();
                    for (const QJsonValue &coord : outer) {
                        const QVariantMap point = pointToVariant(coord);
                        if (!point.isEmpty()) {
                            path.append(point);
                        }
                    }
                    if (path.size() >= 3) {
                        QVariantMap row = base;
                        row.insert(QStringLiteral("id"), QStringLiteral("%1-p%2").arg(baseId).arg(i));
                        row.insert(QStringLiteral("path"), path);
                        out.append(row);
                    }
                }
            }
        }
    };

    appendGeometryItems(lines, false, lineItems);
    appendGeometryItems(polygons, true, polygonItems);

    QVariantList objects;
    QVariantList labels;
    for (auto it = objectsByKey.constBegin(); it != objectsByKey.constEnd(); ++it) {
        const QVariantMap row = it.value();
        objects.append(row);

        QVariantMap label;
        label.insert(QStringLiteral("name"), row.value(QStringLiteral("name")));
        label.insert(QStringLiteral("lat"), row.value(QStringLiteral("lat")));
        label.insert(QStringLiteral("lon"), row.value(QStringLiteral("lon")));
        labels.append(label);
    }

    m_mapObjects = objects;
    m_mapLines = lineItems;
    m_mapPolygons = polygonItems;
    m_locationLabels = labels;
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
