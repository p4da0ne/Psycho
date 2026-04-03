#include "mapgeometryrolesservice.h"

#include "mapeditingservice.h"

MapGeometryRolesService *MapGeometryRolesService::s_instance = nullptr;

MapGeometryRolesService::MapGeometryRolesService(QObject *parent)
    : QObject(parent)
{
    if (s_instance == nullptr) {
        s_instance = this;
    }
    bindEditing();
}

MapGeometryRolesService *MapGeometryRolesService::qmlSingleton(QQmlEngine*, QJSEngine*)
{
    if (s_instance == nullptr) {
        s_instance = new MapGeometryRolesService();
    }
    s_instance->bindEditing();
    return s_instance;
}

MapGeometryRolesService *MapGeometryRolesService::instance()
{
    if (s_instance == nullptr) {
        s_instance = new MapGeometryRolesService();
    }
    s_instance->bindEditing();
    return s_instance;
}

QString MapGeometryRolesService::lastError() const
{
    return m_lastError;
}

QVariantList MapGeometryRolesService::roles(int objectType, int objectId)
{
    MapEditingService *editing = MapEditingService::instance();
    QVariantList result = editing->listGeometryRoles(objectType, objectId);
    m_lastError = editing->lastError();
    emit errorChanged();
    return result;
}

QVariantList MapGeometryRolesService::geometry(int objectType, int objectId, const QString &geometryRole)
{
    MapEditingService *editing = MapEditingService::instance();
    QVariantList result = editing->getGeometry(objectType, objectId, geometryRole);
    m_lastError = editing->lastError();
    emit errorChanged();
    return result;
}

QVariantMap MapGeometryRolesService::geometryBundle(int objectType, int objectId)
{
    QVariantMap bundle;
    const QVariantList currentRoles = roles(objectType, objectId);
    bundle.insert("objectType", objectType);
    bundle.insert("objectId", objectId);
    bundle.insert("roles", currentRoles);

    QVariantMap geometries;
    for (const QVariant &roleValue : currentRoles) {
        const QVariantMap role = roleValue.toMap();
        const QString roleName = role.value("geometryRole").toString();
        geometries.insert(roleName, geometry(objectType, objectId, roleName));
    }
    bundle.insert("geometries", geometries);
    return bundle;
}

bool MapGeometryRolesService::upsertPosition(int objectType, int objectId, const QVariantMap &point)
{
    QVariantList points;
    points.append(point);
    return replaceRoleGeometry(objectType, objectId, "position", "Point", points, false);
}

bool MapGeometryRolesService::upsertRoute(int objectType, int objectId, const QVariantList &points)
{
    return replaceRoleGeometry(objectType, objectId, "route", "LineString", points, false);
}

bool MapGeometryRolesService::upsertCoverage(int objectType, int objectId, const QVariantList &points, bool isClosed)
{
    return replaceRoleGeometry(objectType, objectId, "coverage", "Polygon", points, isClosed);
}

bool MapGeometryRolesService::removeRole(int objectType, int objectId, const QString &geometryRole)
{
    MapEditingService *editing = MapEditingService::instance();
    const bool ok = editing->deleteGeometry(objectType, objectId, geometryRole);
    m_lastError = editing->lastError();
    emit errorChanged();
    if (ok) {
        emit roleRemoved(objectType, objectId, geometryRole);
    }
    return ok;
}

bool MapGeometryRolesService::applyBundle(
    int objectType,
    int objectId,
    const QVariantList &geometries,
    bool replaceAllRoles)
{
    MapEditingService *editing = MapEditingService::instance();
    const bool ok = editing->replaceGeometryBundle(objectType, objectId, geometries, replaceAllRoles);
    m_lastError = editing->lastError();
    emit errorChanged();
    if (ok) {
        for (const QVariant &geometryValue : geometries) {
            const QVariantMap geometry = geometryValue.toMap();
            emit roleChanged(objectType, objectId, geometry.value("geometryRole").toString().trimmed().toLower());
        }
    }
    return ok;
}

bool MapGeometryRolesService::applyObjectsBundles(const QVariantList &objects, bool replaceAllRoles)
{
    MapEditingService *editing = MapEditingService::instance();
    const bool ok = editing->replaceObjectsGeometryBundles(objects, replaceAllRoles);
    m_lastError = editing->lastError();
    emit errorChanged();
    return ok;
}

bool MapGeometryRolesService::replaceRoleGeometry(
    int objectType,
    int objectId,
    const QString &geometryRole,
    const QString &geometryType,
    const QVariantList &points,
    bool isClosed)
{
    MapEditingService *editing = MapEditingService::instance();
    const bool ok = editing->replaceGeometry(
        objectType,
        objectId,
        geometryRole,
        geometryType,
        points,
        isClosed);
    m_lastError = editing->lastError();
    emit errorChanged();
    if (ok) {
        emit roleChanged(objectType, objectId, geometryRole);
    }
    return ok;
}

void MapGeometryRolesService::bindEditing()
{
    if (m_editingBound) {
        return;
    }
    MapEditingService *editing = MapEditingService::instance();
    connect(
        editing,
        &MapEditingService::geometryBundleChanged,
        this,
        [this](int objectType, int objectId) {
            emit bundleChanged(objectType, objectId);
        },
        Qt::UniqueConnection);
    connect(
        editing,
        &MapEditingService::geometriesBatchChanged,
        this,
        [this]() {
            emit bundlesBatchChanged();
        },
        Qt::UniqueConnection);
    connect(
        editing,
        &MapEditingService::errorChanged,
        this,
        [this, editing]() {
            m_lastError = editing->lastError();
            emit errorChanged();
        },
        Qt::UniqueConnection);
    m_editingBound = true;
}
