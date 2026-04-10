#ifndef GEOMETRYREPOSITORY_H
#define GEOMETRYREPOSITORY_H

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QSqlDatabase>
#include <QVariantList>

#include "config_dataaccess.h"

class __EXPORT_DATAACCESS GeometryRepository : public QObject
{
    Q_OBJECT

public:
    explicit GeometryRepository(QObject *parent = nullptr);

    static GeometryRepository* qmlSingleton(QQmlEngine*, QJSEngine*);
    static GeometryRepository* instance();

    // Returns a list of maps:
    // {
    //   "geometryRole": string,
    //   "geometryType": string,
    //   "pointOrder": int,
    //   "coordinateId": int,
    //   "longitude": double,
    //   "latitude": double
    // }
    Q_INVOKABLE QVariantList loadObjectGeometry(int objectType, int objectId);
    QVariantList loadObjectGeometryWithDb(int objectType, int objectId, const QSqlDatabase &db);

    // coordinates is a QVariantList of coordinate IDs.
    Q_INVOKABLE bool saveObjectGeometry(
        int objectType,
        int objectId,
        const QString &geometryRole,
        const QString &geometryType,
        const QVariantList &coordinates,
        bool isClosed = false);

private:
    static GeometryRepository *s_instance;

    static QString legacyCoordTableForType(int objectType);
    static QString legacyObjectFieldForType(int objectType);
};

#endif // GEOMETRYREPOSITORY_H
