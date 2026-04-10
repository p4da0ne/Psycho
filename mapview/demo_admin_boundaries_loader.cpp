#include "demo_admin_boundaries_loader.h"

#include <QFile>
#include <QJsonDocument>
#include <QVector>
#include <QVariantMap>
#include <QtMath>
#include <algorithm>

namespace {
constexpr int kTargetPointsPerRing = 140;
constexpr int kMaxPolygonsPerSource = 260;
constexpr int kMaxPolygonsCombined = 420;
constexpr int kMaxPolygonsPerMultiPolygon = 1;

int simplificationStepFor(const int pointCount)
{
    int step = 1;
    if (pointCount > 20000)
        step = 64;
    else if (pointCount > 12000)
        step = 32;
    else if (pointCount > 6000)
        step = 16;
    else if (pointCount > 3000)
        step = 8;
    else if (pointCount > 1500)
        step = 4;
    else if (pointCount > 700)
        step = 2;

    if (pointCount > kTargetPointsPerRing) {
        const int adaptiveStep = qCeil(static_cast<double>(pointCount) / kTargetPointsPerRing);
        step = std::max(step, adaptiveStep);
    }
    return std::max(step, 1);
}

void appendCoordinateIfValid(const QJsonValue &coordValue, QVariantList &path)
{
    const QJsonArray pair = coordValue.toArray();
    if (pair.size() < 2)
        return;

    const double lon = pair.at(0).toDouble(qQNaN());
    const double lat = pair.at(1).toDouble(qQNaN());
    if (qIsNaN(lon) || qIsNaN(lat))
        return;

    if (!path.isEmpty()) {
        const QVariantMap lastPoint = path.constLast().toMap();
        if (lastPoint.value(QStringLiteral("lat")).toDouble() == lat
            && lastPoint.value(QStringLiteral("lon")).toDouble() == lon)
            return;
    }

    QVariantMap point;
    point.insert(QStringLiteral("lat"), lat);
    point.insert(QStringLiteral("lon"), lon);
    path.push_back(point);
}
} // namespace

DemoAdminBoundariesLoader::DemoAdminBoundariesLoader(QObject *parent)
    : QObject(parent)
{
}

QVariantList DemoAdminBoundariesLoader::loadDefaultPolygons() const
{
    QVariantList result;
    const QVariantList europe = loadPolygonsFromResource(":/data/europe_admin_demo.geojson");
    const QVariantList subjects = loadPolygonsFromResource(":/data/russia_subjects_demo.geojson");

    result.reserve(std::min<qsizetype>(europe.size() + subjects.size(), kMaxPolygonsCombined));
    for (const QVariant &item : europe) {
        if (result.size() >= kMaxPolygonsCombined)
            break;
        result.push_back(item);
    }
    for (const QVariant &item : subjects) {
        if (result.size() >= kMaxPolygonsCombined)
            break;
        result.push_back(item);
    }
    return result;
}

QVariantList DemoAdminBoundariesLoader::loadPolygonsFromResource(const QString &resourcePath) const
{
    QFile source(resourcePath);
    if (!source.open(QIODevice::ReadOnly | QIODevice::Text))
        return {};

    const QByteArray payload = source.readAll();
    source.close();
    return parseFeatureCollection(payload);
}

QVariantList DemoAdminBoundariesLoader::parseFeatureCollection(const QByteArray &jsonPayload) const
{
    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(jsonPayload, &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject())
        return {};

    const QJsonObject root = document.object();
    const QJsonArray features = root.value(QStringLiteral("features")).toArray();

    QVariantList polygons;
    polygons.reserve(std::min<qsizetype>(features.size(), kMaxPolygonsPerSource));
    for (const QJsonValue &featureValue : features) {
        if (polygons.size() >= kMaxPolygonsPerSource)
            break;
        if (!featureValue.isObject())
            continue;
        appendFeaturePolygons(featureValue.toObject(), polygons);
    }
    return polygons;
}

void DemoAdminBoundariesLoader::appendFeaturePolygons(const QJsonObject &feature, QVariantList &outPolygons) const
{
    if (outPolygons.size() >= kMaxPolygonsPerSource)
        return;

    const QJsonObject geometry = feature.value(QStringLiteral("geometry")).toObject();
    const QString geometryType = geometry.value(QStringLiteral("type")).toString();
    const QJsonArray coordinates = geometry.value(QStringLiteral("coordinates")).toArray();
    const QJsonObject properties = feature.value(QStringLiteral("properties")).toObject();

    const QString featureName =
        properties.value(QStringLiteral("name")).toString(
            properties.value(QStringLiteral("name_ru")).toString(
                properties.value(QStringLiteral("name_en")).toString(
                    properties.value(QStringLiteral("NAME")).toString(
                        properties.value(QStringLiteral("ADMIN")).toString(
                            properties.value(QStringLiteral("country")).toString())))));

    if (geometryType == QStringLiteral("Polygon")) {
        if (!coordinates.isEmpty()) {
            const QVariantMap polygon = buildPolygon(featureName, coordinates.at(0).toArray());
            if (!polygon.isEmpty())
                outPolygons.push_back(polygon);
        }
        return;
    }

    if (geometryType == QStringLiteral("MultiPolygon")) {
        QVector<QPair<int, QJsonArray>> rings;
        rings.reserve(coordinates.size());

        for (const QJsonValue &polygonValue : coordinates) {
            const QJsonArray polygonRings = polygonValue.toArray();
            if (polygonRings.isEmpty())
                continue;
            const QJsonArray outerRing = polygonRings.at(0).toArray();
            if (outerRing.size() < 3)
                continue;
            rings.push_back(qMakePair(outerRing.size(), outerRing));
        }

        std::sort(rings.begin(), rings.end(), [](const QPair<int, QJsonArray> &lhs, const QPair<int, QJsonArray> &rhs) {
            return lhs.first > rhs.first;
        });

        const qsizetype limit = std::min<qsizetype>(kMaxPolygonsPerMultiPolygon, rings.size());
        for (qsizetype i = 0; i < limit; ++i) {
            if (outPolygons.size() >= kMaxPolygonsPerSource)
                break;
            const QVariantMap polygon = buildPolygon(featureName, rings.at(i).second);
            if (!polygon.isEmpty())
                outPolygons.push_back(polygon);
        }
    }
}

QVariantMap DemoAdminBoundariesLoader::buildPolygon(const QString &featureName, const QJsonArray &ringCoordinates) const
{
    if (ringCoordinates.size() < 3)
        return {};

    QVariantList path;
    const int pointCount = ringCoordinates.size();
    const int step = simplificationStepFor(pointCount);
    path.reserve(std::min(kTargetPointsPerRing + 2, (pointCount / step) + 2));

    for (int i = 0; i < pointCount; i += step)
        appendCoordinateIfValid(ringCoordinates.at(i), path);

    if ((pointCount - 1) % step != 0)
        appendCoordinateIfValid(ringCoordinates.last(), path);

    if (path.size() < 3)
        return {};

    double north = -90.0;
    double south = 90.0;
    double east = -180.0;
    double west = 180.0;
    for (const QVariant &pointValue : path) {
        const QVariantMap point = pointValue.toMap();
        const double lat = point.value(QStringLiteral("lat")).toDouble();
        const double lon = point.value(QStringLiteral("lon")).toDouble();
        north = std::max(north, lat);
        south = std::min(south, lat);
        east = std::max(east, lon);
        west = std::min(west, lon);
    }

    QVariantMap polygon;
    polygon.insert(QStringLiteral("name"), featureName);
    polygon.insert(QStringLiteral("path"), path);
    polygon.insert(QStringLiteral("north"), north);
    polygon.insert(QStringLiteral("south"), south);
    polygon.insert(QStringLiteral("east"), east);
    polygon.insert(QStringLiteral("west"), west);
    return polygon;
}
