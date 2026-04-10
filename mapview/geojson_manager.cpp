#include "geojson_manager.h"

GeojsonManager::GeojsonManager(QObject *parent)
    : QObject{parent}
{}

QJsonObject GeojsonManager::createProperties(SignData& sign){
    QJsonObject properties;

    properties["signCode"]=sign.getSignCode();

    for (auto semProp = sign.getSemanticList().constBegin(); semProp != sign.getSemanticList().constEnd(); semProp++){
        properties[QString::number(semProp.key())] = semProp.value();
    }

    return properties;
}

QJsonObject GeojsonManager::createGeometry(SignData& sign){
    QList<Coord*> metrics = sign.getMetricList();
    QString markerType = sign.getSemanticList().value(17502);
    QJsonObject geometry;

    switch(markerType.toInt()){
    case SMI_MEANS:
        break;
    case SPECIAL_CONDITIONS:
        break;
    default:
        geometry["type"] = "Point";
        geometry["coordinates"] = QJsonArray{metrics.constFirst()->getLat(), metrics.constFirst()->getLon()};
        break;
    }
}
