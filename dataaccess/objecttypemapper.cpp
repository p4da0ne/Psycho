#include "objecttypemapper.h"

namespace ObjectTypeMapper {

int inferMpoType(int idLs, int idSmi, int idGroups)
{
    // Priority keeps one deterministic type for mpo_pso records.
    if (idGroups > 0) {
        return GROUPS_MEANS;
    }
    if (idLs > 0) {
        return FORMATIONS_MEANS;
    }
    if (idSmi > 0) {
        return SMI_MEANS;
    }
    return SMI_MEANS;
}

int objectTypeByTableName(const QString &tableName, int mpoLsId, int mpoSmiId, int mpoGroupsId)
{
    const QString key = tableName.trimmed().toLower();
    if (key == QStringLiteral("ls")) {
        return FORMATIONS;
    }
    if (key == QStringLiteral("special_conditions")) {
        return SPECIAL_CONDITIONS;
    }
    if (key == QStringLiteral("mpo_pso")) {
        return inferMpoType(mpoLsId, mpoSmiId, mpoGroupsId);
    }
    if (key == QStringLiteral("region")) {
        return REGIONS;
    }
    if (key == QStringLiteral("persones")) {
        return PERSONNEL;
    }
    if (key == QStringLiteral("events")) {
        return EVENTS;
    }
    if (key == QStringLiteral("groups")) {
        return GROUPS;
    }
    if (key == QStringLiteral("smi")) {
        return SMI;
    }
    return 0;
}

QString typeLabel(int objectType)
{
    switch (objectType) {
    case FORMATIONS: return QStringLiteral("Формирования");
    case SPECIAL_CONDITIONS: return QStringLiteral("Особые условия");
    case SMI_MEANS: return QStringLiteral("Средства СМИ");
    case FORMATIONS_MEANS: return QStringLiteral("Средства формирований");
    case GROUPS_MEANS: return QStringLiteral("Средства групп");
    case REGIONS: return QStringLiteral("Регионы");
    case PERSONNEL: return QStringLiteral("Персоналии");
    case EVENTS: return QStringLiteral("События");
    case GROUPS: return QStringLiteral("Группы");
    case SMI: return QStringLiteral("СМИ");
    default: return QStringLiteral("Объекты");
    }
}

} // namespace ObjectTypeMapper

