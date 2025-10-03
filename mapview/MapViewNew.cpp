#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "MapViewNew.h"

MapViewNew::MapViewNew (QWidget *parent){
    this->setSource(QUrl(QString("qrc:/MapView.qml")));
}
