#ifndef GEOJSONSERVICE_H
#define GEOJSONSERVICE_H

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QSqlDatabase>
#include <QString>

#include "config_dataaccess.h"

class __EXPORT_DATAACCESS GeoJsonService : public QObject
{
    Q_OBJECT

public:
    explicit GeoJsonService(QObject *parent = nullptr);

    static GeoJsonService* qmlSingleton(QQmlEngine*, QJSEngine*);
    static GeoJsonService* instance();

    Q_INVOKABLE QString buildFeatureCollection(int objectType, int objectId);
    QString buildFeatureCollectionWithDb(int objectType, int objectId, const QSqlDatabase &db);

private:
    static GeoJsonService *s_instance;
};

#endif // GEOJSONSERVICE_H
