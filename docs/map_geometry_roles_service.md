# MapGeometryRolesService API

Stable purpose:
- role-oriented geometry API for UI workflows (position, route, coverage).
- wraps low-level MapEditingService calls into simple CRUD operations.

Singleton name in QML:
- MapGeometryRoles

Methods:
- roles(objectType, objectId) -> QVariantList
- geometry(objectType, objectId, geometryRole) -> QVariantList
- geometryBundle(objectType, objectId) -> QVariantMap
- upsertPosition(objectType, objectId, point) -> bool
- upsertRoute(objectType, objectId, points) -> bool
- upsertCoverage(objectType, objectId, points, isClosed=true) -> bool
- removeRole(objectType, objectId, geometryRole) -> bool
- applyBundle(objectType, objectId, geometries, replaceAllRoles=false) -> bool
- applyObjectsBundles(objects, replaceAllRoles=false) -> bool
- replaceRoleGeometry(objectType, objectId, geometryRole, geometryType, points, isClosed=false) -> bool

Property:
- lastError: string

Signals:
- roleChanged(objectType, objectId, geometryRole)
- roleRemoved(objectType, objectId, geometryRole)
- bundleChanged(objectType, objectId)
- bundlesBatchChanged()
- errorChanged()

Expected point payload:
- { longitude: number, latitude: number }

Expected bundle payload (`geometries`):
- [{ geometryRole: string, geometryType: "Point"|"LineString"|"Polygon", points: point[], isClosed?: bool }, ...]

Expected objects payload (`objects`):
- [{ objectType: int, objectId: int, geometries: [...] }, ...]

Bundle mode:
- `replaceAllRoles=false`: upsert only roles from `geometries`
- `replaceAllRoles=true`: replace full role-set for object (roles not present in `geometries` are removed)

Role/type constraints:
- `position` -> `Point`
- `route` -> `LineString`
- `coverage` -> `Polygon`


Security:
- Write calls depend on Auth session and permissions enforced by MapEditingService.

