#include "mapobjectsrepository.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QSqlQuery>
#include <QVariantMap>

#include "dataaccess.h"
#include "geojsonservice.h"
#include "legacycalculationservice.h"

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
        row.insert("id", query.value("id").toInt());
        row.insert("name", query.value("name").toString());
        row.insert("subtitle", query.value("subtitle").toString());
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
    switch (objectType) {
    case 1: // FORMATIONS
        return "SELECT id_ls AS id, name_ls AS name, short_name_ls AS subtitle FROM ls ORDER BY name_ls";
    case 2: // SPECIAL_CONDITIONS
        return "SELECT s.id_special_conditions AS id, s.name_special_conditions AS name, "
               "COALESCE(t.name_type_special_conditions, '') AS subtitle "
               "FROM special_conditions s "
               "LEFT JOIN type_special_conditions t ON t.id_type_special_conditions = s.id_type_special_conditions "
               "ORDER BY s.name_special_conditions";
    case 3: // SMI_MEANS
    case 4: // FORMATIONS_MEANS
    case 5: // GROUPS_MEANS
        return "SELECT id_mpo_pso AS id, name_mpo_pso AS name, semantika_1 AS subtitle FROM mpo_pso ORDER BY name_mpo_pso";
    case 6: // REGIONS
        return "SELECT id_region AS id, name_region AS name, type_region AS subtitle FROM region ORDER BY name_region";
    case 7: // PERSONNEL
        return "SELECT id_persones AS id, name_persones AS name, rank_persones AS subtitle FROM persones ORDER BY name_persones";
    case 8: // EVENTS
        return "SELECT id_event AS id, name_event AS name, '' AS subtitle FROM events ORDER BY time_event_start DESC NULLS LAST, id_event DESC";
    case 9: // GROUPS
        return "SELECT id_groups AS id, name_groups AS name, '' AS subtitle FROM groups ORDER BY name_groups";
    case 10: // SMI (non-map object but left for compatibility)
        return "SELECT id_smi AS id, name_smi AS name, site_smi AS subtitle FROM smi ORDER BY name_smi";
    default:
        return QString();
    }
}
