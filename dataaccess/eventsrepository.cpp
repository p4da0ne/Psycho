#include "eventsrepository.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariantList>

#include "dataaccess.h"
#include "objecttypemapper.h"

namespace {
QVariantList queryRows(QSqlQuery &query)
{
    QVariantList rows;
    if (!query.exec()) {
        return rows;
    }
    const QSqlRecord rec = query.record();
    while (query.next()) {
        QVariantMap row;
        for (int i = 0; i < rec.count(); ++i) {
            row.insert(rec.fieldName(i), query.value(i));
        }
        rows.append(row);
    }
    return rows;
}
}

EventsRepository *EventsRepository::s_instance = nullptr;

EventsRepository::EventsRepository(QObject *parent)
    : QObject(parent)
{
    if (s_instance == nullptr) {
        s_instance = this;
    }
}

EventsRepository *EventsRepository::qmlSingleton(QQmlEngine*, QJSEngine*)
{
    if (s_instance == nullptr) {
        s_instance = new EventsRepository();
    }
    return s_instance;
}

EventsRepository *EventsRepository::instance()
{
    if (s_instance == nullptr) {
        s_instance = new EventsRepository();
    }
    return s_instance;
}

QVariantList EventsRepository::listEvents(const QString &nameFilter, int limit, int offset)
{
    QVariantList items;
    DataAccess *db = DataAccess::instance();
    if (!db->connected() && !db->connectToDatabase()) {
        return items;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT e.id_event, e.id_type_event, e.id_event_status, e.name_event, e.time_event_start, e.time_event_end, "
        "te.name_type_event, te.id_sign, si.sign_key, es.name_event_status, coord.x_coordinates, coord.y_coordinates "
        "FROM events e "
        "LEFT JOIN type_event te ON te.id_type_event = e.id_type_event "
        "LEFT JOIN signs si ON si.id_sign = te.id_sign "
        "LEFT JOIN event_status es ON es.id_event_status = e.id_event_status "
        "LEFT JOIN LATERAL ("
        "    SELECT c.x_coordinates, c.y_coordinates "
        "    FROM coord_events ce "
        "    JOIN coordinates c ON c.id_coordinates = ce.id_coordinates "
        "    WHERE ce.id_event = e.id_event "
        "    ORDER BY ce.id_coord_event ASC "
        "    LIMIT 1"
        ") coord ON TRUE "
        "WHERE (:name_filter = '' OR e.name_event ILIKE :name_like) "
        "ORDER BY e.time_event_start DESC NULLS LAST, e.id_event DESC "
        "LIMIT :limit OFFSET :offset");
    query.bindValue(":name_filter", nameFilter.trimmed());
    query.bindValue(":name_like", "%" + nameFilter.trimmed() + "%");
    query.bindValue(":limit", limit);
    query.bindValue(":offset", offset);

    if (!query.exec()) {
        return items;
    }

    while (query.next()) {
        QVariantMap row;
        row.insert("id", query.value("id_event").toInt());
        row.insert("idTypeEvent", query.value("id_type_event").toInt());
        row.insert("idEventStatus", query.value("id_event_status").toInt());
        row.insert("name", query.value("name_event").toString());
        row.insert("typeName", query.value("name_type_event").toString());
        row.insert("idSign", query.value("id_sign").toInt());
        row.insert("signKey", query.value("sign_key").toString());
        row.insert("statusName", query.value("name_event_status").toString());
        row.insert("start", query.value("time_event_start").toDateTime());
        row.insert("end", query.value("time_event_end").toDateTime());
        row.insert("lon", query.value("x_coordinates").toDouble());
        row.insert("lat", query.value("y_coordinates").toDouble());
        items.append(row);
    }
    return items;
}

QVariantList EventsRepository::eventTypeCatalog()
{
    QVariantList rows;
    DataAccess *db = DataAccess::instance();
    if (!db->connected() && !db->connectToDatabase()) {
        return rows;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT te.id_type_event, te.name_type_event, te.id_sign, si.sign_key "
        "FROM type_event te "
        "LEFT JOIN signs si ON si.id_sign = te.id_sign "
        "ORDER BY te.id_type_event ASC");

    if (!query.exec()) {
        return rows;
    }

    while (query.next()) {
        QVariantMap row;
        row.insert("idTypeEvent", query.value("id_type_event").toInt());
        row.insert("typeName", query.value("name_type_event").toString());
        row.insert("idSign", query.value("id_sign").toInt());
        row.insert("signKey", query.value("sign_key").toString());
        rows.append(row);
    }
    return rows;
}

QVariantMap EventsRepository::eventDetails(int eventId)
{
    QVariantMap result;
    DataAccess *db = DataAccess::instance();
    if (!db->connected() && !db->connectToDatabase()) {
        return result;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT e.id_event, e.id_type_event, e.id_event_status, e.name_event, e.description_event, e.resume_event, "
        "e.time_event_start, e.time_event_end, te.name_type_event, te.id_sign, si.sign_key, es.name_event_status "
        "FROM events e "
        "LEFT JOIN type_event te ON te.id_type_event = e.id_type_event "
        "LEFT JOIN signs si ON si.id_sign = te.id_sign "
        "LEFT JOIN event_status es ON es.id_event_status = e.id_event_status "
        "WHERE e.id_event = :id_event "
        "LIMIT 1");
    query.bindValue(":id_event", eventId);

    if (!query.exec() || !query.next()) {
        return result;
    }

    result.insert("id", query.value("id_event").toInt());
    result.insert("idTypeEvent", query.value("id_type_event").toInt());
    result.insert("idEventStatus", query.value("id_event_status").toInt());
    result.insert("name", query.value("name_event").toString());
    result.insert("description", query.value("description_event").toString());
    result.insert("resume", query.value("resume_event").toString());
    result.insert("typeName", query.value("name_type_event").toString());
    result.insert("idSign", query.value("id_sign").toInt());
    result.insert("signKey", query.value("sign_key").toString());
    result.insert("statusName", query.value("name_event_status").toString());
    result.insert("start", query.value("time_event_start").toDateTime());
    result.insert("end", query.value("time_event_end").toDateTime());

    QSqlQuery objectsQuery;
    objectsQuery.prepare(
        "SELECT eo.id_object, eo.is_events_source, teo.id_type_event_object, teo.name_type_event_object, teo.table_name, "
        "m.id_ls, m.id_smi, m.id_groups "
        "FROM event_objects eo "
        "JOIN type_event_object teo ON teo.id_type_event_object = eo.id_type_event_object "
        "LEFT JOIN mpo_pso m ON teo.table_name = 'mpo_pso' AND m.id_mpo_pso = eo.id_object "
        "WHERE eo.id_event = :id_event "
        "ORDER BY eo.is_events_source DESC, eo.id_event_objects ASC");
    objectsQuery.bindValue(":id_event", eventId);

    QVariantList objects;
    if (objectsQuery.exec()) {
        while (objectsQuery.next()) {
            QVariantMap objectRow;
            objectRow.insert("idObject", objectsQuery.value("id_object").toInt());
            objectRow.insert("isEventSource", objectsQuery.value("is_events_source").toBool());
            objectRow.insert("typeEventObjectId", objectsQuery.value("id_type_event_object").toInt());
            objectRow.insert("typeEventObjectName", objectsQuery.value("name_type_event_object").toString());
            objectRow.insert("tableName", objectsQuery.value("table_name").toString());
            objectRow.insert("idLs", objectsQuery.value("id_ls").toInt());
            objectRow.insert("idSmi", objectsQuery.value("id_smi").toInt());
            objectRow.insert("idGroups", objectsQuery.value("id_groups").toInt());
            objectRow.insert(
                "objectType",
                ObjectTypeMapper::objectTypeByTableName(
                    objectsQuery.value("table_name").toString(),
                    objectsQuery.value("id_ls").toInt(),
                    objectsQuery.value("id_smi").toInt(),
                    objectsQuery.value("id_groups").toInt()));
            objects.append(objectRow);
        }
    }

    result.insert("objects", objects);

    QSqlQuery mediaQuery;
    mediaQuery.prepare(
        "SELECT em.id_event_media, mt.type_name, em.filename_media, em.description "
        "FROM event_media em "
        "LEFT JOIN media_type mt ON mt.id_media_type = em.id_media_type "
        "WHERE em.id_event = :id_event "
        "ORDER BY em.id_event_media ASC");
    mediaQuery.bindValue(":id_event", eventId);
    result.insert("media", queryRows(mediaQuery));

    QSqlQuery coordinatesQuery;
    coordinatesQuery.prepare(
        "SELECT c.id_coordinates, c.latitude_wgs_84_g, c.latitude_wgs_84_m, c.latitude_wgs_84_s, "
        "c.longitude_wgs_84_g, c.longitude_wgs_84_m, c.longitude_wgs_84_s, "
        "c.x_coordinates, c.y_coordinates "
        "FROM coord_events ce "
        "JOIN coordinates c ON c.id_coordinates = ce.id_coordinates "
        "WHERE ce.id_event = :id_event "
        "ORDER BY ce.id_coord_event ASC");
    coordinatesQuery.bindValue(":id_event", eventId);
    result.insert("coordinates", queryRows(coordinatesQuery));

    return result;
}
