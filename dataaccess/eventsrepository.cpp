#include "eventsrepository.h"

#include <QSqlQuery>
#include <QVariantList>

#include "dataaccess.h"

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
        "SELECT e.id_event, e.name_event, e.time_event_start, e.time_event_end, "
        "te.name_type_event, es.name_event_status "
        "FROM events e "
        "LEFT JOIN type_event te ON te.id_type_event = e.id_type_event "
        "LEFT JOIN event_status es ON es.id_event_status = e.id_event_status "
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
        row.insert("name", query.value("name_event").toString());
        row.insert("typeName", query.value("name_type_event").toString());
        row.insert("statusName", query.value("name_event_status").toString());
        row.insert("start", query.value("time_event_start").toDateTime());
        row.insert("end", query.value("time_event_end").toDateTime());
        items.append(row);
    }
    return items;
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
        "SELECT e.id_event, e.name_event, e.description_event, e.resume_event, "
        "e.time_event_start, e.time_event_end, te.name_type_event, es.name_event_status "
        "FROM events e "
        "LEFT JOIN type_event te ON te.id_type_event = e.id_type_event "
        "LEFT JOIN event_status es ON es.id_event_status = e.id_event_status "
        "WHERE e.id_event = :id_event "
        "LIMIT 1");
    query.bindValue(":id_event", eventId);

    if (!query.exec() || !query.next()) {
        return result;
    }

    result.insert("id", query.value("id_event").toInt());
    result.insert("name", query.value("name_event").toString());
    result.insert("description", query.value("description_event").toString());
    result.insert("resume", query.value("resume_event").toString());
    result.insert("typeName", query.value("name_type_event").toString());
    result.insert("statusName", query.value("name_event_status").toString());
    result.insert("start", query.value("time_event_start").toDateTime());
    result.insert("end", query.value("time_event_end").toDateTime());

    QSqlQuery objectsQuery;
    objectsQuery.prepare(
        "SELECT eo.id_object, eo.is_events_source, teo.id_type_event_object, teo.name_type_event_object, teo.table_name "
        "FROM event_objects eo "
        "JOIN type_event_object teo ON teo.id_type_event_object = eo.id_type_event_object "
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
            objects.append(objectRow);
        }
    }

    result.insert("objects", objects);
    return result;
}
