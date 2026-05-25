#ifndef OBJECTTYPEMAPPER_H
#define OBJECTTYPEMAPPER_H

#include <QString>

#include "config_dataaccess.h"

namespace ObjectTypeMapper {

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

int inferMpoType(int idLs, int idSmi, int idGroups);
int objectTypeByTableName(const QString &tableName, int mpoLsId = 0, int mpoSmiId = 0, int mpoGroupsId = 0);
QString typeLabel(int objectType);

} // namespace ObjectTypeMapper

#endif // OBJECTTYPEMAPPER_H

