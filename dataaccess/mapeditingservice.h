#ifndef MAPEDITINGSERVICE_H
#define MAPEDITINGSERVICE_H

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QVariantList>
#include <QVariantMap>

#include "config_dataaccess.h"

class __EXPORT_DATAACCESS MapEditingService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString lastError READ lastError NOTIFY errorChanged)

public:
    explicit MapEditingService(QObject *parent = nullptr);

    static MapEditingService* qmlSingleton(QQmlEngine*, QJSEngine*);
    static MapEditingService* instance();

    QString lastError() const;

    // points: [{longitude: number, latitude: number}, ...]
    Q_INVOKABLE bool replaceGeometry(
        int objectType,
        int objectId,
        const QString &geometryRole,
        const QString &geometryType,
        const QVariantList &points,
        bool isClosed = false);

    Q_INVOKABLE bool replaceGeometryByCoordinateIds(
        int objectType,
        int objectId,
        const QString &geometryRole,
        const QString &geometryType,
        const QVariantList &coordinateIds,
        bool isClosed = false);
    // geometries: [{geometryRole, geometryType, points, isClosed?} ...]
    Q_INVOKABLE bool replaceGeometryBundle(
        int objectType,
        int objectId,
        const QVariantList &geometries,
        bool replaceAllRoles = false);
    // objects: [{objectType, objectId, geometries:[{geometryRole, geometryType, points, isClosed?}, ...]}, ...]
    Q_INVOKABLE bool replaceObjectsGeometryBundles(
        const QVariantList &objects,
        bool replaceAllRoles = false);

    Q_INVOKABLE bool deleteGeometry(int objectType, int objectId, const QString &geometryRole);
    Q_INVOKABLE bool validateGeometry(const QString &geometryType, const QVariantList &points, bool isClosed = false);
    Q_INVOKABLE QVariantList listGeometryRoles(int objectType, int objectId);
    Q_INVOKABLE QVariantList getGeometry(int objectType, int objectId, const QString &geometryRole);

signals:
    void geometryChanged(int objectType, int objectId, QString geometryRole);
    void geometryBundleChanged(int objectType, int objectId);
    void geometriesBatchChanged();
    void errorChanged();

private:
    static MapEditingService *s_instance;

    static double dmsToDecimal(int deg, int min, double sec);
    static void decimalToDms(double decimal, int &deg, int &min, double &sec);
    static QString normalizeGeometryType(const QString &geometryType);
    static QString normalizeGeometryRole(const QString &geometryRole);
    static bool isFiniteCoordinate(double value);
    bool ensureGeometryWritePermission(bool deleteOperation = false);
    bool validateRoleGeometryType(const QString &geometryRole, const QString &geometryType);
    bool setError(const QString &error);

    QString m_lastError;
};

#endif // MAPEDITINGSERVICE_H
