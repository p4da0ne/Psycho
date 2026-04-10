#ifndef GEOJSONBUILDER_H
#define GEOJSONBUILDER_H

#include "signdata.h"
#include "saturnSignTypes.h"
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QGeoCoordinate>

class GeojsonBuilder : public QObject
{
    Q_OBJECT
public:
    explicit GeojsonBuilder(QObject *parent = nullptr);


private:

    QJsonObject* FeatureCollection;

signals:
};

#endif // GEOJSONBUILDER_H
