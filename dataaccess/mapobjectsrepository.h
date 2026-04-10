#ifndef MAPOBJECTSREPOSITORY_H
#define MAPOBJECTSREPOSITORY_H

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QSqlDatabase>
#include <QString>
#include <QVariantList>

#include "config_dataaccess.h"

class __EXPORT_DATAACCESS MapObjectsRepository : public QObject
{
    Q_OBJECT

public:
    explicit MapObjectsRepository(QObject *parent = nullptr);

    static MapObjectsRepository* qmlSingleton(QQmlEngine*, QJSEngine*);
    static MapObjectsRepository* instance();

    Q_INVOKABLE QVariantList listObjectsByType(int objectType, int limit = 10000);
    Q_INVOKABLE QString buildObjectGeoJson(int objectType, int objectId);
    Q_INVOKABLE QString buildTypeGeoJson(int objectType, int limit = 5000);
    static QVariantList listObjectsByTypeWithDb(int objectType, int limit, const QSqlDatabase &db);
    static QString buildObjectGeoJsonWithDb(int objectType, int objectId, const QSqlDatabase &db);
    static QString buildTypeGeoJsonWithDb(int objectType, int limit, const QSqlDatabase &db);

private:
    static MapObjectsRepository *s_instance;

    static QString objectSelectSqlForType(int objectType);
};

#endif // MAPOBJECTSREPOSITORY_H
