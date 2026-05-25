#include "legacycalculationservice.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QStringList>
#include <QtMath>

#include "dataaccess.h"
#include "objecttypemapper.h"

namespace {
QString buildInPlaceholders(int count)
{
    QStringList parts;
    for (int i = 0; i < count; ++i) {
        parts.append(QStringLiteral("?"));
    }
    return parts.join(QStringLiteral(", "));
}
}

LegacyCalculationService *LegacyCalculationService::s_instance = nullptr;

LegacyCalculationService::LegacyCalculationService(QObject *parent)
    : QObject(parent)
{
    if (s_instance == nullptr) {
        s_instance = this;
    }
}

LegacyCalculationService *LegacyCalculationService::qmlSingleton(QQmlEngine*, QJSEngine*)
{
    if (s_instance == nullptr) {
        s_instance = new LegacyCalculationService();
    }
    return s_instance;
}

LegacyCalculationService *LegacyCalculationService::instance()
{
    if (s_instance == nullptr) {
        s_instance = new LegacyCalculationService();
    }
    return s_instance;
}

QVariantMap LegacyCalculationService::objectMetrics(int objectType, int objectId)
{
    QSet<QString> guard;
    return objectMetricsInternal(objectType, objectId, QSqlDatabase(), guard);
}

QVariantMap LegacyCalculationService::objectMetricsWithDb(
    int objectType,
    int objectId,
    const QSqlDatabase &db)
{
    QSet<QString> guard;
    return objectMetricsInternal(objectType, objectId, db, guard);
}

QVariantMap LegacyCalculationService::objectMetricsInternal(
    int objectType,
    int objectId,
    const QSqlDatabase &dbConnection,
    QSet<QString> &guard)
{
    if (objectType <= 0 || objectId <= 0) {
        return neutralMetrics("invalid", "objectType/objectId must be positive");
    }

    const QString guardKey = QString::number(objectType) + ":" + QString::number(objectId);
    if (guard.contains(guardKey)) {
        return neutralMetrics("cycle_guard", "recursive object dependency detected");
    }
    guard.insert(guardKey);

    QSqlDatabase db = dbConnection;
    if (!db.isValid()) {
        DataAccess *dataAccess = DataAccess::instance();
        if (!dataAccess->connected() && !dataAccess->connectToDatabase()) {
            guard.remove(guardKey);
            return neutralMetrics("db_unavailable", dataAccess->lastError());
        }
        db = QSqlDatabase::database();
    }
    if (!db.isValid() || !db.isOpen()) {
        guard.remove(guardKey);
        return neutralMetrics("db_unavailable", "database connection is not open");
    }

    QVariantMap metrics;
    switch (objectType) {
    case ObjectTypeMapper::FORMATIONS:
        metrics = formationMetrics(objectId, db);
        break;
    case ObjectTypeMapper::REGIONS:
        metrics = regionMetrics(objectId, db);
        break;
    case ObjectTypeMapper::SMI_MEANS:
    case ObjectTypeMapper::FORMATIONS_MEANS:
    case ObjectTypeMapper::GROUPS_MEANS:
        metrics = mpoMetrics(objectId, db, guard);
        break;
    case ObjectTypeMapper::GROUPS:
        metrics = groupMetrics(objectId, db);
        break;
    case ObjectTypeMapper::PERSONNEL:
        metrics = personnelMetrics(objectId, db, guard);
        break;
    case ObjectTypeMapper::SPECIAL_CONDITIONS:
        metrics = specialConditionMetrics(objectId, db);
        break;
    case ObjectTypeMapper::EVENTS:
        metrics = eventMetrics(objectId, db, guard);
        break;
    default:
        metrics = neutralMetrics("unsupported_object_type");
        break;
    }

    metrics.insert("objectType", objectType);
    metrics.insert("objectId", objectId);

    guard.remove(guardKey);
    return metrics;
}

QVariantMap LegacyCalculationService::formationMetrics(int lsId, const QSqlDatabase &db)
{
    QSqlQuery query(db);
    query.prepare(
        "SELECT mps_priz_ls, mps_konrt_ls, mps_of_ls, counte_ls, enimy_ls "
        "FROM ls WHERE id_ls = :id_ls LIMIT 1");
    query.bindValue(":id_ls", lsId);
    if (!query.exec() || !query.next()) {
        return neutralMetrics("formation_mps", "formation not found");
    }

    const double mpsPriz = query.value("mps_priz_ls").toDouble();
    const double mpsKontr = query.value("mps_konrt_ls").toDouble();
    const double mpsOfficer = query.value("mps_of_ls").toDouble();
    const int peopleCount = query.value("counte_ls").toInt();
    const bool enemyFlag = query.value("enimy_ls").toBool();

    const double mpsValue = (mpsPriz + mpsKontr + mpsOfficer) / 3.0;
    const double normalized = clamp01(mpsValue);
    const double heatValue = 1.0 - normalized;

    const double baseMin[3] = {0.05, 0.10, 0.20};
    const double baseMax[3] = {0.05, 0.15, 0.30};
    const double share[4] = {0.80, 0.20 * 0.65, 0.20 * 0.10, 0.20 * 0.25};

    QVariantList lossMinMatrix;
    QVariantList lossMaxMatrix;
    for (int severity = 0; severity < 3; ++severity) {
        QVariantList minRow;
        QVariantList maxRow;
        for (int category = 0; category < 4; ++category) {
            const int minValue = static_cast<int>(static_cast<double>(peopleCount) * baseMin[severity] * share[category]);
            const int maxValue = static_cast<int>(static_cast<double>(peopleCount) * baseMax[severity] * share[category]);
            minRow.append(minValue);
            maxRow.append(maxValue);
        }
        lossMinMatrix.append(minRow);
        lossMaxMatrix.append(maxRow);
    }

    QVariantMap metrics;
    metrics.insert("calcSource", "formation_mps");
    metrics.insert("legacyScoreRaw", mpsValue);
    metrics.insert("legacyScoreNormalized", normalized);
    metrics.insert("legacyClass", mpsLegacyClass(mpsValue));
    metrics.insert("heatValue", heatValue);
    metrics.insert("heatClass", scoreToHeatClass(heatValue));
    metrics.insert("mpsPriz", mpsPriz);
    metrics.insert("mpsKontr", mpsKontr);
    metrics.insert("mpsOfficer", mpsOfficer);
    metrics.insert("enemy", enemyFlag);
    metrics.insert("peopleCount", peopleCount);
    metrics.insert("lossesMin", lossMinMatrix);
    metrics.insert("lossesMax", lossMaxMatrix);
    metrics.insert("lossesOperationalBandMin", lossMinMatrix.value(1));
    metrics.insert("lossesOperationalBandMax", lossMaxMatrix.value(1));
    return metrics;
}

QVariantMap LegacyCalculationService::regionMetrics(int regionId, const QSqlDatabase &db)
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM region WHERE id_region = :id_region LIMIT 1");
    query.bindValue(":id_region", regionId);
    if (!query.exec() || !query.next()) {
        return neutralMetrics("region_k_omkrf", "region not found");
    }

    const QSqlRecord rec = query.record();
    if (rec.count() <= 81) {
        return neutralMetrics("region_k_omkrf", "region schema mismatch for legacy formula");
    }

    auto valueAt = [&query](int index) -> double {
        return query.value(index).toDouble();
    };

    double kSnf = 0.0;
    for (int i = 0; i < 17; ++i) {
        const double y = valueAt(13 + 2 * i);
        const double p = valueAt(13 + 2 * i + 1);
        kSnf += y * p;
    }
    kSnf /= 17.0;

    double kKf = 0.0;
    for (int i = 0; i < 10; ++i) {
        const double y = valueAt(47 + 2 * i);
        const double p = valueAt(47 + 2 * i + 1);
        kKf += y * p;
    }
    kKf /= 10.0;

    double kRf = 0.0;
    for (int i = 0; i < 6; ++i) {
        const double y = valueAt(67 + 2 * i);
        const double p = valueAt(67 + 2 * i + 1);
        kRf += y * p;
    }
    kRf /= 6.0;

    const double pSnf = valueAt(79);
    const double pKf = valueAt(80);
    const double pRf = valueAt(81);

    const double result = (kSnf * pSnf + kKf * pKf + kRf * pRf) / 3.0;
    const double normalized = clamp01(result / 0.8);
    const double heatValue = 1.0 - normalized;

    QVariantMap metrics;
    metrics.insert("calcSource", "region_k_omkrf");
    metrics.insert("legacyScoreRaw", result);
    metrics.insert("legacyScoreNormalized", normalized);
    metrics.insert("legacyClass", regionLegacyClass(result));
    metrics.insert("heatValue", heatValue);
    metrics.insert("heatClass", scoreToHeatClass(heatValue));
    metrics.insert("kSnf", kSnf);
    metrics.insert("kKf", kKf);
    metrics.insert("kRf", kRf);
    metrics.insert("pSnf", pSnf);
    metrics.insert("pKf", pKf);
    metrics.insert("pRf", pRf);
    return metrics;
}

QVariantMap LegacyCalculationService::mpoMetrics(
    int mpoId,
    const QSqlDatabase &db,
    QSet<QString> &guard)
{
    QSqlQuery query(db);
    query.prepare(
        "SELECT id_ls, id_groups, semantika_digit1, semantika_digit2 "
        "FROM mpo_pso WHERE id_mpo_pso = :id_mpo_pso LIMIT 1");
    query.bindValue(":id_mpo_pso", mpoId);
    if (!query.exec() || !query.next()) {
        return neutralMetrics("mpo_linked", "mpo object not found");
    }

    const int idLs = query.value("id_ls").toInt();
    const int idGroups = query.value("id_groups").toInt();
    const double rangeKm = query.value("semantika_digit1").toDouble();
    const double directionDeg = query.value("semantika_digit2").toDouble();

    QVariantMap parentMetrics = neutralMetrics("mpo_linked_parent_missing");
    QString parentSource = "none";
    if (idLs > 0) {
        parentMetrics = objectMetricsInternal(ObjectTypeMapper::FORMATIONS, idLs, db, guard);
        parentSource = "ls";
    } else if (idGroups > 0) {
        parentMetrics = objectMetricsInternal(ObjectTypeMapper::GROUPS, idGroups, db, guard);
        parentSource = "groups";
    }

    const double parentHeat = clamp01(parentMetrics.value("heatValue").toDouble());
    const double influenceHeat = clamp01(rangeKm / 250.0);
    const double heatValue = clamp01(parentHeat * 0.75 + influenceHeat * 0.25);
    const double normalized = 1.0 - heatValue;

    QVariantMap metrics = parentMetrics;
    metrics.insert("calcSource", "mpo_linked");
    metrics.insert("legacyScoreRaw", normalized);
    metrics.insert("legacyScoreNormalized", normalized);
    metrics.insert("heatValue", heatValue);
    metrics.insert("heatClass", scoreToHeatClass(heatValue));
    metrics.insert("legacyClass", scoreToHeatClass(heatValue));
    metrics.insert("mpoParentSource", parentSource);
    metrics.insert("mpoParentLsId", idLs);
    metrics.insert("mpoParentGroupsId", idGroups);
    metrics.insert("coverageRangeKm", rangeKm);
    metrics.insert("coverageDirectionDeg", directionDeg);
    return metrics;
}

QVariantMap LegacyCalculationService::groupMetrics(int groupId, const QSqlDatabase &db)
{
    QSqlQuery query(db);
    query.prepare("SELECT id_region FROM groups WHERE id_groups = :id_groups LIMIT 1");
    query.bindValue(":id_groups", groupId);
    if (!query.exec() || !query.next()) {
        return neutralMetrics("group_region_k_omkrf", "group not found");
    }

    const int regionId = query.value("id_region").toInt();
    if (regionId <= 0) {
        return neutralMetrics("group_region_k_omkrf", "group without region link");
    }

    QVariantMap metrics = regionMetrics(regionId, db);
    metrics.insert("calcSource", "group_region_k_omkrf");
    metrics.insert("parentRegionId", regionId);
    return metrics;
}

QVariantMap LegacyCalculationService::personnelMetrics(
    int personesId,
    const QSqlDatabase &db,
    QSet<QString> &guard)
{
    QSqlQuery query(db);
    query.prepare(
        "SELECT id_ls, id_groups FROM persones WHERE id_persones = :id_persones LIMIT 1");
    query.bindValue(":id_persones", personesId);
    if (!query.exec() || !query.next()) {
        return neutralMetrics("personnel_linked", "personnel not found");
    }

    const int idLs = query.value("id_ls").toInt();
    const int idGroups = query.value("id_groups").toInt();

    if (idLs > 0) {
        QVariantMap metrics = objectMetricsInternal(ObjectTypeMapper::FORMATIONS, idLs, db, guard);
        metrics.insert("calcSource", "personnel_ls_mps");
        metrics.insert("parentLsId", idLs);
        return metrics;
    }
    if (idGroups > 0) {
        QVariantMap metrics = objectMetricsInternal(ObjectTypeMapper::GROUPS, idGroups, db, guard);
        metrics.insert("calcSource", "personnel_group_region");
        metrics.insert("parentGroupId", idGroups);
        return metrics;
    }

    return neutralMetrics("personnel_linked", "personnel has no linked formation/group");
}

QVariantMap LegacyCalculationService::specialConditionMetrics(
    int specialConditionId,
    const QSqlDatabase &db)
{
    QSqlQuery query(db);
    query.prepare(
        "SELECT id_region FROM special_conditions "
        "WHERE id_special_conditions = :id_special_conditions LIMIT 1");
    query.bindValue(":id_special_conditions", specialConditionId);
    if (!query.exec() || !query.next()) {
        return neutralMetrics("special_condition_region", "special condition not found");
    }

    const int regionId = query.value("id_region").toInt();
    if (regionId <= 0) {
        return neutralMetrics("special_condition_region", "special condition without region link");
    }

    QVariantMap metrics = regionMetrics(regionId, db);
    metrics.insert("calcSource", "special_condition_region");
    metrics.insert("parentRegionId", regionId);
    return metrics;
}

QVariantMap LegacyCalculationService::eventMetrics(
    int eventId,
    const QSqlDatabase &db,
    QSet<QString> &guard)
{
    QSqlQuery query(db);
    query.prepare(
        "SELECT eo.id_object, teo.table_name, m.id_ls, m.id_smi, m.id_groups "
        "FROM event_objects eo "
        "LEFT JOIN type_event_object teo ON teo.id_type_event_object = eo.id_type_event_object "
        "LEFT JOIN mpo_pso m ON teo.table_name = 'mpo_pso' AND m.id_mpo_pso = eo.id_object "
        "WHERE eo.id_event = :id_event");
    query.bindValue(":id_event", eventId);
    if (!query.exec()) {
        return neutralMetrics("event_linked", query.lastError().text());
    }

    int linkedCount = 0;
    double heatAccumulator = 0.0;
    while (query.next()) {
        const int objectId = query.value("id_object").toInt();
        const int objectType = tableNameToObjectType(
            query.value("table_name").toString(),
            query.value("id_ls").toInt(),
            query.value("id_smi").toInt(),
            query.value("id_groups").toInt());
        if (objectId <= 0 || objectType <= 0 || objectType == ObjectTypeMapper::EVENTS) {
            continue;
        }
        const QVariantMap linkedMetrics = objectMetricsInternal(objectType, objectId, db, guard);
        heatAccumulator += clamp01(linkedMetrics.value("heatValue").toDouble());
        ++linkedCount;
    }

    if (linkedCount == 0) {
        return neutralMetrics("event_linked", "event has no linked calculable objects");
    }

    const double heatValue = clamp01(heatAccumulator / static_cast<double>(linkedCount));
    const double normalized = 1.0 - heatValue;

    QVariantMap metrics;
    metrics.insert("calcSource", "event_linked");
    metrics.insert("legacyScoreRaw", normalized);
    metrics.insert("legacyScoreNormalized", normalized);
    metrics.insert("legacyClass", scoreToHeatClass(heatValue));
    metrics.insert("heatValue", heatValue);
    metrics.insert("heatClass", scoreToHeatClass(heatValue));
    metrics.insert("linkedObjectsCount", linkedCount);
    return metrics;
}

QVariantMap LegacyCalculationService::neutralMetrics(const QString &source, const QString &reason)
{
    QVariantMap metrics;
    metrics.insert("calcSource", source);
    metrics.insert("legacyScoreRaw", 0.5);
    metrics.insert("legacyScoreNormalized", 0.5);
    metrics.insert("legacyClass", 0);
    metrics.insert("heatValue", 0.5);
    metrics.insert("heatClass", scoreToHeatClass(0.5));
    if (!reason.isEmpty()) {
        metrics.insert("calcReason", reason);
    }
    return metrics;
}

int LegacyCalculationService::tableNameToObjectType(
    const QString &tableName,
    int mpoLsId,
    int mpoSmiId,
    int mpoGroupsId)
{
    return ObjectTypeMapper::objectTypeByTableName(tableName, mpoLsId, mpoSmiId, mpoGroupsId);
}

double LegacyCalculationService::clamp01(double value)
{
    if (qIsNaN(value)) {
        return 0.0;
    }
    if (value < 0.0) {
        return 0.0;
    }
    if (value > 1.0) {
        return 1.0;
    }
    return value;
}

int LegacyCalculationService::scoreToHeatClass(double heatValue)
{
    const double value = clamp01(heatValue);
    if (value < 0.25) {
        return 1;
    }
    if (value < 0.5) {
        return 2;
    }
    if (value < 0.75) {
        return 3;
    }
    return 4;
}

int LegacyCalculationService::mpsLegacyClass(double mpsValue)
{
    if (mpsValue > 0.0 && mpsValue < 0.3) {
        return 1;
    }
    if (mpsValue >= 0.3 && mpsValue < 0.6) {
        return 2;
    }
    if (mpsValue >= 0.6 && mpsValue <= 1.0) {
        return 3;
    }
    if (qFuzzyIsNull(mpsValue)) {
        return 0;
    }
    return -1;
}

int LegacyCalculationService::regionLegacyClass(double regionValue)
{
    if (regionValue > 0.0 && regionValue < 0.3) {
        return 1;
    }
    if (regionValue >= 0.3 && regionValue < 0.5) {
        return 2;
    }
    if (regionValue >= 0.5 && regionValue <= 0.8) {
        return 3;
    }
    if (qFuzzyIsNull(regionValue)) {
        return 0;
    }
    return -1;
}

QHash<int, QVariantMap> LegacyCalculationService::metricsForType(
    int objectType,
    const QList<int> &ids,
    const QSqlDatabase &db)
{
    BatchCache cache;
    return metricsForTypeInternal(objectType, ids, db, cache);
}

QHash<int, QVariantMap> LegacyCalculationService::metricsForTypeInternal(
    int objectType,
    const QList<int> &ids,
    const QSqlDatabase &db,
    BatchCache &cache)
{
    QHash<int, QVariantMap> result;
    QList<int> pending;
    for (int id : ids) {
        const auto key = qMakePair(objectType, id);
        if (cache.contains(key)) {
            result.insert(id, cache.value(key));
        } else if (id > 0) {
            pending.append(id);
        }
    }
    if (pending.isEmpty()) {
        return result;
    }

    QHash<int, QVariantMap> fresh;
    switch (objectType) {
    case ObjectTypeMapper::FORMATIONS:
        fresh = formationMetricsBatch(pending, db);
        break;
    case ObjectTypeMapper::REGIONS:
        fresh = regionMetricsBatch(pending, db);
        break;
    case ObjectTypeMapper::GROUPS:
        fresh = groupMetricsBatch(pending, db, cache);
        break;
    case ObjectTypeMapper::SPECIAL_CONDITIONS:
        fresh = specialConditionMetricsBatch(pending, db, cache);
        break;
    case ObjectTypeMapper::SMI_MEANS:
    case ObjectTypeMapper::FORMATIONS_MEANS:
    case ObjectTypeMapper::GROUPS_MEANS:
        fresh = mpoMetricsBatch(pending, db, cache);
        break;
    case ObjectTypeMapper::PERSONNEL:
        fresh = personnelMetricsBatch(pending, db, cache);
        break;
    case ObjectTypeMapper::EVENTS:
        fresh = eventMetricsBatch(pending, db, cache);
        break;
    default:
        for (int id : pending) {
            fresh.insert(id, neutralMetrics("unsupported_object_type"));
        }
        break;
    }

    for (auto it = fresh.begin(); it != fresh.end(); ++it) {
        QVariantMap &metrics = it.value();
        metrics.insert("objectType", objectType);
        metrics.insert("objectId", it.key());
        cache.insert(qMakePair(objectType, it.key()), metrics);
        result.insert(it.key(), metrics);
    }
    return result;
}

QHash<int, QVariantMap> LegacyCalculationService::formationMetricsBatch(
    const QList<int> &ids,
    const QSqlDatabase &db)
{
    QHash<int, QVariantMap> out;
    if (ids.isEmpty()) {
        return out;
    }

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "SELECT id_ls, mps_priz_ls, mps_konrt_ls, mps_of_ls, counte_ls, enimy_ls "
        "FROM ls WHERE id_ls IN (%1)").arg(buildInPlaceholders(ids.size())));
    for (int id : ids) {
        query.addBindValue(id);
    }
    if (!query.exec()) {
        for (int id : ids) {
            out.insert(id, neutralMetrics("formation_mps", query.lastError().text()));
        }
        return out;
    }

    const double baseMin[3] = {0.05, 0.10, 0.20};
    const double baseMax[3] = {0.05, 0.15, 0.30};
    const double share[4] = {0.80, 0.20 * 0.65, 0.20 * 0.10, 0.20 * 0.25};

    while (query.next()) {
        const int lsId = query.value("id_ls").toInt();
        const double mpsPriz = query.value("mps_priz_ls").toDouble();
        const double mpsKontr = query.value("mps_konrt_ls").toDouble();
        const double mpsOfficer = query.value("mps_of_ls").toDouble();
        const int peopleCount = query.value("counte_ls").toInt();
        const bool enemyFlag = query.value("enimy_ls").toBool();

        const double mpsValue = (mpsPriz + mpsKontr + mpsOfficer) / 3.0;
        const double normalized = clamp01(mpsValue);
        const double heatValue = 1.0 - normalized;

        QVariantList lossMinMatrix;
        QVariantList lossMaxMatrix;
        for (int severity = 0; severity < 3; ++severity) {
            QVariantList minRow;
            QVariantList maxRow;
            for (int category = 0; category < 4; ++category) {
                const int minValue = static_cast<int>(static_cast<double>(peopleCount) * baseMin[severity] * share[category]);
                const int maxValue = static_cast<int>(static_cast<double>(peopleCount) * baseMax[severity] * share[category]);
                minRow.append(minValue);
                maxRow.append(maxValue);
            }
            lossMinMatrix.append(minRow);
            lossMaxMatrix.append(maxRow);
        }

        QVariantMap metrics;
        metrics.insert("calcSource", "formation_mps");
        metrics.insert("legacyScoreRaw", mpsValue);
        metrics.insert("legacyScoreNormalized", normalized);
        metrics.insert("legacyClass", mpsLegacyClass(mpsValue));
        metrics.insert("heatValue", heatValue);
        metrics.insert("heatClass", scoreToHeatClass(heatValue));
        metrics.insert("mpsPriz", mpsPriz);
        metrics.insert("mpsKontr", mpsKontr);
        metrics.insert("mpsOfficer", mpsOfficer);
        metrics.insert("enemy", enemyFlag);
        metrics.insert("peopleCount", peopleCount);
        metrics.insert("lossesMin", lossMinMatrix);
        metrics.insert("lossesMax", lossMaxMatrix);
        metrics.insert("lossesOperationalBandMin", lossMinMatrix.value(1));
        metrics.insert("lossesOperationalBandMax", lossMaxMatrix.value(1));
        out.insert(lsId, metrics);
    }

    for (int id : ids) {
        if (!out.contains(id)) {
            out.insert(id, neutralMetrics("formation_mps", "formation not found"));
        }
    }
    return out;
}

QHash<int, QVariantMap> LegacyCalculationService::regionMetricsBatch(
    const QList<int> &ids,
    const QSqlDatabase &db)
{
    QHash<int, QVariantMap> out;
    if (ids.isEmpty()) {
        return out;
    }

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "SELECT * FROM region WHERE id_region IN (%1)").arg(buildInPlaceholders(ids.size())));
    for (int id : ids) {
        query.addBindValue(id);
    }
    if (!query.exec()) {
        for (int id : ids) {
            out.insert(id, neutralMetrics("region_k_omkrf", query.lastError().text()));
        }
        return out;
    }

    int idRegionIndex = -1;
    while (query.next()) {
        if (idRegionIndex < 0) {
            idRegionIndex = query.record().indexOf("id_region");
        }
        const int regionId = idRegionIndex >= 0
            ? query.value(idRegionIndex).toInt()
            : query.value("id_region").toInt();

        const QSqlRecord rec = query.record();
        if (rec.count() <= 81) {
            out.insert(regionId, neutralMetrics("region_k_omkrf", "region schema mismatch for legacy formula"));
            continue;
        }

        auto valueAt = [&query](int index) -> double {
            return query.value(index).toDouble();
        };

        double kSnf = 0.0;
        for (int i = 0; i < 17; ++i) {
            kSnf += valueAt(13 + 2 * i) * valueAt(13 + 2 * i + 1);
        }
        kSnf /= 17.0;

        double kKf = 0.0;
        for (int i = 0; i < 10; ++i) {
            kKf += valueAt(47 + 2 * i) * valueAt(47 + 2 * i + 1);
        }
        kKf /= 10.0;

        double kRf = 0.0;
        for (int i = 0; i < 6; ++i) {
            kRf += valueAt(67 + 2 * i) * valueAt(67 + 2 * i + 1);
        }
        kRf /= 6.0;

        const double pSnf = valueAt(79);
        const double pKf = valueAt(80);
        const double pRf = valueAt(81);

        const double resultValue = (kSnf * pSnf + kKf * pKf + kRf * pRf) / 3.0;
        const double normalized = clamp01(resultValue / 0.8);
        const double heatValue = 1.0 - normalized;

        QVariantMap metrics;
        metrics.insert("calcSource", "region_k_omkrf");
        metrics.insert("legacyScoreRaw", resultValue);
        metrics.insert("legacyScoreNormalized", normalized);
        metrics.insert("legacyClass", regionLegacyClass(resultValue));
        metrics.insert("heatValue", heatValue);
        metrics.insert("heatClass", scoreToHeatClass(heatValue));
        metrics.insert("kSnf", kSnf);
        metrics.insert("kKf", kKf);
        metrics.insert("kRf", kRf);
        metrics.insert("pSnf", pSnf);
        metrics.insert("pKf", pKf);
        metrics.insert("pRf", pRf);
        out.insert(regionId, metrics);
    }

    for (int id : ids) {
        if (!out.contains(id)) {
            out.insert(id, neutralMetrics("region_k_omkrf", "region not found"));
        }
    }
    return out;
}

QHash<int, QVariantMap> LegacyCalculationService::groupMetricsBatch(
    const QList<int> &ids,
    const QSqlDatabase &db,
    BatchCache &cache)
{
    QHash<int, QVariantMap> out;
    if (ids.isEmpty()) {
        return out;
    }

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "SELECT id_groups, id_region FROM groups WHERE id_groups IN (%1)")
            .arg(buildInPlaceholders(ids.size())));
    for (int id : ids) {
        query.addBindValue(id);
    }
    if (!query.exec()) {
        for (int id : ids) {
            out.insert(id, neutralMetrics("group_region_k_omkrf", query.lastError().text()));
        }
        return out;
    }

    QHash<int, int> regionIdByGroup;
    QSet<int> regionIds;
    while (query.next()) {
        const int groupId = query.value("id_groups").toInt();
        const int regionId = query.value("id_region").toInt();
        if (regionId <= 0) {
            out.insert(groupId, neutralMetrics("group_region_k_omkrf", "group without region link"));
        } else {
            regionIdByGroup.insert(groupId, regionId);
            regionIds.insert(regionId);
        }
    }

    QHash<int, QVariantMap> regionMetrics;
    if (!regionIds.isEmpty()) {
        regionMetrics = metricsForTypeInternal(
            ObjectTypeMapper::REGIONS,
            QList<int>(regionIds.begin(), regionIds.end()),
            db,
            cache);
    }
    for (auto it = regionIdByGroup.constBegin(); it != regionIdByGroup.constEnd(); ++it) {
        QVariantMap metrics = regionMetrics.value(it.value(), neutralMetrics("group_region_k_omkrf"));
        metrics.insert("calcSource", "group_region_k_omkrf");
        metrics.insert("parentRegionId", it.value());
        out.insert(it.key(), metrics);
    }

    for (int id : ids) {
        if (!out.contains(id)) {
            out.insert(id, neutralMetrics("group_region_k_omkrf", "group not found"));
        }
    }
    return out;
}

QHash<int, QVariantMap> LegacyCalculationService::specialConditionMetricsBatch(
    const QList<int> &ids,
    const QSqlDatabase &db,
    BatchCache &cache)
{
    QHash<int, QVariantMap> out;
    if (ids.isEmpty()) {
        return out;
    }

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "SELECT id_special_conditions, id_region FROM special_conditions "
        "WHERE id_special_conditions IN (%1)").arg(buildInPlaceholders(ids.size())));
    for (int id : ids) {
        query.addBindValue(id);
    }
    if (!query.exec()) {
        for (int id : ids) {
            out.insert(id, neutralMetrics("special_condition_region", query.lastError().text()));
        }
        return out;
    }

    QHash<int, int> regionIdByObject;
    QSet<int> regionIds;
    while (query.next()) {
        const int objectId = query.value("id_special_conditions").toInt();
        const int regionId = query.value("id_region").toInt();
        if (regionId <= 0) {
            out.insert(objectId, neutralMetrics("special_condition_region", "special condition without region link"));
        } else {
            regionIdByObject.insert(objectId, regionId);
            regionIds.insert(regionId);
        }
    }

    QHash<int, QVariantMap> regionMetrics;
    if (!regionIds.isEmpty()) {
        regionMetrics = metricsForTypeInternal(
            ObjectTypeMapper::REGIONS,
            QList<int>(regionIds.begin(), regionIds.end()),
            db,
            cache);
    }
    for (auto it = regionIdByObject.constBegin(); it != regionIdByObject.constEnd(); ++it) {
        QVariantMap metrics = regionMetrics.value(it.value(), neutralMetrics("special_condition_region"));
        metrics.insert("calcSource", "special_condition_region");
        metrics.insert("parentRegionId", it.value());
        out.insert(it.key(), metrics);
    }

    for (int id : ids) {
        if (!out.contains(id)) {
            out.insert(id, neutralMetrics("special_condition_region", "special condition not found"));
        }
    }
    return out;
}

QHash<int, QVariantMap> LegacyCalculationService::mpoMetricsBatch(
    const QList<int> &ids,
    const QSqlDatabase &db,
    BatchCache &cache)
{
    QHash<int, QVariantMap> out;
    if (ids.isEmpty()) {
        return out;
    }

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "SELECT id_mpo_pso, id_ls, id_groups, semantika_digit1, semantika_digit2 "
        "FROM mpo_pso WHERE id_mpo_pso IN (%1)").arg(buildInPlaceholders(ids.size())));
    for (int id : ids) {
        query.addBindValue(id);
    }
    if (!query.exec()) {
        for (int id : ids) {
            out.insert(id, neutralMetrics("mpo_linked", query.lastError().text()));
        }
        return out;
    }

    struct MpoRow
    {
        int idLs = 0;
        int idGroups = 0;
        double rangeKm = 0.0;
        double directionDeg = 0.0;
    };
    QHash<int, MpoRow> rowsByMpo;
    QSet<int> lsIds;
    QSet<int> groupIds;
    while (query.next()) {
        MpoRow row;
        const int mpoId = query.value("id_mpo_pso").toInt();
        row.idLs = query.value("id_ls").toInt();
        row.idGroups = query.value("id_groups").toInt();
        row.rangeKm = query.value("semantika_digit1").toDouble();
        row.directionDeg = query.value("semantika_digit2").toDouble();
        rowsByMpo.insert(mpoId, row);
        if (row.idLs > 0) {
            lsIds.insert(row.idLs);
        } else if (row.idGroups > 0) {
            groupIds.insert(row.idGroups);
        }
    }

    QHash<int, QVariantMap> lsMetricsHash;
    if (!lsIds.isEmpty()) {
        lsMetricsHash = metricsForTypeInternal(
            ObjectTypeMapper::FORMATIONS,
            QList<int>(lsIds.begin(), lsIds.end()),
            db,
            cache);
    }
    QHash<int, QVariantMap> groupMetricsHash;
    if (!groupIds.isEmpty()) {
        groupMetricsHash = metricsForTypeInternal(
            ObjectTypeMapper::GROUPS,
            QList<int>(groupIds.begin(), groupIds.end()),
            db,
            cache);
    }

    for (auto it = rowsByMpo.constBegin(); it != rowsByMpo.constEnd(); ++it) {
        const int mpoId = it.key();
        const MpoRow &row = it.value();

        QVariantMap parentMetrics = neutralMetrics("mpo_linked_parent_missing");
        QString parentSource = QStringLiteral("none");
        if (row.idLs > 0) {
            parentMetrics = lsMetricsHash.value(row.idLs, neutralMetrics("mpo_linked_parent_missing"));
            parentSource = QStringLiteral("ls");
        } else if (row.idGroups > 0) {
            parentMetrics = groupMetricsHash.value(row.idGroups, neutralMetrics("mpo_linked_parent_missing"));
            parentSource = QStringLiteral("groups");
        }

        const double parentHeat = clamp01(parentMetrics.value("heatValue").toDouble());
        const double influenceHeat = clamp01(row.rangeKm / 250.0);
        const double heatValue = clamp01(parentHeat * 0.75 + influenceHeat * 0.25);
        const double normalized = 1.0 - heatValue;

        QVariantMap metrics = parentMetrics;
        metrics.insert("calcSource", "mpo_linked");
        metrics.insert("legacyScoreRaw", normalized);
        metrics.insert("legacyScoreNormalized", normalized);
        metrics.insert("heatValue", heatValue);
        metrics.insert("heatClass", scoreToHeatClass(heatValue));
        metrics.insert("legacyClass", scoreToHeatClass(heatValue));
        metrics.insert("mpoParentSource", parentSource);
        metrics.insert("mpoParentLsId", row.idLs);
        metrics.insert("mpoParentGroupsId", row.idGroups);
        metrics.insert("coverageRangeKm", row.rangeKm);
        metrics.insert("coverageDirectionDeg", row.directionDeg);
        out.insert(mpoId, metrics);
    }

    for (int id : ids) {
        if (!out.contains(id)) {
            out.insert(id, neutralMetrics("mpo_linked", "mpo object not found"));
        }
    }
    return out;
}

QHash<int, QVariantMap> LegacyCalculationService::personnelMetricsBatch(
    const QList<int> &ids,
    const QSqlDatabase &db,
    BatchCache &cache)
{
    QHash<int, QVariantMap> out;
    if (ids.isEmpty()) {
        return out;
    }

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "SELECT id_persones, id_ls, id_groups FROM persones WHERE id_persones IN (%1)")
            .arg(buildInPlaceholders(ids.size())));
    for (int id : ids) {
        query.addBindValue(id);
    }
    if (!query.exec()) {
        for (int id : ids) {
            out.insert(id, neutralMetrics("personnel_linked", query.lastError().text()));
        }
        return out;
    }

    struct PersonRow
    {
        int idLs = 0;
        int idGroups = 0;
    };
    QHash<int, PersonRow> rowsByPerson;
    QSet<int> lsIds;
    QSet<int> groupIds;
    while (query.next()) {
        PersonRow row;
        const int personId = query.value("id_persones").toInt();
        row.idLs = query.value("id_ls").toInt();
        row.idGroups = query.value("id_groups").toInt();
        rowsByPerson.insert(personId, row);
        if (row.idLs > 0) {
            lsIds.insert(row.idLs);
        } else if (row.idGroups > 0) {
            groupIds.insert(row.idGroups);
        }
    }

    QHash<int, QVariantMap> lsMetricsHash;
    if (!lsIds.isEmpty()) {
        lsMetricsHash = metricsForTypeInternal(
            ObjectTypeMapper::FORMATIONS,
            QList<int>(lsIds.begin(), lsIds.end()),
            db,
            cache);
    }
    QHash<int, QVariantMap> groupMetricsHash;
    if (!groupIds.isEmpty()) {
        groupMetricsHash = metricsForTypeInternal(
            ObjectTypeMapper::GROUPS,
            QList<int>(groupIds.begin(), groupIds.end()),
            db,
            cache);
    }

    for (auto it = rowsByPerson.constBegin(); it != rowsByPerson.constEnd(); ++it) {
        const int personId = it.key();
        const PersonRow &row = it.value();
        if (row.idLs > 0) {
            QVariantMap metrics = lsMetricsHash.value(
                row.idLs,
                neutralMetrics("personnel_linked", "personnel has no linked formation/group"));
            metrics.insert("calcSource", "personnel_ls_mps");
            metrics.insert("parentLsId", row.idLs);
            out.insert(personId, metrics);
        } else if (row.idGroups > 0) {
            QVariantMap metrics = groupMetricsHash.value(
                row.idGroups,
                neutralMetrics("personnel_linked", "personnel has no linked formation/group"));
            metrics.insert("calcSource", "personnel_group_region");
            metrics.insert("parentGroupId", row.idGroups);
            out.insert(personId, metrics);
        } else {
            out.insert(personId, neutralMetrics("personnel_linked", "personnel has no linked formation/group"));
        }
    }

    for (int id : ids) {
        if (!out.contains(id)) {
            out.insert(id, neutralMetrics("personnel_linked", "personnel not found"));
        }
    }
    return out;
}

QHash<int, QVariantMap> LegacyCalculationService::eventMetricsBatch(
    const QList<int> &ids,
    const QSqlDatabase &db,
    BatchCache &cache)
{
    QHash<int, QVariantMap> out;
    if (ids.isEmpty()) {
        return out;
    }

    QSqlQuery query(db);
    query.prepare(QStringLiteral(
        "SELECT eo.id_event, eo.id_object, teo.table_name, m.id_ls, m.id_smi, m.id_groups "
        "FROM event_objects eo "
        "LEFT JOIN type_event_object teo ON teo.id_type_event_object = eo.id_type_event_object "
        "LEFT JOIN mpo_pso m ON teo.table_name = 'mpo_pso' AND m.id_mpo_pso = eo.id_object "
        "WHERE eo.id_event IN (%1)").arg(buildInPlaceholders(ids.size())));
    for (int id : ids) {
        query.addBindValue(id);
    }
    if (!query.exec()) {
        for (int id : ids) {
            out.insert(id, neutralMetrics("event_linked", query.lastError().text()));
        }
        return out;
    }

    QHash<int, QList<QPair<int, int>>> linkedByEvent;
    QHash<int, QSet<int>> idsByType;
    while (query.next()) {
        const int eventId = query.value("id_event").toInt();
        const int objectId = query.value("id_object").toInt();
        const int linkedType = tableNameToObjectType(
            query.value("table_name").toString(),
            query.value("id_ls").toInt(),
            query.value("id_smi").toInt(),
            query.value("id_groups").toInt());
        if (objectId <= 0 || linkedType <= 0 || linkedType == ObjectTypeMapper::EVENTS) {
            continue;
        }
        linkedByEvent[eventId].append(qMakePair(linkedType, objectId));
        idsByType[linkedType].insert(objectId);
    }

    QHash<int, QHash<int, QVariantMap>> metricsByType;
    for (auto it = idsByType.constBegin(); it != idsByType.constEnd(); ++it) {
        const int linkedType = it.key();
        const QSet<int> &set = it.value();
        metricsByType.insert(
            linkedType,
            metricsForTypeInternal(
                linkedType,
                QList<int>(set.begin(), set.end()),
                db,
                cache));
    }

    for (int eventId : ids) {
        const QList<QPair<int, int>> &links = linkedByEvent.value(eventId);
        if (links.isEmpty()) {
            out.insert(eventId, neutralMetrics("event_linked", "event has no linked calculable objects"));
            continue;
        }

        double heatAccumulator = 0.0;
        int linkedCount = 0;
        for (const auto &pair : links) {
            const QVariantMap linkedMetrics = metricsByType.value(pair.first).value(pair.second);
            if (linkedMetrics.isEmpty()) {
                continue;
            }
            heatAccumulator += clamp01(linkedMetrics.value("heatValue").toDouble());
            ++linkedCount;
        }

        if (linkedCount == 0) {
            out.insert(eventId, neutralMetrics("event_linked", "event has no linked calculable objects"));
            continue;
        }

        const double heatValue = clamp01(heatAccumulator / static_cast<double>(linkedCount));
        const double normalized = 1.0 - heatValue;

        QVariantMap metrics;
        metrics.insert("calcSource", "event_linked");
        metrics.insert("legacyScoreRaw", normalized);
        metrics.insert("legacyScoreNormalized", normalized);
        metrics.insert("legacyClass", scoreToHeatClass(heatValue));
        metrics.insert("heatValue", heatValue);
        metrics.insert("heatClass", scoreToHeatClass(heatValue));
        metrics.insert("linkedObjectsCount", linkedCount);
        out.insert(eventId, metrics);
    }

    return out;
}
