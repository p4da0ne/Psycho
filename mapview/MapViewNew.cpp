#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "MapViewNew.h"

MapViewNew::MapViewNew (QWidget *parent){
    this->setSource(QUrl("qrc:/MyMapView.qml"));
}

void MapViewNew::onAfterRendering()
{

    const char* vendor   = (const char*)glGetString(GL_VENDOR);
    const char* renderer = (const char*)glGetString(GL_RENDERER);
    const char* version  = (const char*)glGetString(GL_VERSION);

    qDebug() << "Vendor:" << vendor;
    qDebug() << "Renderer:" << renderer;
    qDebug() << "Version:" << version;
}
