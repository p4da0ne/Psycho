#ifndef GEOMETRYREPOSITORY_H
#define GEOMETRYREPOSITORY_H

#include <QHash>
#include <QJSEngine>
#include <QList>
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

    // Batch loader: single SELECT for all objectIds of the given type.
    // Returns hash object_id -> list of row maps (same shape as loadObjectGeometryWithDb).
    // Legacy coord_* fallback applies to ids missing from object_geometry.
    QHash<int, QVariantList> loadGeometryForType(
        int objectType,
        const QList<int> &objectIds,
        const QSqlDatabase &db);

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
