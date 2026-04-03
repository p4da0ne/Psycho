#include "geojsonservice.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QStringList>
#include <algorithm>

#include "geometryrepository.h"

GeoJsonService *GeoJsonService::s_instance = nullptr;

GeoJsonService::GeoJsonService(QObject *parent)
    : QObject(parent)
{
    if (s_instance == nullptr) {
        s_instance = this;
    }
}

GeoJsonService *GeoJsonService::qmlSingleton(QQmlEngine*, QJSEngine*)
{
    if (s_instance == nullptr) {
        s_instance = new GeoJsonService();
    }
    return s_instance;
}

GeoJsonService *GeoJsonService::instance()
{
    if (s_instance == nullptr) {
        s_instance = new GeoJsonService();
    }
    return s_instance;
}

QString GeoJsonService::buildFeatureCollection(int objectType, int objectId)
{
    const QVariantList rows = GeometryRepository::instance()->loadObjectGeometry(objectType, objectId);

    QJsonObject featureCollection;
    featureCollection.insert("type", "FeatureCollection");

    QJsonArray features;
    QMap<QString, QVariantList> grouped;
    QMap<QString, QString> geometryTypeByRole;
    for (const QVariant &rowValue : rows) {
        const QVariantMap row = rowValue.toMap();
        const QString role = row.value("geometryRole").toString();
        grouped[role].append(row);
        geometryTypeByRole[role] = row.value("geometryType").toString();
    }

    for (auto it = grouped.begin(); it != grouped.end(); ++it) {
        const QString role = it.key();
        QVariantList points = it.value();
        std::sort(points.begin(), points.end(), [](const QVariant &a, const QVariant &b) {
            return a.toMap().value("pointOrder").toInt() < b.toMap().value("pointOrder").toInt();
        });

        QJsonArray coordinatesArray;
        for (const QVariant &pointValue : points) {
            const QVariantMap point = pointValue.toMap();
            QJsonArray coord;
            coord.append(point.value("longitude").toDouble());
            coord.append(point.value("latitude").toDouble());
            coordinatesArray.append(coord);
        }

        QString geometryType = geometryTypeByRole.value(role, "Point");
        if (coordinatesArray.size() == 1) {
            geometryType = "Point";
        }

        QJsonObject geometry;
        geometry.insert("type", geometryType);
        if (geometryType == "Point") {
            geometry.insert("coordinates", coordinatesArray.at(0));
        } else if (geometryType == "Polygon") {
            if (coordinatesArray.size() > 0 && coordinatesArray.first() != coordinatesArray.last()) {
                coordinatesArray.append(coordinatesArray.first());
            }
            QJsonArray polygon;
            polygon.append(coordinatesArray);
            geometry.insert("coordinates", polygon);
        } else {
            geometry.insert("coordinates", coordinatesArray);
        }

        QJsonObject properties;
        properties.insert("objectType", objectType);
        properties.insert("objectId", objectId);
        properties.insert("geometryRole", role);
        properties.insert("geometryType", geometryType);

        QJsonObject feature;
        feature.insert("type", "Feature");
        feature.insert("id", QString("%1:%2:%3").arg(objectType).arg(objectId).arg(role));
        feature.insert("geometry", geometry);
        feature.insert("properties", properties);
        features.append(feature);
    }

    featureCollection.insert("features", features);
    return QString::fromUtf8(QJsonDocument(featureCollection).toJson(QJsonDocument::Compact));
}
