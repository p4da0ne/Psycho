#include "personnelrepository.h"

#include <QSqlQuery>
#include <QVariantList>

#include "dataaccess.h"

PersonnelRepository *PersonnelRepository::s_instance = nullptr;

PersonnelRepository::PersonnelRepository(QObject *parent)
    : QObject(parent)
{
    if (s_instance == nullptr) {
        s_instance = this;
    }
}

PersonnelRepository *PersonnelRepository::qmlSingleton(QQmlEngine*, QJSEngine*)
{
    if (s_instance == nullptr) {
        s_instance = new PersonnelRepository();
    }
    return s_instance;
}

PersonnelRepository *PersonnelRepository::instance()
{
    if (s_instance == nullptr) {
        s_instance = new PersonnelRepository();
    }
    return s_instance;
}

QVariantList PersonnelRepository::listPersonnel(const QString &nameFilter, int limit, int offset)
{
    QVariantList items;
    DataAccess *db = DataAccess::instance();
    if (!db->connected() && !db->connectToDatabase()) {
        return items;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT p.id_persones, p.name_persones, p.rank_persones, p.birth_date, tp.name_type_persones "
        "FROM persones p "
        "LEFT JOIN type_persones tp ON tp.id_type_persones = p.id_type_persones "
        "WHERE (:name_filter = '' OR p.name_persones ILIKE :name_like) "
        "ORDER BY p.name_persones ASC NULLS LAST, p.id_persones ASC "
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
        row.insert("id", query.value("id_persones").toInt());
        row.insert("name", query.value("name_persones").toString());
        row.insert("rank", query.value("rank_persones").toString());
        row.insert("birthDate", query.value("birth_date").toDate());
        row.insert("typeName", query.value("name_type_persones").toString());
        items.append(row);
    }
    return items;
}

QVariantMap PersonnelRepository::personnelDetails(int personesId)
{
    QVariantMap result;
    DataAccess *db = DataAccess::instance();
    if (!db->connected() && !db->connectToDatabase()) {
        return result;
    }

    QSqlQuery query;
    query.prepare(
        "SELECT p.id_persones, p.surname, p.name, p.patronumic, p.name_persones, p.rank_persones, "
        "p.birth_date, p.birth_place, p.nationality, p.personal_number, p.family_address, "
        "tp.name_type_persones, p.id_ls, p.id_groups, p.id_smi "
        "FROM persones p "
        "LEFT JOIN type_persones tp ON tp.id_type_persones = p.id_type_persones "
        "WHERE p.id_persones = :id_persones "
        "LIMIT 1");
    query.bindValue(":id_persones", personesId);

    if (!query.exec() || !query.next()) {
        return result;
    }

    result.insert("id", query.value("id_persones").toInt());
    result.insert("surname", query.value("surname").toString());
    result.insert("name", query.value("name").toString());
    result.insert("patronumic", query.value("patronumic").toString());
    result.insert("displayName", query.value("name_persones").toString());
    result.insert("rank", query.value("rank_persones").toString());
    result.insert("birthDate", query.value("birth_date").toDate());
    result.insert("birthPlace", query.value("birth_place").toString());
    result.insert("nationality", query.value("nationality").toString());
    result.insert("personalNumber", query.value("personal_number").toString());
    result.insert("familyAddress", query.value("family_address").toString());
    result.insert("typeName", query.value("name_type_persones").toString());
    result.insert("idLs", query.value("id_ls").toInt());
    result.insert("idGroups", query.value("id_groups").toInt());
    result.insert("idSmi", query.value("id_smi").toInt());

    QVariantList documents;
    QSqlQuery docsQuery;
    docsQuery.prepare(
        "SELECT dt.type_name, dd.sequence, dd.number, dd.organ, dd.date "
        "FROM document_data dd "
        "LEFT JOIN document_type dt ON dt.id_document_type = dd.id_document_type "
        "WHERE dd.id_persones = :id_persones "
        "ORDER BY dd.id_document_data ASC");
    docsQuery.bindValue(":id_persones", personesId);
    if (docsQuery.exec()) {
        while (docsQuery.next()) {
            QVariantMap doc;
            doc.insert("typeName", docsQuery.value("type_name").toString());
            doc.insert("sequence", docsQuery.value("sequence").toString());
            doc.insert("number", docsQuery.value("number").toString());
            doc.insert("organ", docsQuery.value("organ").toString());
            doc.insert("date", docsQuery.value("date").toDate());
            documents.append(doc);
        }
    }
    result.insert("documents", documents);

    return result;
}
