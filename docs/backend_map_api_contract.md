# Backend Map API Contract (QML Singletons)

This document describes the backend contract exposed through `Saturn.Backend 1.0`.

## Stable Services

### `Database` (`DataAccess`)
- `connectToDatabase() -> bool`
- `disconnectDatabase() -> bool`
- Properties: `connected`, `lastError`, `driver`, `host`, `port`, `dbName`, `user`, `password`

### `Auth` (`AuthManager`)
- `login(loginName, password) -> bool`
- `logout()`
- Permissions: `canRead()`, `canCreate()`, `canEditGeometry()`, `canDelete()`
- Properties: `loggedIn`, `currentUserId`, `currentUserGroup`, `currentUserDisplayName`, `lastError`

### `DbSettings` (`DbConnectionSettings`)
- `load()`
- `save()`
- `applyAndConnect() -> bool`
- Properties: `driver`, `host`, `port`, `dbName`, `user`, `password`

### `GeometryRepo` (`GeometryRepository`)
- `loadObjectGeometry(objectType, objectId) -> QVariantList`
- `saveObjectGeometry(objectType, objectId, geometryRole, geometryType, coordinateIds, isClosed) -> bool`

Row format from `loadObjectGeometry`:
- `geometryRole: string`
- `geometryType: "Point" | "LineString" | "Polygon"`
- `pointOrder: int`
- `coordinateId: int`
- `longitude: double`
- `latitude: double`

### `GeoJson` (`GeoJsonService`)
- `buildFeatureCollection(objectType, objectId) -> string (GeoJSON FeatureCollection)`

### `MapObjectsRepo` (`MapObjectsRepository`)
- `listObjectsByType(objectType, limit) -> QVariantList`
- `buildObjectGeoJson(objectType, objectId) -> string`
- `buildTypeGeoJson(objectType, limit) -> string`

### `ObjectDetailsRepo` (`ObjectDetailsRepository`)
- `objectSummary(objectType, objectId) -> QVariantMap`
- `objectDetails(objectType, objectId) -> QVariantMap`

### `EventsRepo` (`EventsRepository`)
- `listEvents(nameFilter, limit, offset) -> QVariantList`
- `eventDetails(eventId) -> QVariantMap`

### `PersonnelRepo` (`PersonnelRepository`)
- `listPersonnel(nameFilter, limit, offset) -> QVariantList`
- `personnelDetails(personesId) -> QVariantMap`

### `Polling` (`PollingService`)
- `start()`, `stop()`, `triggerNow()`
- Properties: `running`, `intervalMs`
- Signal: `tick()`

### `MapSnapshot` (`MapSnapshotService`)
- `refreshAll() -> string (FeatureCollection)`
- `refreshByDelta(limit) -> QVariantList (changed objectType list)`
- `refreshType(objectType, limit) -> string`
- `featureCollectionForType(objectType) -> string`
- `featureCollectionAll() -> string`
- `snapshotMeta() -> QVariantList`
- Properties: `lastUpdatedIso`, `pollingBound`
- Delta strategy: preferred source is object_geometry_changes; fallback is MAX(updated_at)+COUNT(*) per object type.

### `MapRuntime` (`MapRuntimeService`)
- `refreshNow()`
- `sourceForGeometry("points"|"lines"|"polygons") -> string`
- `updateMeta() -> QVariantList`
- Properties: `pointsSource`, `linesSource`, `polygonsSource`, `lastRuntimeUpdateIso`

### `MapEditing` (`MapEditingService`)
- `replaceGeometry(objectType, objectId, geometryRole, geometryType, points, isClosed) -> bool`
- `replaceGeometryByCoordinateIds(objectType, objectId, geometryRole, geometryType, coordinateIds, isClosed) -> bool`
- `replaceGeometryBundle(objectType, objectId, geometries, replaceAllRoles=false) -> bool`
- `replaceObjectsGeometryBundles(objects, replaceAllRoles=false) -> bool`
- `deleteGeometry(objectType, objectId, geometryRole) -> bool`
- `validateGeometry(geometryType, points, isClosed) -> bool`
- `listGeometryRoles(objectType, objectId) -> QVariantList`
- `getGeometry(objectType, objectId, geometryRole) -> QVariantList`
- Property: `lastError`
- Signal: `geometryChanged(objectType, objectId, geometryRole)`
- Signal: `geometryBundleChanged(objectType, objectId)`
- Signal: `geometriesBatchChanged()`
- Security: write operations require active session (Auth.loggedIn) and corresponding permission checks (canEditGeometry / canDelete).

Role/type guards in editing:
- `position` requires `Point`
- `route` requires `LineString`
- `coverage` requires `Polygon`
- Longitude range: `[-180, 180]`, latitude range: `[-90, 90]`

## Notes
- `SMI` (`objectType = 10`) is treated as a non-map entity and is excluded from map runtime snapshots.
- Geometry fallback is supported via legacy `coord_*` tables when `object_geometry` has no records.

## TEMPORARY / TEST Markers
- `mainform/main.qml`: technical backend validation shell only.
- `mainform/main.qml`: diagnostic text panels (`snapshotInfo`, `runtimeInfo`, `editingInfo`).
- `mainform/main.qml`: button "Тест записи геометрии" for smoke validation only.
- `mainform/main.cpp`: comment marking temporary shell entrypoint.

