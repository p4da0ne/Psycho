#ifndef MAPGEOMETRYROLESSERVICE_H
#define MAPGEOMETRYROLESSERVICE_H

#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QVariantList>
#include <QVariantMap>

#include "config_dataaccess.h"

class __EXPORT_DATAACCESS MapGeometryRolesService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString lastError READ lastError NOTIFY errorChanged)

public:
    explicit MapGeometryRolesService(QObject *parent = nullptr);

    static MapGeometryRolesService* qmlSingleton(QQmlEngine*, QJSEngine*);
    static MapGeometryRolesService* instance();

    QString lastError() const;

    Q_INVOKABLE QVariantList roles(int objectType, int objectId);
    Q_INVOKABLE QVariantList geometry(int objectType, int objectId, const QString &geometryRole);
    Q_INVOKABLE QVariantMap geometryBundle(int objectType, int objectId);

    Q_INVOKABLE bool upsertPosition(int objectType, int objectId, const QVariantMap &point);
    Q_INVOKABLE bool upsertRoute(int objectType, int objectId, const QVariantList &points);
    Q_INVOKABLE bool upsertCoverage(int objectType, int objectId, const QVariantList &points, bool isClosed = true);
    Q_INVOKABLE bool removeRole(int objectType, int objectId, const QString &geometryRole);
    // geometries: [{geometryRole, geometryType, points, isClosed?} ...]
    Q_INVOKABLE bool applyBundle(
        int objectType,
        int objectId,
        const QVariantList &geometries,
        bool replaceAllRoles = false);
    // objects: [{objectType, objectId, geometries:[...]} ...]
    Q_INVOKABLE bool applyObjectsBundles(const QVariantList &objects, bool replaceAllRoles = false);

    Q_INVOKABLE bool replaceRoleGeometry(
        int objectType,
        int objectId,
        const QString &geometryRole,
        const QString &geometryType,
        const QVariantList &points,
        bool isClosed = false);

signals:
    void roleChanged(int objectType, int objectId, QString geometryRole);
    void roleRemoved(int objectType, int objectId, QString geometryRole);
    void bundleChanged(int objectType, int objectId);
    void bundlesBatchChanged();
    void errorChanged();

private:
    static MapGeometryRolesService *s_instance;

    void bindEditing();
    bool m_editingBound = false;
    QString m_lastError;
};

#endif // MAPGEOMETRYROLESSERVICE_H
