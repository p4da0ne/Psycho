#include "legacycalculationservice.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QtMath>

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
    return objectMetricsInternal(objectType, objectId, guard);
}

QVariantMap LegacyCalculationService::objectMetricsInternal(int objectType, int objectId, QSet<QString> &guard)
{
    if (objectType <= 0 || objectId <= 0) {
        return neutralMetrics("invalid", "objectType/objectId must be positive");
    }

    const QString guardKey = QString::number(objectType) + ":" + QString::number(objectId);
    if (guard.contains(guardKey)) {
        return neutralMetrics("cycle_guard", "recursive object dependency detected");
    }
    guard.insert(guardKey);

    DataAccess *db = DataAccess::instance();
    if (!db->connected() && !db->connectToDatabase()) {
        guard.remove(guardKey);
        return neutralMetrics("db_unavailable", db->lastError());
    }

    QVariantMap metrics;
    switch (objectType) {
    case FORMATIONS:
        metrics = formationMetrics(objectId);
        break;
    case REGIONS:
        metrics = regionMetrics(objectId);
        break;
    case SMI_MEANS:
    case FORMATIONS_MEANS:
    case GROUPS_MEANS:
        metrics = mpoMetrics(objectId, guard);
        break;
    case GROUPS:
        metrics = groupMetrics(objectId);
        break;
    case PERSONNEL:
        metrics = personnelMetrics(objectId, guard);
        break;
    case SPECIAL_CONDITIONS:
        metrics = specialConditionMetrics(objectId);
        break;
    case EVENTS:
        metrics = eventMetrics(objectId, guard);
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

QVariantMap LegacyCalculationService::formationMetrics(int lsId)
{
    QSqlQuery query;
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

QVariantMap LegacyCalculationService::regionMetrics(int regionId)
{
    QSqlQuery query;
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

QVariantMap LegacyCalculationService::mpoMetrics(int mpoId, QSet<QString> &guard)
{
    QSqlQuery query;
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
        parentMetrics = objectMetricsInternal(FORMATIONS, idLs, guard);
        parentSource = "ls";
    } else if (idGroups > 0) {
        parentMetrics = objectMetricsInternal(GROUPS, idGroups, guard);
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

QVariantMap LegacyCalculationService::groupMetrics(int groupId)
{
    QSqlQuery query;
    query.prepare("SELECT id_region FROM groups WHERE id_groups = :id_groups LIMIT 1");
    query.bindValue(":id_groups", groupId);
    if (!query.exec() || !query.next()) {
        return neutralMetrics("group_region_k_omkrf", "group not found");
    }

    const int regionId = query.value("id_region").toInt();
    if (regionId <= 0) {
        return neutralMetrics("group_region_k_omkrf", "group without region link");
    }

    QVariantMap metrics = regionMetrics(regionId);
    metrics.insert("calcSource", "group_region_k_omkrf");
    metrics.insert("parentRegionId", regionId);
    return metrics;
}

QVariantMap LegacyCalculationService::personnelMetrics(int personesId, QSet<QString> &guard)
{
    QSqlQuery query;
    query.prepare(
        "SELECT id_ls, id_groups FROM persones WHERE id_persones = :id_persones LIMIT 1");
    query.bindValue(":id_persones", personesId);
    if (!query.exec() || !query.next()) {
        return neutralMetrics("personnel_linked", "personnel not found");
    }

    const int idLs = query.value("id_ls").toInt();
    const int idGroups = query.value("id_groups").toInt();

    if (idLs > 0) {
        QVariantMap metrics = objectMetricsInternal(FORMATIONS, idLs, guard);
        metrics.insert("calcSource", "personnel_ls_mps");
        metrics.insert("parentLsId", idLs);
        return metrics;
    }
    if (idGroups > 0) {
        QVariantMap metrics = objectMetricsInternal(GROUPS, idGroups, guard);
        metrics.insert("calcSource", "personnel_group_region");
        metrics.insert("parentGroupId", idGroups);
        return metrics;
    }

    return neutralMetrics("personnel_linked", "personnel has no linked formation/group");
}

QVariantMap LegacyCalculationService::specialConditionMetrics(int specialConditionId)
{
    QSqlQuery query;
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

    QVariantMap metrics = regionMetrics(regionId);
    metrics.insert("calcSource", "special_condition_region");
    metrics.insert("parentRegionId", regionId);
    return metrics;
}

QVariantMap LegacyCalculationService::eventMetrics(int eventId, QSet<QString> &guard)
{
    QSqlQuery query;
    query.prepare(
        "SELECT eo.id_object, teo.table_name "
        "FROM event_objects eo "
        "LEFT JOIN type_event_object teo ON teo.id_type_event_object = eo.id_type_event_object "
        "WHERE eo.id_event = :id_event");
    query.bindValue(":id_event", eventId);
    if (!query.exec()) {
        return neutralMetrics("event_linked", query.lastError().text());
    }

    int linkedCount = 0;
    double heatAccumulator = 0.0;
    while (query.next()) {
        const int objectId = query.value("id_object").toInt();
        const int objectType = tableNameToObjectType(query.value("table_name").toString());
        if (objectId <= 0 || objectType <= 0 || objectType == EVENTS) {
            continue;
        }
        const QVariantMap linkedMetrics = objectMetricsInternal(objectType, objectId, guard);
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

int LegacyCalculationService::tableNameToObjectType(const QString &tableName)
{
    const QString key = tableName.trimmed().toLower();
    if (key == "ls") {
        return FORMATIONS;
    }
    if (key == "special_conditions") {
        return SPECIAL_CONDITIONS;
    }
    if (key == "mpo_pso") {
        return SMI_MEANS;
    }
    if (key == "region") {
        return REGIONS;
    }
    if (key == "persones") {
        return PERSONNEL;
    }
    if (key == "events") {
        return EVENTS;
    }
    if (key == "groups") {
        return GROUPS;
    }
    return 0;
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
