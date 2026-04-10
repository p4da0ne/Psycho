#ifndef GEOJSON_MANAGER_H
#define GEOJSON_MANAGER_H

#include "signdata.h"
#include "saturnSignTypes.h"
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QGeoCoordinate>

class GeojsonManager : public QObject
{
    Q_OBJECT
public:
    explicit GeojsonManager(QObject *parent = nullptr);

    static QJsonObject createFeatureCollection(QList<QJsonObject>& features);
    static QJsonObject createFeature(QJsonObject& properties, QJsonObject& geometry);
    static void extracted(SignData &sign);
    static QJsonObject createProperties(SignData &sign);
    static QJsonObject createGeometry(SignData& sign);

    QJsonObject* getFeatureCollection() const { return FeatureCollection; };

private:

    QJsonObject* FeatureCollection;

signals:
};

#endif // GEOJSON_MANAGER_H
