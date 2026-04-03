#include "mapeditingservice.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSet>
#include <QtMath>
#include <QVariantMap>

#include "dataaccess.h"
#include "authmanager.h"

MapEditingService *MapEditingService::s_instance = nullptr;

MapEditingService::MapEditingService(QObject *parent)
    : QObject(parent)
{
    if (s_instance == nullptr) {
        s_instance = this;
    }
}

MapEditingService *MapEditingService::qmlSingleton(QQmlEngine*, QJSEngine*)
{
    if (s_instance == nullptr) {
        s_instance = new MapEditingService();
    }
    return s_instance;
}

MapEditingService *MapEditingService::instance()
{
    if (s_instance == nullptr) {
        s_instance = new MapEditingService();
    }
    return s_instance;
}

QString MapEditingService::lastError() const
{
    return m_lastError;
}

bool MapEditingService::replaceGeometry(
    int objectType,
    int objectId,
    const QString &geometryRole,
    const QString &geometryType,
    const QVariantList &points,
    bool isClosed)
{
    if (!ensureGeometryWritePermission(false)) {
        return false;
    }

    const QString normalizedRole = normalizeGeometryRole(geometryRole);
    const QString normalizedType = normalizeGeometryType(geometryType);
    if (normalizedRole.isEmpty()) {
        return setError("Пустая роль геометрии");
    }
    if (!validateRoleGeometryType(normalizedRole, normalizedType)) {
        return false;
    }
    if (!validateGeometry(normalizedType, points, isClosed)) {
        return false;
    }

    DataAccess *db = DataAccess::instance();
    if (!db->connected() && !db->connectToDatabase()) {
        return setError(db->lastError());
    }

    QSqlQuery tx;
    if (!tx.exec("BEGIN")) {
        return setError("Не удалось начать транзакцию");
    }

    QSqlQuery removeOld;
    removeOld.prepare(
        "DELETE FROM object_geometry "
        "WHERE object_type = :object_type AND object_id = :object_id AND geometry_role = :geometry_role");
    removeOld.bindValue(":object_type", objectType);
    removeOld.bindValue(":object_id", objectId);
    removeOld.bindValue(":geometry_role", normalizedRole);
    if (!removeOld.exec()) {
        tx.exec("ROLLBACK");
        return setError(removeOld.lastError().text());
    }

    QVariantList coordinateIds;
    QSqlQuery insertCoordinate;
    insertCoordinate.prepare(
        "INSERT INTO coordinates "
        "(x_coordinates, y_coordinates, latitude_wgs_84_g, latitude_wgs_84_m, latitude_wgs_84_s, "
        " longitude_wgs_84_g, longitude_wgs_84_m, longitude_wgs_84_s) "
        "VALUES (NULL, NULL, :lat_g, :lat_m, :lat_s, :lon_g, :lon_m, :lon_s) "
        "RETURNING id_coordinates");

    for (const QVariant &pointValue : points) {
        const QVariantMap point = pointValue.toMap();
        const double lon = point.value("longitude").toDouble();
        const double lat = point.value("latitude").toDouble();

        int latDeg = 0;
        int latMin = 0;
        double latSec = 0.0;
        decimalToDms(lat, latDeg, latMin, latSec);

        int lonDeg = 0;
        int lonMin = 0;
        double lonSec = 0.0;
        decimalToDms(lon, lonDeg, lonMin, lonSec);

        insertCoordinate.bindValue(":lat_g", latDeg);
        insertCoordinate.bindValue(":lat_m", latMin);
        insertCoordinate.bindValue(":lat_s", latSec);
        insertCoordinate.bindValue(":lon_g", lonDeg);
        insertCoordinate.bindValue(":lon_m", lonMin);
        insertCoordinate.bindValue(":lon_s", lonSec);

        if (!insertCoordinate.exec() || !insertCoordinate.next()) {
            tx.exec("ROLLBACK");
            return setError(insertCoordinate.lastError().text());
        }
        coordinateIds.append(insertCoordinate.value("id_coordinates").toInt());
    }

    QSqlQuery insertGeometry;
    insertGeometry.prepare(
        "INSERT INTO object_geometry "
        "(object_type, object_id, geometry_role, geometry_type, id_coordinates, point_order, is_closed) "
        "VALUES (:object_type, :object_id, :geometry_role, :geometry_type, :id_coordinates, :point_order, :is_closed)");

    for (int i = 0; i < coordinateIds.size(); ++i) {
        insertGeometry.bindValue(":object_type", objectType);
        insertGeometry.bindValue(":object_id", objectId);
        insertGeometry.bindValue(":geometry_role", normalizedRole);
        insertGeometry.bindValue(":geometry_type", normalizedType);
        insertGeometry.bindValue(":id_coordinates", coordinateIds.at(i).toInt());
        insertGeometry.bindValue(":point_order", i);
        insertGeometry.bindValue(":is_closed", isClosed);
        if (!insertGeometry.exec()) {
            tx.exec("ROLLBACK");
            return setError(insertGeometry.lastError().text());
        }
    }

    if (!tx.exec("COMMIT")) {
        tx.exec("ROLLBACK");
        return setError("Не удалось зафиксировать транзакцию");
    }

    m_lastError.clear();
    emit errorChanged();
    emit geometryChanged(objectType, objectId, normalizedRole);
    return true;
}

bool MapEditingService::replaceGeometryByCoordinateIds(
    int objectType,
    int objectId,
    const QString &geometryRole,
    const QString &geometryType,
    const QVariantList &coordinateIds,
    bool isClosed)
{
    if (!ensureGeometryWritePermission(false)) {
        return false;
    }

    const QString normalizedRole = normalizeGeometryRole(geometryRole);
    const QString normalizedType = normalizeGeometryType(geometryType);
    if (normalizedRole.isEmpty()) {
        return setError("Пустая роль геометрии");
    }
    if (!validateRoleGeometryType(normalizedRole, normalizedType)) {
        return false;
    }
    if (coordinateIds.isEmpty()) {
        return setError("Список coordinateIds пуст");
    }

    DataAccess *db = DataAccess::instance();
    if (!db->connected() && !db->connectToDatabase()) {
        return setError(db->lastError());
    }

    QSqlQuery tx;
    if (!tx.exec("BEGIN")) {
        return setError("Не удалось начать транзакцию");
    }

    QSqlQuery removeOld;
    removeOld.prepare(
        "DELETE FROM object_geometry "
        "WHERE object_type = :object_type AND object_id = :object_id AND geometry_role = :geometry_role");
    removeOld.bindValue(":object_type", objectType);
    removeOld.bindValue(":object_id", objectId);
    removeOld.bindValue(":geometry_role", normalizedRole);
    if (!removeOld.exec()) {
        tx.exec("ROLLBACK");
        return setError(removeOld.lastError().text());
    }

    QSqlQuery insertGeometry;
    insertGeometry.prepare(
        "INSERT INTO object_geometry "
        "(object_type, object_id, geometry_role, geometry_type, id_coordinates, point_order, is_closed) "
        "VALUES (:object_type, :object_id, :geometry_role, :geometry_type, :id_coordinates, :point_order, :is_closed)");

    for (int i = 0; i < coordinateIds.size(); ++i) {
        const int coordinateId = coordinateIds.at(i).toInt();
        insertGeometry.bindValue(":object_type", objectType);
        insertGeometry.bindValue(":object_id", objectId);
        insertGeometry.bindValue(":geometry_role", normalizedRole);
        insertGeometry.bindValue(":geometry_type", normalizedType);
        insertGeometry.bindValue(":id_coordinates", coordinateId);
        insertGeometry.bindValue(":point_order", i);
        insertGeometry.bindValue(":is_closed", isClosed);
        if (!insertGeometry.exec()) {
            tx.exec("ROLLBACK");
            return setError(insertGeometry.lastError().text());
        }
    }

    if (!tx.exec("COMMIT")) {
        tx.exec("ROLLBACK");
        return setError("Не удалось зафиксировать транзакцию");
    }

    m_lastError.clear();
    emit errorChanged();
    emit geometryChanged(objectType, objectId, normalizedRole);
    return true;
}

bool MapEditingService::replaceGeometryBundle(
    int objectType,
    int objectId,
    const QVariantList &geometries,
    bool replaceAllRoles)
{
    if (!ensureGeometryWritePermission(false)) {
        return false;
    }

    if (geometries.isEmpty()) {
        return setError("Пустой набор геометрий");
    }

    QVariantList normalized;
    QSet<QString> roleSet;
    for (const QVariant &geometryValue : geometries) {
        const QVariantMap geometry = geometryValue.toMap();
        const QString role = normalizeGeometryRole(geometry.value("geometryRole").toString());
        const QString type = normalizeGeometryType(geometry.value("geometryType").toString());
        const QVariantList points = geometry.value("points").toList();
        const bool isClosed = geometry.value("isClosed").toBool();

        if (role.isEmpty()) {
            return setError("В наборе геометрий указана пустая geometryRole");
        }
        if (roleSet.contains(role)) {
            return setError("В наборе геометрий обнаружен дубликат geometryRole: " + role);
        }
        if (!validateRoleGeometryType(role, type)) {
            return false;
        }
        if (!validateGeometry(type, points, isClosed)) {
            return false;
        }

        roleSet.insert(role);
        QVariantMap row;
        row.insert("geometryRole", role);
        row.insert("geometryType", type);
        row.insert("points", points);
        row.insert("isClosed", isClosed);
        normalized.append(row);
    }

    DataAccess *db = DataAccess::instance();
    if (!db->connected() && !db->connectToDatabase()) {
        return setError(db->lastError());
    }

    QSqlQuery tx;
    if (!tx.exec("BEGIN")) {
        return setError("Не удалось начать транзакцию");
    }

    QSqlQuery removeRoleQuery;
    bool fullReplacePending = replaceAllRoles;
    if (fullReplacePending) {
        removeRoleQuery.prepare(
            "DELETE FROM object_geometry "
            "WHERE object_type = :object_type AND object_id = :object_id");
    } else {
        removeRoleQuery.prepare(
            "DELETE FROM object_geometry "
            "WHERE object_type = :object_type AND object_id = :object_id AND geometry_role = :geometry_role");
    }

    QSqlQuery insertCoordinate;
    insertCoordinate.prepare(
        "INSERT INTO coordinates "
        "(x_coordinates, y_coordinates, latitude_wgs_84_g, latitude_wgs_84_m, latitude_wgs_84_s, "
        " longitude_wgs_84_g, longitude_wgs_84_m, longitude_wgs_84_s) "
        "VALUES (NULL, NULL, :lat_g, :lat_m, :lat_s, :lon_g, :lon_m, :lon_s) "
        "RETURNING id_coordinates");

    QSqlQuery insertGeometry;
    insertGeometry.prepare(
        "INSERT INTO object_geometry "
        "(object_type, object_id, geometry_role, geometry_type, id_coordinates, point_order, is_closed) "
        "VALUES (:object_type, :object_id, :geometry_role, :geometry_type, :id_coordinates, :point_order, :is_closed)");

    for (const QVariant &rowValue : normalized) {
        const QVariantMap row = rowValue.toMap();
        const QString role = row.value("geometryRole").toString();
        const QString type = row.value("geometryType").toString();
        const QVariantList points = row.value("points").toList();
        const bool isClosed = row.value("isClosed").toBool();

        removeRoleQuery.bindValue(":object_type", objectType);
        removeRoleQuery.bindValue(":object_id", objectId);
        if (!fullReplacePending) {
            removeRoleQuery.bindValue(":geometry_role", role);
        }
        if (!removeRoleQuery.exec()) {
            tx.exec("ROLLBACK");
            return setError(removeRoleQuery.lastError().text());
        }
        if (fullReplacePending) {
            fullReplacePending = false;
            removeRoleQuery.prepare(
                "DELETE FROM object_geometry "
                "WHERE object_type = :object_type AND object_id = :object_id AND geometry_role = :geometry_role");
        }

        QVariantList coordinateIds;
        for (const QVariant &pointValue : points) {
            const QVariantMap point = pointValue.toMap();
            const double lon = point.value("longitude").toDouble();
            const double lat = point.value("latitude").toDouble();

            int latDeg = 0;
            int latMin = 0;
            double latSec = 0.0;
            decimalToDms(lat, latDeg, latMin, latSec);

            int lonDeg = 0;
            int lonMin = 0;
            double lonSec = 0.0;
            decimalToDms(lon, lonDeg, lonMin, lonSec);

            insertCoordinate.bindValue(":lat_g", latDeg);
            insertCoordinate.bindValue(":lat_m", latMin);
            insertCoordinate.bindValue(":lat_s", latSec);
            insertCoordinate.bindValue(":lon_g", lonDeg);
            insertCoordinate.bindValue(":lon_m", lonMin);
            insertCoordinate.bindValue(":lon_s", lonSec);

            if (!insertCoordinate.exec() || !insertCoordinate.next()) {
                tx.exec("ROLLBACK");
                return setError(insertCoordinate.lastError().text());
            }
            coordinateIds.append(insertCoordinate.value("id_coordinates").toInt());
        }

        for (int i = 0; i < coordinateIds.size(); ++i) {
            insertGeometry.bindValue(":object_type", objectType);
            insertGeometry.bindValue(":object_id", objectId);
            insertGeometry.bindValue(":geometry_role", role);
            insertGeometry.bindValue(":geometry_type", type);
            insertGeometry.bindValue(":id_coordinates", coordinateIds.at(i).toInt());
            insertGeometry.bindValue(":point_order", i);
            insertGeometry.bindValue(":is_closed", isClosed);
            if (!insertGeometry.exec()) {
                tx.exec("ROLLBACK");
                return setError(insertGeometry.lastError().text());
            }
        }
    }

    if (!tx.exec("COMMIT")) {
        tx.exec("ROLLBACK");
        return setError("Не удалось зафиксировать транзакцию");
    }

    m_lastError.clear();
    emit errorChanged();
    for (const QVariant &rowValue : normalized) {
        const QVariantMap row = rowValue.toMap();
        emit geometryChanged(objectType, objectId, row.value("geometryRole").toString());
    }
    emit geometryBundleChanged(objectType, objectId);
    return true;
}

bool MapEditingService::replaceObjectsGeometryBundles(
    const QVariantList &objects,
    bool replaceAllRoles)
{
    if (!ensureGeometryWritePermission(false)) {
        return false;
    }

    if (objects.isEmpty()) {
        return setError("Пустой набор объектов");
    }

    QVariantList normalizedObjects;
    QSet<QString> objectKeySet;

    for (const QVariant &objectValue : objects) {
        const QVariantMap objectMap = objectValue.toMap();
        const int objectType = objectMap.value("objectType").toInt();
        const int objectId = objectMap.value("objectId").toInt();
        const QVariantList geometries = objectMap.value("geometries").toList();

        if (objectType <= 0 || objectId <= 0) {
            return setError("objectType и objectId должны быть положительными");
        }
        if (geometries.isEmpty()) {
            return setError("Для одного из объектов не переданы геометрии");
        }

        const QString objectKey = QString::number(objectType) + ":" + QString::number(objectId);
        if (objectKeySet.contains(objectKey)) {
            return setError("Обнаружен дубликат объекта в батче: " + objectKey);
        }
        objectKeySet.insert(objectKey);

        QVariantList normalizedGeometries;
        QSet<QString> roleSet;
        for (const QVariant &geometryValue : geometries) {
            const QVariantMap geometry = geometryValue.toMap();
            const QString role = normalizeGeometryRole(geometry.value("geometryRole").toString());
            const QString type = normalizeGeometryType(geometry.value("geometryType").toString());
            const QVariantList points = geometry.value("points").toList();
            const bool isClosed = geometry.value("isClosed").toBool();

            if (role.isEmpty()) {
                return setError("В батче обнаружена пустая geometryRole");
            }
            if (roleSet.contains(role)) {
                return setError("В объекте " + objectKey + " обнаружен дубликат geometryRole: " + role);
            }
            if (!validateRoleGeometryType(role, type)) {
                return false;
            }
            if (!validateGeometry(type, points, isClosed)) {
                return false;
            }

            roleSet.insert(role);
            QVariantMap row;
            row.insert("geometryRole", role);
            row.insert("geometryType", type);
            row.insert("points", points);
            row.insert("isClosed", isClosed);
            normalizedGeometries.append(row);
        }

        QVariantMap normalizedObject;
        normalizedObject.insert("objectType", objectType);
        normalizedObject.insert("objectId", objectId);
        normalizedObject.insert("geometries", normalizedGeometries);
        normalizedObjects.append(normalizedObject);
    }

    DataAccess *db = DataAccess::instance();
    if (!db->connected() && !db->connectToDatabase()) {
        return setError(db->lastError());
    }

    QSqlQuery tx;
    if (!tx.exec("BEGIN")) {
        return setError("Не удалось начать транзакцию");
    }

    QSqlQuery deleteAllQuery;
    deleteAllQuery.prepare(
        "DELETE FROM object_geometry "
        "WHERE object_type = :object_type AND object_id = :object_id");

    QSqlQuery deleteRoleQuery;
    deleteRoleQuery.prepare(
        "DELETE FROM object_geometry "
        "WHERE object_type = :object_type AND object_id = :object_id AND geometry_role = :geometry_role");

    QSqlQuery insertCoordinate;
    insertCoordinate.prepare(
        "INSERT INTO coordinates "
        "(x_coordinates, y_coordinates, latitude_wgs_84_g, latitude_wgs_84_m, latitude_wgs_84_s, "
        " longitude_wgs_84_g, longitude_wgs_84_m, longitude_wgs_84_s) "
        "VALUES (NULL, NULL, :lat_g, :lat_m, :lat_s, :lon_g, :lon_m, :lon_s) "
        "RETURNING id_coordinates");

    QSqlQuery insertGeometry;
    insertGeometry.prepare(
        "INSERT INTO object_geometry "
        "(object_type, object_id, geometry_role, geometry_type, id_coordinates, point_order, is_closed) "
        "VALUES (:object_type, :object_id, :geometry_role, :geometry_type, :id_coordinates, :point_order, :is_closed)");

    for (const QVariant &objectValue : normalizedObjects) {
        const QVariantMap objectMap = objectValue.toMap();
        const int objectType = objectMap.value("objectType").toInt();
        const int objectId = objectMap.value("objectId").toInt();
        const QVariantList geometries = objectMap.value("geometries").toList();

        if (replaceAllRoles) {
            deleteAllQuery.bindValue(":object_type", objectType);
            deleteAllQuery.bindValue(":object_id", objectId);
            if (!deleteAllQuery.exec()) {
                tx.exec("ROLLBACK");
                return setError(deleteAllQuery.lastError().text());
            }
        }

        for (const QVariant &geometryValue : geometries) {
            const QVariantMap geometry = geometryValue.toMap();
            const QString role = geometry.value("geometryRole").toString();
            const QString type = geometry.value("geometryType").toString();
            const QVariantList points = geometry.value("points").toList();
            const bool isClosed = geometry.value("isClosed").toBool();

            if (!replaceAllRoles) {
                deleteRoleQuery.bindValue(":object_type", objectType);
                deleteRoleQuery.bindValue(":object_id", objectId);
                deleteRoleQuery.bindValue(":geometry_role", role);
                if (!deleteRoleQuery.exec()) {
                    tx.exec("ROLLBACK");
                    return setError(deleteRoleQuery.lastError().text());
                }
            }

            QVariantList coordinateIds;
            for (const QVariant &pointValue : points) {
                const QVariantMap point = pointValue.toMap();
                const double lon = point.value("longitude").toDouble();
                const double lat = point.value("latitude").toDouble();

                int latDeg = 0;
                int latMin = 0;
                double latSec = 0.0;
                decimalToDms(lat, latDeg, latMin, latSec);

                int lonDeg = 0;
                int lonMin = 0;
                double lonSec = 0.0;
                decimalToDms(lon, lonDeg, lonMin, lonSec);

                insertCoordinate.bindValue(":lat_g", latDeg);
                insertCoordinate.bindValue(":lat_m", latMin);
                insertCoordinate.bindValue(":lat_s", latSec);
                insertCoordinate.bindValue(":lon_g", lonDeg);
                insertCoordinate.bindValue(":lon_m", lonMin);
                insertCoordinate.bindValue(":lon_s", lonSec);

                if (!insertCoordinate.exec() || !insertCoordinate.next()) {
                    tx.exec("ROLLBACK");
                    return setError(insertCoordinate.lastError().text());
                }
                coordinateIds.append(insertCoordinate.value("id_coordinates").toInt());
            }

            for (int i = 0; i < coordinateIds.size(); ++i) {
                insertGeometry.bindValue(":object_type", objectType);
                insertGeometry.bindValue(":object_id", objectId);
                insertGeometry.bindValue(":geometry_role", role);
                insertGeometry.bindValue(":geometry_type", type);
                insertGeometry.bindValue(":id_coordinates", coordinateIds.at(i).toInt());
                insertGeometry.bindValue(":point_order", i);
                insertGeometry.bindValue(":is_closed", isClosed);
                if (!insertGeometry.exec()) {
                    tx.exec("ROLLBACK");
                    return setError(insertGeometry.lastError().text());
                }
            }
        }
    }

    if (!tx.exec("COMMIT")) {
        tx.exec("ROLLBACK");
        return setError("Не удалось зафиксировать транзакцию");
    }

    m_lastError.clear();
    emit errorChanged();
    for (const QVariant &objectValue : normalizedObjects) {
        const QVariantMap objectMap = objectValue.toMap();
        const int objectType = objectMap.value("objectType").toInt();
        const int objectId = objectMap.value("objectId").toInt();
        const QVariantList geometries = objectMap.value("geometries").toList();
        for (const QVariant &geometryValue : geometries) {
            const QVariantMap geometry = geometryValue.toMap();
            emit geometryChanged(objectType, objectId, geometry.value("geometryRole").toString());
        }
        emit geometryBundleChanged(objectType, objectId);
    }
    emit geometriesBatchChanged();
    return true;
}

bool MapEditingService::deleteGeometry(int objectType, int objectId, const QString &geometryRole)
{
    if (!ensureGeometryWritePermission(true)) {
        return false;
    }

    const QString normalizedRole = normalizeGeometryRole(geometryRole);
    DataAccess *db = DataAccess::instance();
    if (!db->connected() && !db->connectToDatabase()) {
        return setError(db->lastError());
    }

    QSqlQuery query;
    query.prepare(
        "DELETE FROM object_geometry "
        "WHERE object_type = :object_type AND object_id = :object_id AND geometry_role = :geometry_role");
    query.bindValue(":object_type", objectType);
    query.bindValue(":object_id", objectId);
    query.bindValue(":geometry_role", normalizedRole);
    if (!query.exec()) {
        return setError(query.lastError().text());
    }

    m_lastError.clear();
    emit errorChanged();
    emit geometryChanged(objectType, objectId, normalizedRole);
    return true;
}

bool MapEditingService::validateGeometry(const QString &geometryType, const QVariantList &points, bool isClosed)
{
    const QString type = normalizeGeometryType(geometryType);
    if (type != "Point" && type != "LineString" && type != "Polygon") {
        return setError("Неподдерживаемый тип геометрии");
    }

    if (type == "Point" && points.size() != 1) {
        return setError("Point должен содержать ровно 1 точку");
    }
    if (type == "LineString" && points.size() < 2) {
        return setError("LineString должен содержать минимум 2 точки");
    }
    if (type == "Polygon" && points.size() < 3) {
        return setError("Polygon должен содержать минимум 3 точки");
    }
    if (type == "Polygon" && !isClosed) {
        // We allow saving and closure is handled on rendering side too.
    }

    for (const QVariant &pointValue : points) {
        const QVariantMap point = pointValue.toMap();
        if (!point.contains("longitude") || !point.contains("latitude")) {
            return setError("Каждая точка должна содержать longitude и latitude");
        }
        const double lon = point.value("longitude").toDouble();
        const double lat = point.value("latitude").toDouble();
        if (!isFiniteCoordinate(lon) || !isFiniteCoordinate(lat)) {
            return setError("Координаты должны быть конечными числами");
        }
        if (lon < -180.0 || lon > 180.0) {
            return setError("Долгота должна быть в диапазоне [-180, 180]");
        }
        if (lat < -90.0 || lat > 90.0) {
            return setError("Широта должна быть в диапазоне [-90, 90]");
        }
    }

    m_lastError.clear();
    emit errorChanged();
    return true;
}

QVariantList MapEditingService::listGeometryRoles(int objectType, int objectId)
{
    QVariantList roles;
    DataAccess *db = DataAccess::instance();
    if (!db->connected() && !db->connectToDatabase()) {
        setError(db->lastError());
        return roles;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT geometry_role, geometry_type, COUNT(*) AS points_count, MAX(is_closed) AS is_closed "
        "FROM object_geometry "
        "WHERE object_type = :object_type AND object_id = :object_id "
        "GROUP BY geometry_role, geometry_type "
        "ORDER BY geometry_role");
    query.bindValue(":object_type", objectType);
    query.bindValue(":object_id", objectId);

    if (!query.exec()) {
        setError(query.lastError().text());
        return roles;
    }

    while (query.next()) {
        QVariantMap row;
        row.insert("geometryRole", query.value("geometry_role").toString());
        row.insert("geometryType", query.value("geometry_type").toString());
        row.insert("pointsCount", query.value("points_count").toInt());
        row.insert("isClosed", query.value("is_closed").toBool());
        roles.append(row);
    }
    return roles;
}

QVariantList MapEditingService::getGeometry(int objectType, int objectId, const QString &geometryRole)
{
    const QString normalizedRole = normalizeGeometryRole(geometryRole);
    QVariantList points;
    DataAccess *db = DataAccess::instance();
    if (!db->connected() && !db->connectToDatabase()) {
        setError(db->lastError());
        return points;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT og.geometry_role, og.geometry_type, og.point_order, og.id_coordinates, og.is_closed, "
        "c.latitude_wgs_84_g, c.latitude_wgs_84_m, c.latitude_wgs_84_s, "
        "c.longitude_wgs_84_g, c.longitude_wgs_84_m, c.longitude_wgs_84_s "
        "FROM object_geometry og "
        "JOIN coordinates c ON c.id_coordinates = og.id_coordinates "
        "WHERE og.object_type = :object_type AND og.object_id = :object_id AND og.geometry_role = :geometry_role "
        "ORDER BY og.point_order");
    query.bindValue(":object_type", objectType);
    query.bindValue(":object_id", objectId);
    query.bindValue(":geometry_role", normalizedRole);

    if (!query.exec()) {
        setError(query.lastError().text());
        return points;
    }

    while (query.next()) {
        QVariantMap row;
        row.insert("geometryRole", query.value("geometry_role").toString());
        row.insert("geometryType", query.value("geometry_type").toString());
        row.insert("pointOrder", query.value("point_order").toInt());
        row.insert("coordinateId", query.value("id_coordinates").toInt());
        row.insert("isClosed", query.value("is_closed").toBool());
        row.insert(
            "latitude",
            dmsToDecimal(
                query.value("latitude_wgs_84_g").toInt(),
                query.value("latitude_wgs_84_m").toInt(),
                query.value("latitude_wgs_84_s").toDouble()));
        row.insert(
            "longitude",
            dmsToDecimal(
                query.value("longitude_wgs_84_g").toInt(),
                query.value("longitude_wgs_84_m").toInt(),
                query.value("longitude_wgs_84_s").toDouble()));
        points.append(row);
    }

    return points;
}

double MapEditingService::dmsToDecimal(int deg, int min, double sec)
{
    const double sign = deg < 0 ? -1.0 : 1.0;
    const double absDeg = qAbs(static_cast<double>(deg));
    return sign * (absDeg + static_cast<double>(min) / 60.0 + sec / 3600.0);
}

void MapEditingService::decimalToDms(double decimal, int &deg, int &min, double &sec)
{
    const double absValue = qAbs(decimal);
    deg = static_cast<int>(qFloor(absValue));
    const double minutesFull = (absValue - static_cast<double>(deg)) * 60.0;
    min = static_cast<int>(qFloor(minutesFull));
    sec = (minutesFull - static_cast<double>(min)) * 60.0;

    if (decimal < 0.0) {
        deg = -deg;
    }
}

bool MapEditingService::setError(const QString &error)
{
    m_lastError = error;
    emit errorChanged();
    return false;
}

bool MapEditingService::ensureGeometryWritePermission(bool deleteOperation)
{
    AuthManager *auth = AuthManager::instance();
    if (!auth->loggedIn()) {
        return setError("Требуется авторизация для изменения геометрии");
    }
    if (deleteOperation) {
        if (!auth->canDelete()) {
            return setError("Недостаточно прав для удаления геометрии");
        }
        return true;
    }
    if (!auth->canEditGeometry()) {
        return setError("Недостаточно прав для редактирования геометрии");
    }
    return true;
}

QString MapEditingService::normalizeGeometryType(const QString &geometryType)
{
    const QString lowered = geometryType.trimmed().toLower();
    if (lowered == "point") {
        return "Point";
    }
    if (lowered == "linestring") {
        return "LineString";
    }
    if (lowered == "polygon") {
        return "Polygon";
    }
    return geometryType.trimmed();
}

QString MapEditingService::normalizeGeometryRole(const QString &geometryRole)
{
    return geometryRole.trimmed().toLower();
}

bool MapEditingService::isFiniteCoordinate(double value)
{
    return qIsFinite(value);
}

bool MapEditingService::validateRoleGeometryType(const QString &geometryRole, const QString &geometryType)
{
    const QString role = normalizeGeometryRole(geometryRole);
    const QString type = normalizeGeometryType(geometryType);

    if (role == "position" && type != "Point") {
        return setError("Для роли position допустим только тип Point");
    }
    if (role == "route" && type != "LineString") {
        return setError("Для роли route допустим только тип LineString");
    }
    if (role == "coverage" && type != "Polygon") {
        return setError("Для роли coverage допустим только тип Polygon");
    }

    return true;
}
