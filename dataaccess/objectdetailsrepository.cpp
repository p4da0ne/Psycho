#include "objectdetailsrepository.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariantList>

#include "dataaccess.h"
#include "eventsrepository.h"
#include "personnelrepository.h"

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

QVariantMap readSingleRow(QSqlQuery &query)
{
    QVariantMap row;
    if (!query.exec() || !query.next()) {
        return row;
    }
    const QSqlRecord record = query.record();
    for (int i = 0; i < record.count(); ++i) {
        row.insert(record.fieldName(i), query.value(i));
    }
    return row;
}
}

ObjectDetailsRepository *ObjectDetailsRepository::s_instance = nullptr;

ObjectDetailsRepository::ObjectDetailsRepository(QObject *parent)
    : QObject(parent)
{
    if (s_instance == nullptr) {
        s_instance = this;
    }
}

ObjectDetailsRepository *ObjectDetailsRepository::qmlSingleton(QQmlEngine*, QJSEngine*)
{
    if (s_instance == nullptr) {
        s_instance = new ObjectDetailsRepository();
    }
    return s_instance;
}

ObjectDetailsRepository *ObjectDetailsRepository::instance()
{
    if (s_instance == nullptr) {
        s_instance = new ObjectDetailsRepository();
    }
    return s_instance;
}

QVariantMap ObjectDetailsRepository::objectSummary(int objectType, int objectId)
{
    QVariantMap result;
    result.insert("objectType", objectType);
    result.insert("objectId", objectId);

    DataAccess *db = DataAccess::instance();
    if (!db->connected() && !db->connectToDatabase()) {
        return result;
    }

    QSqlQuery query;
    switch (objectType) {
    case FORMATIONS:
        query.prepare(
            "SELECT tl.name_type_ls AS type_name, ls.name_ls AS object_name "
            "FROM ls ls "
            "LEFT JOIN type_ls tl ON tl.id_type_ls = ls.id_type_ls "
            "WHERE ls.id_ls = :id LIMIT 1");
        query.bindValue(":id", objectId);
        break;
    case SPECIAL_CONDITIONS:
        query.prepare(
            "SELECT t.name_type_special_conditions AS type_name, s.name_special_conditions AS object_name "
            "FROM special_conditions s "
            "LEFT JOIN type_special_conditions t ON t.id_type_special_conditions = s.id_type_special_conditions "
            "WHERE s.id_special_conditions = :id LIMIT 1");
        query.bindValue(":id", objectId);
        break;
    case SMI_MEANS:
    case FORMATIONS_MEANS:
    case GROUPS_MEANS:
        query.prepare(
            "SELECT t.name_type_mpo_pso AS type_name, m.name_mpo_pso AS object_name "
            "FROM mpo_pso m "
            "LEFT JOIN type_mpo_pso t ON t.id_type_mpo_pso = m.id_type_mpo_pso "
            "WHERE m.id_mpo_pso = :id LIMIT 1");
        query.bindValue(":id", objectId);
        break;
    case REGIONS:
        query.prepare(
            "SELECT t.name_type_region AS type_name, r.name_region AS object_name "
            "FROM region r "
            "LEFT JOIN type_region t ON t.id_type_region = r.id_type_region "
            "WHERE r.id_region = :id LIMIT 1");
        query.bindValue(":id", objectId);
        break;
    case PERSONNEL:
        query.prepare(
            "SELECT p.rank_persones AS type_name, p.name_persones AS object_name "
            "FROM persones p WHERE p.id_persones = :id LIMIT 1");
        query.bindValue(":id", objectId);
        break;
    case EVENTS:
        query.prepare(
            "SELECT t.name_type_event AS type_name, e.name_event AS object_name "
            "FROM events e "
            "LEFT JOIN type_event t ON t.id_type_event = e.id_type_event "
            "WHERE e.id_event = :id LIMIT 1");
        query.bindValue(":id", objectId);
        break;
    case GROUPS:
        query.prepare(
            "SELECT r.name_region AS type_name, g.name_groups AS object_name "
            "FROM groups g "
            "LEFT JOIN region r ON r.id_region = g.id_region "
            "WHERE g.id_groups = :id LIMIT 1");
        query.bindValue(":id", objectId);
        break;
    case SMI:
        query.prepare(
            "SELECT t.nametype_smi AS type_name, s.name_smi AS object_name "
            "FROM smi s "
            "LEFT JOIN type_smi t ON t.id_type_smi = s.id_type_smi "
            "WHERE s.id_smi = :id LIMIT 1");
        query.bindValue(":id", objectId);
        break;
    default:
        return result;
    }

    const QVariantMap row = readSingleRow(query);
    if (row.isEmpty()) {
        return result;
    }

    result.insert("typeName", row.value("type_name").toString());
    result.insert("objectName", row.value("object_name").toString());
    result.insert("title", QString("%1 (%2)").arg(row.value("object_name").toString(), row.value("type_name").toString()));
    return result;
}

QVariantMap ObjectDetailsRepository::objectDetails(int objectType, int objectId)
{
    QVariantMap details = objectSummary(objectType, objectId);

    DataAccess *db = DataAccess::instance();
    if (!db->connected() && !db->connectToDatabase()) {
        return details;
    }

    switch (objectType) {
    case FORMATIONS: {
        QSqlQuery query;
        query.prepare(
            "SELECT ls.id_ls, ls.name_ls, ls.short_name_ls, ls.counte_ls, ls.enimy_ls, "
            "ls.id_region, ls.id_type_ls, tl.name_type_ls, r.name_region "
            "FROM ls ls "
            "LEFT JOIN type_ls tl ON tl.id_type_ls = ls.id_type_ls "
            "LEFT JOIN region r ON r.id_region = ls.id_region "
            "WHERE ls.id_ls = :id LIMIT 1");
        query.bindValue(":id", objectId);
        details.insert("payload", readSingleRow(query));
        break;
    }
    case SPECIAL_CONDITIONS: {
        QSqlQuery query;
        query.prepare(
            "SELECT s.id_special_conditions, s.name_special_conditions, s.description_special_conditions, "
            "s.semantika_1, s.semantika_2, s.id_region, r.name_region, "
            "t.name_type_special_conditions "
            "FROM special_conditions s "
            "LEFT JOIN region r ON r.id_region = s.id_region "
            "LEFT JOIN type_special_conditions t ON t.id_type_special_conditions = s.id_type_special_conditions "
            "WHERE s.id_special_conditions = :id LIMIT 1");
        query.bindValue(":id", objectId);
        details.insert("payload", readSingleRow(query));
        break;
    }
    case SMI_MEANS:
    case FORMATIONS_MEANS:
    case GROUPS_MEANS: {
        QSqlQuery query;
        query.prepare(
            "SELECT m.id_mpo_pso, m.name_mpo_pso, m.counte_mpo_pso, m.description_mpo_pso, "
            "m.semantika_1, m.semantika_digit1, m.semantika_digit2, m.id_ls, m.id_smi, m.id_groups, "
            "t.name_type_mpo_pso "
            "FROM mpo_pso m "
            "LEFT JOIN type_mpo_pso t ON t.id_type_mpo_pso = m.id_type_mpo_pso "
            "WHERE m.id_mpo_pso = :id LIMIT 1");
        query.bindValue(":id", objectId);
        details.insert("payload", readSingleRow(query));
        break;
    }
    case REGIONS: {
        QSqlQuery query;
        query.prepare(
            "SELECT r.id_region, r.name_region, r.description_region, r.parent_region, "
            "r.counte_population, r.density_population, r.id_country, c.name_country, "
            "t.name_type_region "
            "FROM region r "
            "LEFT JOIN country c ON c.id_country = r.id_country "
            "LEFT JOIN type_region t ON t.id_type_region = r.id_type_region "
            "WHERE r.id_region = :id LIMIT 1");
        query.bindValue(":id", objectId);
        details.insert("payload", readSingleRow(query));
        break;
    }
    case PERSONNEL:
        details.insert("payload", PersonnelRepository::instance()->personnelDetails(objectId));
        break;
    case EVENTS:
        details.insert("payload", EventsRepository::instance()->eventDetails(objectId));
        break;
    case GROUPS: {
        QSqlQuery query;
        query.prepare(
            "SELECT g.id_groups, g.name_groups, g.description_groups, g.id_region, r.name_region, "
            "g.id_form_groups, fg.name_form_groups, g.id_sphere_groups, sg.name_sphere_groups, "
            "g.id_trend, tg.name_trend_groups, g.opposition_groups, g.cooperation_rls_groups "
            "FROM groups g "
            "LEFT JOIN region r ON r.id_region = g.id_region "
            "LEFT JOIN form_groups fg ON fg.id_form_groups = g.id_form_groups "
            "LEFT JOIN sphere_groups sg ON sg.id_sphere_groups = g.id_sphere_groups "
            "LEFT JOIN trend_groups tg ON tg.id_trend_groups = g.id_trend "
            "WHERE g.id_groups = :id LIMIT 1");
        query.bindValue(":id", objectId);
        details.insert("payload", readSingleRow(query));
        break;
    }
    case SMI: {
        QSqlQuery query;
        query.prepare(
            "SELECT s.id_smi, s.name_smi, s.site_smi, s.description_smi, s.power_broadcast_smi, "
            "s.frequency_smi, s.auditoria, s.tirag, ts.nametype_smi, "
            "tb.name_type_broadcast_smi, tofs.name_type_office_smi, th.name_theme_smi "
            "FROM smi s "
            "LEFT JOIN type_smi ts ON ts.id_type_smi = s.id_type_smi "
            "LEFT JOIN type_broadcast_smi tb ON tb.id_type_broadcast_smi = s.id_type_broadcast_smi "
            "LEFT JOIN type_office_smi tofs ON tofs.id_type_office_smi = s.id_type_office_smi "
            "LEFT JOIN theme_smi th ON th.id_theme_smi = s.id_theme_smi "
            "WHERE s.id_smi = :id LIMIT 1");
        query.bindValue(":id", objectId);
        details.insert("payload", readSingleRow(query));
        break;
    }
    default:
        break;
    }

    return details;
}
