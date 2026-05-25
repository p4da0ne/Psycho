#include "mapobjectsrepository.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariantMap>

#include "dataaccess.h"
#include "geojsonservice.h"
#include "legacycalculationservice.h"
#include "objecttypemapper.h"

namespace {
QString countryScopeSql(const QString &countryAlias)
{
    return QStringLiteral(
        "CASE "
        "WHEN %1.id_country IS NULL THEN 'unknown' "
        "WHEN LOWER(COALESCE(%1.name_country, '')) LIKE 'росс%%' THEN 'russia' "
        "WHEN LOWER(COALESCE(%1.name_country, '')) LIKE 'russia%%' THEN 'russia' "
        "ELSE 'foreign' END")
        .arg(countryAlias);
}
}

MapObjectsRepository *MapObjectsRepository::s_instance = nullptr;

MapObjectsRepository::MapObjectsRepository(QObject *parent)
    : QObject(parent)
{
    if (s_instance == nullptr) {
        s_instance = this;
    }
}

MapObjectsRepository *MapObjectsRepository::qmlSingleton(QQmlEngine*, QJSEngine*)
{
    if (s_instance == nullptr) {
        s_instance = new MapObjectsRepository();
    }
    return s_instance;
}

MapObjectsRepository *MapObjectsRepository::instance()
{
    if (s_instance == nullptr) {
        s_instance = new MapObjectsRepository();
    }
    return s_instance;
}

QVariantList MapObjectsRepository::listObjectsByType(int objectType, int limit)
{
    return listObjectsByTypeWithDb(objectType, limit, QSqlDatabase());
}

QVariantList MapObjectsRepository::listObjectsByTypeWithDb(
    int objectType,
    int limit,
    const QSqlDatabase &dbConnection)
{
    QVariantList items;
    QSqlDatabase db = dbConnection;
    if (!db.isValid()) {
        DataAccess *dataAccess = DataAccess::instance();
        if (!dataAccess->connected() && !dataAccess->connectToDatabase()) {
            return items;
        }
        db = QSqlDatabase::database();
    }
    if (!db.isValid() || !db.isOpen()) {
        return items;
    }

    const QString baseSql = objectSelectSqlForType(objectType);
    if (baseSql.isEmpty()) {
        return items;
    }

    QSqlQuery query(db);
    query.prepare(baseSql + " LIMIT :limit");
    query.bindValue(":limit", limit);
    if (!query.exec()) {
        return items;
    }

    while (query.next()) {
        QVariantMap row;
        const QSqlRecord record = query.record();
        for (int i = 0; i < record.count(); ++i) {
            row.insert(record.fieldName(i), query.value(i));
        }
        row.insert("countryId", row.value("country_id"));
        row.insert("countryScope", row.value("country_scope"));
        row.insert("objectType", objectType);
        items.append(row);
    }

    return items;
}

QString MapObjectsRepository::buildObjectGeoJson(int objectType, int objectId)
{
    return buildObjectGeoJsonWithDb(objectType, objectId, QSqlDatabase());
}

QString MapObjectsRepository::buildObjectGeoJsonWithDb(
    int objectType,
    int objectId,
    const QSqlDatabase &db)
{
    return GeoJsonService::instance()->buildFeatureCollectionWithDb(objectType, objectId, db);
}

QString MapObjectsRepository::buildTypeGeoJson(int objectType, int limit)
{
    return buildTypeGeoJsonWithDb(objectType, limit, QSqlDatabase());
}

QString MapObjectsRepository::buildTypeGeoJsonWithDb(
    int objectType,
    int limit,
    const QSqlDatabase &db)
{
    const QVariantList objects = listObjectsByTypeWithDb(objectType, limit, db);
    QJsonArray featureAccumulator;

    for (const QVariant &itemValue : objects) {
        const QVariantMap item = itemValue.toMap();
        const int objectId = item.value("id").toInt();
        const QString oneObjectJson = buildObjectGeoJsonWithDb(objectType, objectId, db);
        const QVariantMap metrics = LegacyCalculationService::instance()->objectMetricsWithDb(objectType, objectId, db);
        const QJsonDocument oneDoc = QJsonDocument::fromJson(oneObjectJson.toUtf8());
        if (!oneDoc.isObject()) {
            continue;
        }
        const QJsonArray oneFeatures = oneDoc.object().value("features").toArray();
        for (const QJsonValue &featureValue : oneFeatures) {
            QJsonObject feature = featureValue.toObject();
            QJsonObject properties = feature.value("properties").toObject();
            properties.insert("title", item.value("name").toString());
            properties.insert("subtitle", item.value("subtitle").toString());
            properties.insert("countryId", QJsonValue::fromVariant(item.value("countryId")));
            properties.insert("countryScope", QJsonValue::fromVariant(item.value("countryScope")));
            properties.insert("legacySemantic17501", objectId);
            properties.insert("legacySemantic17502", objectType);

            const int idSign = item.value("id_sign").toInt();
            if (idSign > 0) {
                properties.insert("idSign", idSign);
            }
            const QString signKey = item.value("sign_key").toString();
            if (!signKey.isEmpty()) {
                properties.insert("signKey", signKey);
            }

            const QString semantika1 = item.value("semantika_1").toString();
            const QVariant semantikaDigit1 = item.value("semantika_digit1");
            const QVariant semantikaDigit2 = item.value("semantika_digit2");
            if (objectType == 3 || objectType == 4 || objectType == 5) {
                if (semantikaDigit1.isValid() && !semantikaDigit1.isNull()) {
                    properties.insert("legacySemantic18", QJsonValue::fromVariant(semantikaDigit1));
                    properties.insert("legacySemantic32811", QJsonValue::fromVariant(semantikaDigit1));
                }
                if (!semantika1.isEmpty()) {
                    properties.insert("legacySemantic19", semantika1);
                }
                if (semantikaDigit2.isValid() && !semantikaDigit2.isNull()) {
                    properties.insert("legacySemantic32852", QJsonValue::fromVariant(semantikaDigit2));
                }
            } else if (objectType == 1) {
                const QString shortName = item.value("subtitle").toString();
                if (!shortName.isEmpty()) {
                    properties.insert("legacySemantic105", shortName);
                    properties.insert("legacySemantic19", shortName);
                }
            }
            for (auto it = metrics.constBegin(); it != metrics.constEnd(); ++it) {
                properties.insert(it.key(), QJsonValue::fromVariant(it.value()));
            }
            feature.insert("properties", properties);
            featureAccumulator.append(feature);
        }
    }

    QJsonObject collection;
    collection.insert("type", "FeatureCollection");
    collection.insert("features", featureAccumulator);
    return QString::fromUtf8(QJsonDocument(collection).toJson(QJsonDocument::Compact));
}

QString MapObjectsRepository::objectSelectSqlForType(int objectType)
{
    const QString countryScopeCountry = countryScopeSql(QStringLiteral("c"));
    switch (objectType) {
    case 1: // FORMATIONS
        return QStringLiteral(
                   "SELECT l.id_ls AS id, l.name_ls AS name, l.short_name_ls AS subtitle, "
                   "c.id_country AS country_id, %1 AS country_scope, "
                   "tl.id_sign AS id_sign, si.sign_key AS sign_key, "
                   "NULL::text AS semantika_1, NULL::double precision AS semantika_digit1, NULL::double precision AS semantika_digit2 "
                   "FROM ls l "
                   "LEFT JOIN type_ls tl ON tl.id_type_ls = l.id_type_ls "
                   "LEFT JOIN signs si ON si.id_sign = tl.id_sign "
                   "LEFT JOIN region r ON r.id_region = l.id_region "
                   "LEFT JOIN country c ON c.id_country = r.id_country "
                   "ORDER BY l.name_ls")
            .arg(countryScopeCountry);
    case 2: // SPECIAL_CONDITIONS
        return QStringLiteral(
                   "SELECT s.id_special_conditions AS id, s.name_special_conditions AS name, "
                   "COALESCE(t.name_type_special_conditions, '') AS subtitle, "
                   "c.id_country AS country_id, %1 AS country_scope, "
                   "t.id_sign AS id_sign, si.sign_key AS sign_key, "
                   "s.semantika_1 AS semantika_1, NULL::double precision AS semantika_digit1, NULL::double precision AS semantika_digit2 "
                   "FROM special_conditions s "
                   "LEFT JOIN type_special_conditions t ON t.id_type_special_conditions = s.id_type_special_conditions "
                   "LEFT JOIN signs si ON si.id_sign = t.id_sign "
                   "LEFT JOIN region r ON r.id_region = s.id_region "
                   "LEFT JOIN country c ON c.id_country = r.id_country "
                   "ORDER BY s.name_special_conditions")
            .arg(countryScopeCountry);
    case ObjectTypeMapper::SMI_MEANS:
    case ObjectTypeMapper::FORMATIONS_MEANS:
    case ObjectTypeMapper::GROUPS_MEANS:
    {
        const int mpoType = objectType;
        return QStringLiteral(
                   "SELECT m.id_mpo_pso AS id, m.name_mpo_pso AS name, m.semantika_1 AS subtitle, "
                   "c.id_country AS country_id, %1 AS country_scope, "
                   "tm.id_sign AS id_sign, si.sign_key AS sign_key, "
                   "m.semantika_1 AS semantika_1, m.semantika_digit1 AS semantika_digit1, m.semantika_digit2 AS semantika_digit2 "
                   "FROM mpo_pso m "
                   "LEFT JOIN type_mpo_pso tm ON tm.id_type_mpo_pso = m.id_type_mpo_pso "
                   "LEFT JOIN signs si ON si.id_sign = tm.id_sign "
                   "LEFT JOIN ls l ON l.id_ls = m.id_ls "
                   "LEFT JOIN region r_ls ON r_ls.id_region = l.id_region "
                   "LEFT JOIN groups g ON g.id_groups = m.id_groups "
                   "LEFT JOIN region r_gr ON r_gr.id_region = g.id_region "
                   "LEFT JOIN country c ON c.id_country = COALESCE(r_ls.id_country, r_gr.id_country) "
                   "WHERE (CASE "
                   "  WHEN m.id_groups IS NOT NULL THEN 5 "
                   "  WHEN m.id_ls IS NOT NULL THEN 4 "
                   "  WHEN m.id_smi IS NOT NULL THEN 3 "
                   "  ELSE 3 END) = %2 "
                   "ORDER BY m.name_mpo_pso")
            .arg(countryScopeCountry)
            .arg(mpoType);
    }
    case 6: // REGIONS
        return QStringLiteral(
                   "SELECT r.id_region AS id, r.name_region AS name, r.type_region AS subtitle, "
                   "c.id_country AS country_id, %1 AS country_scope, "
                   "tr.id_sign AS id_sign, si.sign_key AS sign_key, "
                   "NULL::text AS semantika_1, NULL::double precision AS semantika_digit1, NULL::double precision AS semantika_digit2 "
                   "FROM region r "
                   "LEFT JOIN type_region tr ON tr.id_type_region = r.id_type_region "
                   "LEFT JOIN signs si ON si.id_sign = tr.id_sign "
                   "LEFT JOIN country c ON c.id_country = r.id_country "
                   "ORDER BY r.name_region")
            .arg(countryScopeCountry);
    case 7: // PERSONNEL
        return QStringLiteral(
                   "SELECT p.id_persones AS id, p.name_persones AS name, p.rank_persones AS subtitle, "
                   "c.id_country AS country_id, %1 AS country_scope, "
                   "tp.id_sign AS id_sign, si.sign_key AS sign_key, "
                   "NULL::text AS semantika_1, NULL::double precision AS semantika_digit1, NULL::double precision AS semantika_digit2 "
                   "FROM persones p "
                   "LEFT JOIN type_persones tp ON tp.id_type_persones = p.id_type_persones "
                   "LEFT JOIN signs si ON si.id_sign = tp.id_sign "
                   "LEFT JOIN ls l ON l.id_ls = p.id_ls "
                   "LEFT JOIN region r_ls ON r_ls.id_region = l.id_region "
                   "LEFT JOIN groups g ON g.id_groups = p.id_groups "
                   "LEFT JOIN region r_gr ON r_gr.id_region = g.id_region "
                   "LEFT JOIN country c ON c.id_country = COALESCE(r_ls.id_country, r_gr.id_country) "
                   "ORDER BY p.name_persones")
            .arg(countryScopeCountry);
    case 8: // EVENTS
        return "SELECT e.id_event AS id, e.name_event AS name, '' AS subtitle, "
               "NULL::int AS country_id, 'unknown'::text AS country_scope, "
               "te.id_sign AS id_sign, si.sign_key AS sign_key, "
               "NULL::text AS semantika_1, NULL::double precision AS semantika_digit1, NULL::double precision AS semantika_digit2 "
               "FROM events e "
               "LEFT JOIN type_event te ON te.id_type_event = e.id_type_event "
               "LEFT JOIN signs si ON si.id_sign = te.id_sign "
               "ORDER BY e.time_event_start DESC NULLS LAST, e.id_event DESC";
    case 9: // GROUPS
        return QStringLiteral(
                   "SELECT g.id_groups AS id, g.name_groups AS name, '' AS subtitle, "
                   "c.id_country AS country_id, %1 AS country_scope, "
                   "NULL::int AS id_sign, ''::text AS sign_key, "
                   "NULL::text AS semantika_1, NULL::double precision AS semantika_digit1, NULL::double precision AS semantika_digit2 "
                   "FROM groups g "
                   "LEFT JOIN region r ON r.id_region = g.id_region "
                   "LEFT JOIN country c ON c.id_country = r.id_country "
                   "ORDER BY g.name_groups")
            .arg(countryScopeCountry);
    case 10: // SMI (non-map object but left for compatibility)
        return QStringLiteral(
                   "SELECT s.id_smi AS id, s.name_smi AS name, s.site_smi AS subtitle, "
                   "c.id_country AS country_id, %1 AS country_scope, "
                   "NULL::int AS id_sign, ''::text AS sign_key, "
                   "NULL::text AS semantika_1, NULL::double precision AS semantika_digit1, NULL::double precision AS semantika_digit2 "
                   "FROM smi s "
                   "LEFT JOIN LATERAL ("
                   "    SELECT sr.id_region "
                   "    FROM smi_region sr "
                   "    WHERE sr.id_smi = s.id_smi "
                   "    ORDER BY sr.id_smi_region ASC "
                   "    LIMIT 1"
                   ") sr1 ON TRUE "
                   "LEFT JOIN region r ON r.id_region = sr1.id_region "
                   "LEFT JOIN country c ON c.id_country = r.id_country "
                   "ORDER BY s.name_smi")
            .arg(countryScopeCountry);
    default:
        return QString();
    }
}
