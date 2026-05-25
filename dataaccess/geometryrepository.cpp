#include "geometryrepository.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>
#include <QVariantMap>
#include <QtGlobal>

#include "dataaccess.h"

namespace {
constexpr int FORMATIONS = 1;
constexpr int SPECIAL_CONDITIONS = 2;
constexpr int SMI_MEANS = 3;
constexpr int FORMATIONS_MEANS = 4;
constexpr int GROUPS_MEANS = 5;
constexpr int REGIONS = 6;
constexpr int PERSONNEL = 7;
constexpr int EVENTS = 8;
constexpr int GROUPS = 9;
constexpr int SMI = 10;

double dmsToDecimal(int deg, int min, double sec)
{
    double sign = deg < 0 ? -1.0 : 1.0;
    double absDeg = qAbs(static_cast<double>(deg));
    return sign * (absDeg + static_cast<double>(min) / 60.0 + sec / 3600.0);
}
}

GeometryRepository *GeometryRepository::s_instance = nullptr;

GeometryRepository::GeometryRepository(QObject *parent)
    : QObject(parent)
{
    if (s_instance == nullptr) {
        s_instance = this;
    }
}

GeometryRepository *GeometryRepository::qmlSingleton(QQmlEngine*, QJSEngine*)
{
    if (s_instance == nullptr) {
        s_instance = new GeometryRepository();
    }
    return s_instance;
}

GeometryRepository *GeometryRepository::instance()
{
    if (s_instance == nullptr) {
        s_instance = new GeometryRepository();
    }
    return s_instance;
}

QVariantList GeometryRepository::loadObjectGeometry(int objectType, int objectId)
{
    return loadObjectGeometryWithDb(objectType, objectId, QSqlDatabase());
}

QVariantList GeometryRepository::loadObjectGeometryWithDb(
    int objectType,
    int objectId,
    const QSqlDatabase &dbConnection)
{
    QVariantList result;

    QSqlDatabase db = dbConnection;
    if (!db.isValid()) {
        DataAccess *dataAccess = DataAccess::instance();
        if (!dataAccess->connected() && !dataAccess->connectToDatabase()) {
            return result;
        }
        db = QSqlDatabase::database();
    }

    if (!db.isValid() || !db.isOpen()) {
        return result;
    }

    QSqlQuery query(db);
    query.prepare(
        "SELECT og.geometry_role, og.geometry_type, og.point_order, og.id_coordinates, "
        "c.latitude_wgs_84_g, c.latitude_wgs_84_m, c.latitude_wgs_84_s, "
        "c.longitude_wgs_84_g, c.longitude_wgs_84_m, c.longitude_wgs_84_s "
        "FROM object_geometry og "
        "JOIN coordinates c ON c.id_coordinates = og.id_coordinates "
        "WHERE og.object_type = :object_type AND og.object_id = :object_id "
        "ORDER BY og.geometry_role, og.point_order");
    query.bindValue(":object_type", objectType);
    query.bindValue(":object_id", objectId);

    if (query.exec()) {
        while (query.next()) {
            QVariantMap row;
            row.insert("geometryRole", query.value("geometry_role").toString());
            row.insert("geometryType", query.value("geometry_type").toString());
            row.insert("pointOrder", query.value("point_order").toInt());
            row.insert("coordinateId", query.value("id_coordinates").toInt());
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
            result.append(row);
        }
    }

    if (!result.isEmpty()) {
        return result;
    }

    // Compatibility fallback for legacy coord_* mapping.
    const QString coordTable = legacyCoordTableForType(objectType);
    const QString objectField = legacyObjectFieldForType(objectType);
    if (coordTable.isEmpty() || objectField.isEmpty()) {
        return result;
    }

    QSqlQuery legacyQuery(db);
    const QString sql = QString(
        "SELECT c.id_coordinates, c.latitude_wgs_84_g, c.latitude_wgs_84_m, c.latitude_wgs_84_s, "
        "c.longitude_wgs_84_g, c.longitude_wgs_84_m, c.longitude_wgs_84_s "
        "FROM %1 co "
        "JOIN coordinates c ON c.id_coordinates = co.id_coordinates "
        "WHERE co.%2 = :object_id "
        "ORDER BY c.id_coordinates")
            .arg(coordTable, objectField);
    legacyQuery.prepare(sql);
    legacyQuery.bindValue(":object_id", objectId);

    if (!legacyQuery.exec()) {
        return result;
    }

    const QString geometryType = objectType == REGIONS ? "Polygon" : "LineString";
    int pointOrder = 0;
    while (legacyQuery.next()) {
        QVariantMap row;
        row.insert("geometryRole", "legacy");
        row.insert("geometryType", geometryType);
        row.insert("pointOrder", pointOrder++);
        row.insert("coordinateId", legacyQuery.value("id_coordinates").toInt());
        row.insert(
            "latitude",
            dmsToDecimal(
                legacyQuery.value("latitude_wgs_84_g").toInt(),
                legacyQuery.value("latitude_wgs_84_m").toInt(),
                legacyQuery.value("latitude_wgs_84_s").toDouble()));
        row.insert(
            "longitude",
            dmsToDecimal(
                legacyQuery.value("longitude_wgs_84_g").toInt(),
                legacyQuery.value("longitude_wgs_84_m").toInt(),
                legacyQuery.value("longitude_wgs_84_s").toDouble()));
        result.append(row);
    }

    if (result.size() == 1) {
        QVariantMap row = result.first().toMap();
        row["geometryType"] = "Point";
        result[0] = row;
    }

    return result;
}

QHash<int, QVariantList> GeometryRepository::loadGeometryForType(
    int objectType,
    const QList<int> &objectIds,
    const QSqlDatabase &dbConnection)
{
    QHash<int, QVariantList> result;
    if (objectIds.isEmpty()) {
        return result;
    }

    QSqlDatabase db = dbConnection;
    if (!db.isValid()) {
        DataAccess *dataAccess = DataAccess::instance();
        if (!dataAccess->connected() && !dataAccess->connectToDatabase()) {
            return result;
        }
        db = QSqlDatabase::database();
    }
    if (!db.isValid() || !db.isOpen()) {
        return result;
    }

    QStringList placeholders;
    for (int i = 0; i < objectIds.size(); ++i) {
        placeholders.append(QStringLiteral("?"));
    }
    const QString placeholderList = placeholders.join(QStringLiteral(", "));

    QSqlQuery primary(db);
    const QString primarySql = QStringLiteral(
        "SELECT og.object_id, og.geometry_role, og.geometry_type, og.point_order, og.id_coordinates, "
        "c.latitude_wgs_84_g, c.latitude_wgs_84_m, c.latitude_wgs_84_s, "
        "c.longitude_wgs_84_g, c.longitude_wgs_84_m, c.longitude_wgs_84_s "
        "FROM object_geometry og "
        "JOIN coordinates c ON c.id_coordinates = og.id_coordinates "
        "WHERE og.object_type = ? AND og.object_id IN (%1) "
        "ORDER BY og.object_id, og.geometry_role, og.point_order")
            .arg(placeholderList);
    primary.prepare(primarySql);
    primary.addBindValue(objectType);
    for (int id : objectIds) {
        primary.addBindValue(id);
    }
    if (primary.exec()) {
        while (primary.next()) {
            const int objectId = primary.value("object_id").toInt();
            QVariantMap row;
            row.insert("geometryRole", primary.value("geometry_role").toString());
            row.insert("geometryType", primary.value("geometry_type").toString());
            row.insert("pointOrder", primary.value("point_order").toInt());
            row.insert("coordinateId", primary.value("id_coordinates").toInt());
            row.insert(
                "latitude",
                dmsToDecimal(
                    primary.value("latitude_wgs_84_g").toInt(),
                    primary.value("latitude_wgs_84_m").toInt(),
                    primary.value("latitude_wgs_84_s").toDouble()));
            row.insert(
                "longitude",
                dmsToDecimal(
                    primary.value("longitude_wgs_84_g").toInt(),
                    primary.value("longitude_wgs_84_m").toInt(),
                    primary.value("longitude_wgs_84_s").toDouble()));
            result[objectId].append(row);
        }
    }

    QList<int> missing;
    for (int id : objectIds) {
        if (!result.contains(id)) {
            missing.append(id);
        }
    }
    if (missing.isEmpty()) {
        return result;
    }

    const QString coordTable = legacyCoordTableForType(objectType);
    const QString objectField = legacyObjectFieldForType(objectType);
    if (coordTable.isEmpty() || objectField.isEmpty()) {
        return result;
    }

    QStringList missingPlaceholders;
    for (int i = 0; i < missing.size(); ++i) {
        missingPlaceholders.append(QStringLiteral("?"));
    }

    QSqlQuery legacy(db);
    const QString legacySql = QStringLiteral(
        "SELECT co.%2 AS object_id, c.id_coordinates, "
        "c.latitude_wgs_84_g, c.latitude_wgs_84_m, c.latitude_wgs_84_s, "
        "c.longitude_wgs_84_g, c.longitude_wgs_84_m, c.longitude_wgs_84_s "
        "FROM %1 co "
        "JOIN coordinates c ON c.id_coordinates = co.id_coordinates "
        "WHERE co.%2 IN (%3) "
        "ORDER BY co.%2, c.id_coordinates")
            .arg(coordTable, objectField, missingPlaceholders.join(QStringLiteral(", ")));
    legacy.prepare(legacySql);
    for (int id : missing) {
        legacy.addBindValue(id);
    }
    if (!legacy.exec()) {
        return result;
    }

    const QString legacyGeometryType = objectType == REGIONS
        ? QStringLiteral("Polygon")
        : QStringLiteral("LineString");
    QHash<int, int> nextOrder;
    while (legacy.next()) {
        const int objectId = legacy.value("object_id").toInt();
        const int order = nextOrder.value(objectId, 0);
        QVariantMap row;
        row.insert("geometryRole", "legacy");
        row.insert("geometryType", legacyGeometryType);
        row.insert("pointOrder", order);
        row.insert("coordinateId", legacy.value("id_coordinates").toInt());
        row.insert(
            "latitude",
            dmsToDecimal(
                legacy.value("latitude_wgs_84_g").toInt(),
                legacy.value("latitude_wgs_84_m").toInt(),
                legacy.value("latitude_wgs_84_s").toDouble()));
        row.insert(
            "longitude",
            dmsToDecimal(
                legacy.value("longitude_wgs_84_g").toInt(),
                legacy.value("longitude_wgs_84_m").toInt(),
                legacy.value("longitude_wgs_84_s").toDouble()));
        result[objectId].append(row);
        nextOrder[objectId] = order + 1;
    }

    // Promote single-point legacy LineString to Point (matches single-object loader).
    for (auto it = result.begin(); it != result.end(); ++it) {
        QVariantList &rows = it.value();
        if (rows.size() == 1) {
            QVariantMap row = rows.first().toMap();
            if (row.value("geometryRole").toString() == QStringLiteral("legacy")) {
                row["geometryType"] = QStringLiteral("Point");
                rows[0] = row;
            }
        }
    }

    return result;
}

bool GeometryRepository::saveObjectGeometry(
    int objectType,
    int objectId,
    const QString &geometryRole,
    const QString &geometryType,
    const QVariantList &coordinates,
    bool isClosed)
{
    if (coordinates.isEmpty()) {
        return false;
    }

    DataAccess *db = DataAccess::instance();
    if (!db->connected() && !db->connectToDatabase()) {
        return false;
    }

    QSqlQuery tx;
    if (!tx.exec("BEGIN")) {
        return false;
    }

    QSqlQuery deleteQuery;
    deleteQuery.prepare(
        "DELETE FROM object_geometry "
        "WHERE object_type = :object_type AND object_id = :object_id AND geometry_role = :geometry_role");
    deleteQuery.bindValue(":object_type", objectType);
    deleteQuery.bindValue(":object_id", objectId);
    deleteQuery.bindValue(":geometry_role", geometryRole);
    if (!deleteQuery.exec()) {
        tx.exec("ROLLBACK");
        return false;
    }

    QSqlQuery insertQuery;
    insertQuery.prepare(
        "INSERT INTO object_geometry "
        "(object_type, object_id, geometry_role, geometry_type, id_coordinates, point_order, is_closed) "
        "VALUES (:object_type, :object_id, :geometry_role, :geometry_type, :id_coordinates, :point_order, :is_closed)");

    for (int i = 0; i < coordinates.size(); ++i) {
        insertQuery.bindValue(":object_type", objectType);
        insertQuery.bindValue(":object_id", objectId);
        insertQuery.bindValue(":geometry_role", geometryRole);
        insertQuery.bindValue(":geometry_type", geometryType);
        insertQuery.bindValue(":id_coordinates", coordinates.at(i).toInt());
        insertQuery.bindValue(":point_order", i);
        insertQuery.bindValue(":is_closed", isClosed);
        if (!insertQuery.exec()) {
            tx.exec("ROLLBACK");
            return false;
        }
    }

    return tx.exec("COMMIT");
}

QString GeometryRepository::legacyCoordTableForType(int objectType)
{
    switch (objectType) {
    case FORMATIONS:
        return "coord_ls";
    case SPECIAL_CONDITIONS:
        return "coord_spec_cond";
    case SMI_MEANS:
    case FORMATIONS_MEANS:
    case GROUPS_MEANS:
        return "coord_mpo_pso";
    case REGIONS:
        return "coord_region";
    case PERSONNEL:
        return "coord_persones";
    case EVENTS:
        return "coord_events";
    case GROUPS:
        return "coord_groups";
    case SMI:
    default:
        return QString();
    }
}

QString GeometryRepository::legacyObjectFieldForType(int objectType)
{
    switch (objectType) {
    case FORMATIONS:
        return "id_ls";
    case SPECIAL_CONDITIONS:
        return "id_special_conditions";
    case SMI_MEANS:
    case FORMATIONS_MEANS:
    case GROUPS_MEANS:
        return "id_mpo_pso";
    case REGIONS:
        return "id_region";
    case PERSONNEL:
        return "id_persones";
    case EVENTS:
        return "id_event";
    case GROUPS:
        return "id_groups";
    case SMI:
    default:
        return QString();
    }
}
