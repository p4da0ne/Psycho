pragma ComponentBehavior: Bound
import QtQuick 2.15
import QtQuick.Window 2.15
import Qt5Compat.GraphicalEffects
import Qt.labs.settings 1.0
import Saturn.Backend 1.0
import "qrc:/components"

Window {
    id: root

    visible: true
    width: 1540
    height: 920
    minimumWidth: 1180
    minimumHeight: 760
    title: "Saturn GIS"
    color: "#0B0F14"

    property string panel: "none"
    property var mapObjects: []
    property var mapLines: []
    property var mapPolygons: []
    property var locationLabels: []
    // Перечень для правой панели: точки + по одному представителю на каждый
    // линейный/полигональный объект (центроид), чтобы в списке были все объекты карты.
    readonly property var mapObjectListModel: root.buildMapObjectListModel(mapObjects, mapLines, mapPolygons)
    property var mapEvents: []
    property var referenceTreeData: []
    property bool pendingDbBootstrap: false
    property bool connectedBootstrapScheduled: false
    property bool connectedBootstrapRunning: false
    property bool connectedBootstrapRerunRequested: false
    property bool connectedBootstrapMapFullScheduled: false

    QtObject {
        id: appState

        property real centerLon: 37.6176
        property real centerLat: 55.7558
        property real cursorLon: 37.6176
        property real cursorLat: 55.7558
        property real zoomLevel: 11.8
        property real viewNorth: centerLat
        property real viewSouth: centerLat
        property real viewEast: centerLon
        property real viewWest: centerLon
        property real mapMetersPerPixel: 0
        property real mapScaleDenominator: 0
        // Локальный tileserver-gl, поднимается через tools/start_tileserver.ps1.
        property string tileServerHost: "http://localhost:8080"
        property string mapStyleName: "maptiler-basic"
        readonly property string mapStyleUrl: (tileServerHost && tileServerHost.length > 0 && mapStyleName && mapStyleName.length > 0)
            ? tileServerHost + "/styles/" + mapStyleName + "/style.json"
            : "https://demotiles.maplibre.org/style.json"
        property string mapMode: "point"
        property string activeTool: "navigate"
        property string searchText: ""
        property bool showFriendly: true
        property bool showForeign: true
        property bool showBaseStations: true
        property bool showCoverageLine: true
        property bool showLabels: true
        property bool controlsVisible: true
        property bool inspectorVisible: true
        property bool navigationVisible: false
        property int activePanel: 0
        property int activeInspectorTab: 0
        property string selectedGroup: ""
        property string selectedStructurePath: ""
        property string eventStatusFilter: "all"
        property string eventTypeFilter: "all"
        property int eventTypeFilterId: 0
        property var eventTypeCatalog: []
        property int quickFilterObjectType: 0
        property string quickFilterOwnership: "all"
        property string quickFilterRegionScope: "all"
        property string quickFilterEventFrom: ""
        property string quickFilterEventTo: ""
        property bool geometryEditActive: false
        property string geometryEditRole: ""
        property string geometryEditType: ""
        property int geometryEditObjectType: 0
        property int geometryEditObjectId: 0
        property string geometryEditObjectName: ""
        property bool geometryEditClosed: false
        property bool geometryEditDirty: false
        property var geometryEditPoints: []
        property bool geometryEditAllActive: false
        property var geometryEditBundle: ({})
        property var selectedObject: null
        property var selectedObjectDetails: ({})
        property var selectedEvent: null
        property var hoveredObject: null
        property string selectionType: "none"
        property var selectedObjects: []
        property real lastContextLon: centerLon
        property real lastContextLat: centerLat
        property string statusMessage: "Готово."

        function selectObject(objectData) {
            selectedObject = objectData
            hoveredObject = objectData
            selectionType = objectData ? "object" : "none"
            selectedObjects = objectData ? [objectData] : []
            if (objectData) {
                selectedEvent = null
                root.loadObjectDetailsForObject(objectData)
            } else {
                selectedObjectDetails = ({})
            }
            statusMessage = objectData ? "Выбран объект: " + objectData.name : "Выделение снято"
        }

        function clearSelection() {
            selectedObject = null
            selectedObjectDetails = ({})
            hoveredObject = null
            selectionType = "none"
            selectedObjects = []
            statusMessage = "Выделение снято"
        }

        function showObjectInfo(objectData) {
            root.showObjectInfoForObject(objectData || selectedObject)
        }

        function beginGeometryEdit(roleName) {
            root.startGeometryEditForSelected(roleName)
        }

        function beginGeometryEditAll() {
            root.startAllGeometryEditForSelected()
        }

        function setGeometryEditRole(roleName) {
            root.switchGeometryEditRole(roleName)
        }

        function commitGeometryEdit() {
            root.commitGeometryEdit()
        }

        function cancelGeometryEdit() {
            root.cancelGeometryEdit()
        }

        onGeometryEditPointsChanged: {
            if (!geometryEditAllActive || !geometryEditRole)
                return
            var nextBundle = Object.assign({}, geometryEditBundle || {})
            var entry = Object.assign({}, nextBundle[geometryEditRole] || {})
            entry.points = geometryEditPoints ? geometryEditPoints.slice(0) : []
            nextBundle[geometryEditRole] = entry
            geometryEditBundle = nextBundle
        }
    }

    Settings {
        category: "map"
        property alias tileServerHost: appState.tileServerHost
        property alias mapStyleName: appState.mapStyleName
    }

    QtObject {
        id: panelManager
        property var panels: ({
            "reference-panel": {
                "id": "reference-panel",
                "visible": false,
                "collapsed": false,
                "detached": false,
                "x": 16,
                "y": 120,
                "width": 300,
                "height": 620
            },
            "right-sidebar": {
                "id": "right-sidebar",
                "visible": true,
                "collapsed": false,
                "detached": false,
                "x": 16,
                "y": 120,
                "width": 372,
                "height": 620
            },
            "status-bar": {
                "id": "status-bar",
                "visible": true,
                "collapsed": false,
                "detached": false,
                "x": 16,
                "y": root.height - 40,
                "width": root.width - 32,
                "height": 24
            }
        })
        property string activePanelId: "right-sidebar"
        property string draggingPanelId: ""
        property var layoutBounds: ({ "x": 0, "y": 0, "width": root.width, "height": root.height })

        function panelById(panelId) {
            var source = panels || {}
            return source[panelId] || {
                "id": panelId,
                "visible": true,
                "collapsed": false,
                "detached": false,
                "x": 16,
                "y": 120,
                "width": 320,
                "height": 320
            }
        }

        function setPanelState(panelId, patch) {
            var source = panels || {}
            var current = panelById(panelId)
            var next = {
                "id": panelId,
                "visible": patch.visible !== undefined ? patch.visible : current.visible,
                "collapsed": patch.collapsed !== undefined ? patch.collapsed : current.collapsed,
                "detached": patch.detached !== undefined ? patch.detached : current.detached,
                "x": patch.x !== undefined ? patch.x : current.x,
                "y": patch.y !== undefined ? patch.y : current.y,
                "width": patch.width !== undefined ? patch.width : current.width,
                "height": patch.height !== undefined ? patch.height : current.height
            }
            var clone = Object.assign({}, source)
            clone[panelId] = next
            panels = clone
        }

        function setPanelVisible(panelId, visibleValue) {
            setPanelState(panelId, { "visible": !!visibleValue })
            if (panelId === "right-sidebar")
                appState.inspectorVisible = !!visibleValue
            if (panelId === "reference-panel")
                appState.navigationVisible = !!visibleValue
        }

        function toggleCollapsed(panelId) {
            var current = panelById(panelId)
            setPanelState(panelId, { "collapsed": !current.collapsed, "visible": true })
        }

        function resizePanel(panelId, widthValue, heightValue) {
            var patch = {}
            if (widthValue !== undefined)
                patch.width = Math.max(120, widthValue)
            if (heightValue !== undefined)
                patch.height = Math.max(24, heightValue)
            setPanelState(panelId, patch)
        }

        function beginDrag(panelId) {
            draggingPanelId = panelId
            var current = panelById(panelId)
            setPanelState(panelId, { "detached": true, "visible": true, "x": current.x, "y": current.y })
        }

        function updateDrag(panelId, xValue, yValue) {
            var targetId = panelId || draggingPanelId
            if (!targetId)
                return
            setPanelState(targetId, { "detached": true, "x": Math.max(0, xValue), "y": Math.max(0, yValue) })
        }

        function endDrag(panelId) {
            var targetId = panelId || draggingPanelId
            if (!targetId)
                return
            draggingPanelId = ""
        }

        function setActivePanel(panelId) {
            activePanelId = panelId
            appState.activePanel = panelId === "right-sidebar" ? 1 : panelId === "reference-panel" ? 2 : 0
        }
    }

    Agents {
        id: agentHub
        appState: appState
        panelManager: panelManager
        host: root
    }

    property var stateManager: appState
    property bool leftDockedVisible: false
    property bool rightDockedVisible: false
    property bool inspectorDockedLeftVisible: root.rightSidebarState
        && root.rightSidebarState.visible === true
        && root.rightSidebarState.detached !== true
    property var referencePanelState: {
        if (!root.panelManager)
            return ({})
        root.panelManager.panels
        return root.panelManager.panelById("reference-panel")
    }
    property var rightSidebarState: {
        if (!root.panelManager)
            return ({})
        root.panelManager.panels
        return root.panelManager.panelById("right-sidebar")
    }
    property var statusBarState: {
        if (!root.panelManager)
            return ({})
        root.panelManager.panels
        return root.panelManager.panelById("status-bar")
    }

    function visibleObjectCount() {
        return root.agentHub.filterAgent.visibleObjectCount(root.mapObjects)
    }

    function averageMpps() {
        return root.agentHub.filterAgent.averageMpps(root.mapObjects)
    }

    function sideByType(objectType) {
        if (objectType === 3 || objectType === 4 || objectType === 5 || objectType === 6 || objectType === 9)
            return "infrastructure"
        if (objectType === 8)
            return "foreign"
        return "friendly"
    }

    function kindByType(objectType) {
        if (objectType === 6)
            return "region"
        if (objectType === 7)
            return "personnel"
        if (objectType === 8)
            return "event"
        if (objectType === 3 || objectType === 4 || objectType === 5)
            return "lbs"
        return "unit"
    }

    function structurePathByType(objectType) {
        if (objectType === 6 || objectType === 7 || objectType === 8)
            return "countries/russia/moscow"
        return "countries/russia"
    }

    function typeLabel(objectType) {
        switch (objectType) {
        case 1: return "Формирования"
        case 2: return "Особые условия"
        case 3: return "Средства СМИ"
        case 4: return "Средства формирований"
        case 5: return "Средства групп"
        case 6: return "Регионы"
        case 7: return "Персоналии"
        case 8: return "События"
        case 9: return "Группы"
        case 10: return "СМИ"
        default: return "Объекты"
        }
    }

    function eventStatusValue(statusName, startDate, endDate) {
        var normalized = String(statusName || "").toLowerCase()
        if (normalized.indexOf("отмен") !== -1 || normalized.indexOf("cancel") !== -1)
            return "cancelled"
        if (normalized.indexOf("план") !== -1 || normalized.indexOf("plan") !== -1)
            return "planned"
        if (normalized.indexOf("заверш") !== -1 || normalized.indexOf("past") !== -1)
            return "past"

        var nowMs = (new Date()).getTime()
        var startMs = startDate ? (new Date(startDate)).getTime() : 0
        var endMs = endDate ? (new Date(endDate)).getTime() : 0
        if (startMs > 0 && startMs > nowMs)
            return "planned"
        if (endMs > 0 && endMs < nowMs)
            return "past"
        return "actual"
    }

    function eventStatusLabel(statusKey) {
        if (statusKey === "actual")
            return "Актуальные"
        if (statusKey === "planned")
            return "Плановые"
        if (statusKey === "past")
            return "Завершенные"
        if (statusKey === "cancelled")
            return "Отмененные"
        return "События"
    }

    function eventTypeValue(typeName) {
        var normalized = String(typeName || "").toLowerCase()
        if (normalized.indexOf("связ") !== -1 || normalized.indexOf("comm") !== -1)
            return "communications"
        if (normalized.indexOf("логист") !== -1 || normalized.indexOf("logist") !== -1)
            return "logistics"
        if (normalized.indexOf("инфра") !== -1 || normalized.indexOf("infra") !== -1)
            return "infrastructure"
        if (normalized.indexOf("опера") !== -1 || normalized.indexOf("oper") !== -1)
            return "operations"
        return "monitoring"
    }

    function objectTypeByTableName(sourceObject) {
        var tableName = sourceObject
        var mpoLsId = 0
        var mpoSmiId = 0
        var mpoGroupsId = 0

        if (typeof sourceObject === "object" && sourceObject !== null) {
            tableName = sourceObject.tableName
            mpoLsId = Number(sourceObject.idLs || 0)
            mpoSmiId = Number(sourceObject.idSmi || 0)
            mpoGroupsId = Number(sourceObject.idGroups || 0)
        }

        var key = String(tableName || "").toLowerCase()
        if (key === "ls")
            return 1
        if (key === "special_conditions")
            return 2
        if (key === "mpo_pso") {
            if (mpoGroupsId > 0)
                return 5
            if (mpoLsId > 0)
                return 4
            if (mpoSmiId > 0)
                return 3
            return 3
        }
        if (key === "region")
            return 6
        if (key === "persones")
            return 7
        if (key === "events")
            return 8
        if (key === "groups")
            return 9
        if (key === "smi")
            return 10
        return 0
    }

    function findRuntimeObject(objectType, objectId) {
        var targetType = Number(objectType)
        var targetId = Number(objectId)
        var items = mapObjects || []
        for (var i = 0; i < items.length; ++i) {
            if (Number(items[i].objectType) === targetType && Number(items[i].objectId) === targetId)
                return items[i]
        }
        return null
    }

    function loadObjectDetailsForObject(objectData) {
        if (!objectData || objectData.objectType === undefined || objectData.objectId === undefined) {
            appState.selectedObjectDetails = ({})
            return
        }
        var details = ObjectDetailsRepo.objectDetails(
                    Number(objectData.objectType),
                    Number(objectData.objectId))
        appState.selectedObjectDetails = details || ({})
    }

    function showObjectInfoForObject(objectData) {
        if (!objectData || objectData.objectType === undefined || objectData.objectId === undefined) {
            appState.statusMessage = "Детали недоступны: объект не выбран"
            return
        }
        appState.selectObject(objectData)
        if (root.agentHub && root.agentHub.uiStateAgent) {
            root.agentHub.uiStateAgent.setInspectorVisible(true)
            root.agentHub.uiStateAgent.setInspectorTab(0)
            root.agentHub.uiStateAgent.setActivePanel(1)
        }
        var details = appState.selectedObjectDetails || {}
        var payload = details.fullRow || details.payload || {}
        var count = Object.keys(payload).length
        appState.statusMessage = "Открыты детали из БД: " + (objectData.name || "") + " (" + count + " полей)"
    }

    function coordinatePairToPoint(pair) {
        if (!pair || pair.length < 2)
            return null
        return {
            "lon": Number(pair[0]),
            "lat": Number(pair[1])
        }
    }

    function geometryTypeByRole(roleName) {
        if (roleName === "position")
            return "Point"
        if (roleName === "route")
            return "LineString"
        if (roleName === "coverage")
            return "Polygon"
        return ""
    }

    function roleClosedByName(roleName) {
        return roleName === "coverage"
    }

    function geometryPointsForRole(objectType, objectId, roleName) {
        var raw = MapGeometryRoles.geometry(objectType, objectId, roleName)
        var points = []
        for (var i = 0; i < raw.length; ++i) {
            var row = raw[i]
            points.push({
                "longitude": Number(row.longitude),
                "latitude": Number(row.latitude)
            })
        }
        return points
    }

    function switchGeometryEditRole(roleName) {
        if (!appState.geometryEditActive) {
            appState.statusMessage = "Режим редактирования не активен"
            return
        }
        var geometryType = geometryTypeByRole(roleName)
        if (geometryType === "") {
            appState.statusMessage = "Неизвестная роль геометрии: " + roleName
            return
        }
        if (appState.geometryEditAllActive) {
            var bundle = appState.geometryEditBundle || {}
            var bundleEntry = bundle[roleName]
            if (!bundleEntry) {
                bundleEntry = {
                    "geometryRole": roleName,
                    "geometryType": geometryType,
                    "isClosed": roleClosedByName(roleName),
                    "points": []
                }
                var updatedBundle = Object.assign({}, bundle)
                updatedBundle[roleName] = bundleEntry
                appState.geometryEditBundle = updatedBundle
            }
            appState.geometryEditRole = roleName
            appState.geometryEditType = geometryType
            appState.geometryEditClosed = !!bundleEntry.isClosed
            appState.geometryEditPoints = (bundleEntry.points || []).slice(0)
            appState.geometryEditDirty = false
            appState.statusMessage = "Режим редактирования: " + roleName + " (" + appState.geometryEditObjectName + ")"
            return
        }
        startGeometryEditForSelected(roleName)
    }

    function startGeometryEditForSelected(roleName) {
        if (!appState.selectedObject) {
            appState.statusMessage = "Не выбран объект для редактирования геометрии"
            return
        }
        if (!Auth.loggedIn || !Auth.canEditGeometry()) {
            appState.statusMessage = "Требуется авторизация с правом редактирования геометрии"
            return
        }
        var geometryType = geometryTypeByRole(roleName)
        if (geometryType === "") {
            appState.statusMessage = "Неизвестная роль геометрии: " + roleName
            return
        }
        var points = geometryPointsForRole(
                    appState.selectedObject.objectType,
                    appState.selectedObject.objectId,
                    roleName)

        appState.geometryEditActive = true
        appState.geometryEditAllActive = false
        appState.geometryEditBundle = ({})
        appState.geometryEditRole = roleName
        appState.geometryEditType = geometryType
        appState.geometryEditObjectType = appState.selectedObject.objectType
        appState.geometryEditObjectId = appState.selectedObject.objectId
        appState.geometryEditObjectName = appState.selectedObject.name
        appState.geometryEditClosed = roleClosedByName(roleName)
        appState.geometryEditPoints = points
        appState.geometryEditDirty = false
        appState.statusMessage = "Режим редактирования: " + roleName + " (" + appState.selectedObject.name + ")"
    }

    function startAllGeometryEditForSelected() {
        if (!appState.selectedObject) {
            appState.statusMessage = "Не выбран объект для редактирования геометрии"
            return
        }
        if (!Auth.loggedIn || !Auth.canEditGeometry()) {
            appState.statusMessage = "Требуется авторизация с правом редактирования геометрии"
            return
        }

        var objectType = appState.selectedObject.objectType
        var objectId = appState.selectedObject.objectId
        var roles = ["position", "route", "coverage"]
        var bundle = {}
        for (var i = 0; i < roles.length; ++i) {
            var roleName = roles[i]
            var geometryType = geometryTypeByRole(roleName)
            bundle[roleName] = {
                "geometryRole": roleName,
                "geometryType": geometryType,
                "isClosed": roleClosedByName(roleName),
                "points": geometryPointsForRole(objectType, objectId, roleName)
            }
        }

        appState.geometryEditActive = true
        appState.geometryEditAllActive = true
        appState.geometryEditBundle = bundle
        appState.geometryEditObjectType = objectType
        appState.geometryEditObjectId = objectId
        appState.geometryEditObjectName = appState.selectedObject.name
        appState.geometryEditDirty = false
        switchGeometryEditRole("position")
    }

    function commitGeometryEdit() {
        if (!appState.geometryEditActive) {
            appState.statusMessage = "Режим редактирования не активен"
            return
        }

        if (appState.geometryEditAllActive) {
            var roles = ["position", "route", "coverage"]
            var payload = []
            var bundle = appState.geometryEditBundle || {}
            for (var i = 0; i < roles.length; ++i) {
                var roleName = roles[i]
                var entry = bundle[roleName]
                if (!entry)
                    continue
                var points = entry.points || []
                if (roleName === "position" && points.length >= 1) {
                    payload.push({
                        "geometryRole": roleName,
                        "geometryType": "Point",
                        "points": [points[0]],
                        "isClosed": false
                    })
                } else if (roleName === "route" && points.length >= 2) {
                    payload.push({
                        "geometryRole": roleName,
                        "geometryType": "LineString",
                        "points": points,
                        "isClosed": false
                    })
                } else if (roleName === "coverage" && points.length >= 3) {
                    payload.push({
                        "geometryRole": roleName,
                        "geometryType": "Polygon",
                        "points": points,
                        "isClosed": true
                    })
                }
            }
            var bundleOk = MapGeometryRoles.applyBundle(
                        appState.geometryEditObjectType,
                        appState.geometryEditObjectId,
                        payload,
                        true)
            if (bundleOk) {
                appState.geometryEditActive = false
                appState.geometryEditAllActive = false
                appState.geometryEditRole = ""
                appState.geometryEditType = ""
                appState.geometryEditObjectType = 0
                appState.geometryEditObjectId = 0
                appState.geometryEditObjectName = ""
                appState.geometryEditClosed = false
                appState.geometryEditDirty = false
                appState.geometryEditPoints = []
                appState.geometryEditBundle = ({})
                appState.statusMessage = "Геометрии сохранены: position/route/coverage"
                MapRuntime.refreshNow()
                refreshObjectsFromRuntime()
                return
            }
            appState.statusMessage = "Ошибка пакетного сохранения геометрии: " + MapGeometryRoles.lastError
            return
        }

        var points = appState.geometryEditPoints || []
        var ok = false
        if (appState.geometryEditRole === "position") {
            if (points.length === 0) {
                appState.statusMessage = "Для position нужна 1 точка"
                return
            }
            ok = MapGeometryRoles.upsertPosition(
                        appState.geometryEditObjectType,
                        appState.geometryEditObjectId,
                        {
                            "longitude": Number(points[0].longitude),
                            "latitude": Number(points[0].latitude)
                        })
        } else if (appState.geometryEditRole === "route") {
            ok = MapGeometryRoles.upsertRoute(
                        appState.geometryEditObjectType,
                        appState.geometryEditObjectId,
                        points)
        } else if (appState.geometryEditRole === "coverage") {
            ok = MapGeometryRoles.upsertCoverage(
                        appState.geometryEditObjectType,
                        appState.geometryEditObjectId,
                        points,
                        true)
        }

        if (ok) {
            appState.statusMessage = "Геометрия сохранена: " + appState.geometryEditRole
            appState.geometryEditActive = false
            appState.geometryEditDirty = false
            appState.geometryEditPoints = []
            MapRuntime.refreshNow()
            refreshObjectsFromRuntime()
            return
        }

        appState.statusMessage = "Ошибка сохранения геометрии: " + MapGeometryRoles.lastError
    }

    function cancelGeometryEdit() {
        appState.geometryEditActive = false
        appState.geometryEditAllActive = false
        appState.geometryEditRole = ""
        appState.geometryEditType = ""
        appState.geometryEditObjectType = 0
        appState.geometryEditObjectId = 0
        appState.geometryEditObjectName = ""
        appState.geometryEditClosed = false
        appState.geometryEditDirty = false
        appState.geometryEditPoints = []
        appState.geometryEditBundle = ({})
        appState.statusMessage = "Редактирование геометрии отменено"
    }

    function refreshReferenceTreeFromDb(objectsLimitArg, personnelLimitArg, eventsLimitArg) {
        if (!Database.connected)
            return
        var objectsLimit = objectsLimitArg !== undefined ? Number(objectsLimitArg) : 1500
        var personnelLimit = personnelLimitArg !== undefined ? Number(personnelLimitArg) : 1500
        var eventsLimit = eventsLimitArg !== undefined ? Number(eventsLimitArg) : 1500
        var roots = UiDataRepo.buildReferenceTree(objectsLimit, personnelLimit, eventsLimit)
        referenceTreeData = roots || []
        if (agentHub.structureAgent)
            agentHub.structureAgent.referenceTree = referenceTreeData
    }
    function refreshEventsFromDb(limitArg) {
        if (!Database.connected)
            return
        var limit = limitArg !== undefined ? Number(limitArg) : 500
        mapEvents = UiDataRepo.listMapEvents(limit) || []
    }
    function refreshEventTypeCatalogFromDb() {
        if (!Database.connected)
            return
        appState.eventTypeCatalog = EventsRepo.eventTypeCatalog() || []
    }
    function pathCentroid(path) {
        var list = path || []
        var sumLat = 0
        var sumLon = 0
        var count = 0
        for (var i = 0; i < list.length; ++i) {
            var p = list[i] || {}
            var la = Number(p.lat)
            var lo = Number(p.lon)
            if (isFinite(la) && isFinite(lo)) {
                sumLat += la
                sumLon += lo
                ++count
            }
        }
        if (count === 0)
            return null
        return { "lat": sumLat / count, "lon": sumLon / count }
    }
    function buildMapObjectListModel(points, lines, polygons) {
        var out = []
        var seen = ({})
        var pushItem = function(item, center) {
            var oid = Number(item.objectId || 0)
            var key = Number(item.objectType || 0) + "-" + oid
            if (oid > 0 && seen[key])
                return
            if (oid > 0)
                seen[key] = true
            if (!center) {
                out.push(item)
                return
            }
            var clone = ({})
            for (var k in item)
                clone[k] = item[k]
            clone.lat = center.lat
            clone.lon = center.lon
            if (clone.mpps === undefined)
                clone.mpps = Math.round(Number(clone.legacyScoreNormalized || 0.5) * 100)
            out.push(clone)
        }
        var pts = points || []
        for (var i = 0; i < pts.length; ++i)
            pushItem(pts[i] || ({}), null)
        var addPathList = function(items) {
            var list = items || []
            for (var j = 0; j < list.length; ++j) {
                var item = list[j] || ({})
                pushItem(item, root.pathCentroid(item.path))
            }
        }
        addPathList(lines)
        addPathList(polygons)
        return out
    }
    function refreshObjectsFromRuntime() {
        mapObjects = MapRuntime.mapObjects || []
        mapLines = MapRuntime.mapLines || []
        mapPolygons = MapRuntime.mapPolygons || []
        locationLabels = MapRuntime.locationLabels || []
        appState.statusMessage = "Объектов: " + mapObjects.length + ", линий: " + mapLines.length + ", полигонов: " + mapPolygons.length
    }
    function syncLayoutBounds() {
        if (!root.panelManager)
            return
        root.panelManager.layoutBounds = {
            "x": 0,
            "y": 0,
            "width": root.width,
            "height": root.height
        }
    }

    function scheduleConnectedBootstrap() {
        if (!Database.connected)
            return
        if (connectedBootstrapRunning) {
            connectedBootstrapRerunRequested = true
            return
        }
        if (connectedBootstrapScheduled)
            return
        connectedBootstrapScheduled = true
        dbBootstrapMapTimer.restart()
    }

    function runConnectedBootstrapMapPhase() {
        connectedBootstrapScheduled = false
        if (!Database.connected)
            return
        connectedBootstrapRunning = true
        appState.statusMessage = "Быстрая загрузка карты..."
        MapRuntime.refreshNowLimited(260)
        connectedBootstrapMapFullScheduled = true
        dbBootstrapMapFullTimer.restart()
        dbBootstrapMetaTimer.restart()
    }
    function runConnectedBootstrapMetaPhase() {
        if (!Database.connected) {
            connectedBootstrapRunning = false
            return
        }
        refreshEventsFromDb(160)
        refreshEventTypeCatalogFromDb()
        if (root.stateManager && root.stateManager.navigationVisible)
            refreshReferenceTreeFromDb(280, 280, 280)
        connectedBootstrapRunning = false
        if (connectedBootstrapRerunRequested) {
            connectedBootstrapRerunRequested = false
            scheduleConnectedBootstrap()
            return
        }
        pendingDbBootstrap = false
    }
    function runConnectedBootstrapMapFullPhase() {
        connectedBootstrapMapFullScheduled = false
        if (!Database.connected)
            return
        MapRuntime.refreshNow()
    }
    onPanelChanged: {
        if (panel === "none" && pendingDbBootstrap && Database.connected)
            scheduleConnectedBootstrap()
    }

    Component.onCompleted: {
        Polling.intervalMs = 30000
        TileFeed.enabled = true
        if (Database.connected) {
            if (!Polling.running)
                Polling.start()
            if (panel === "none")
                scheduleConnectedBootstrap()
            else
                pendingDbBootstrap = true
            refreshEventTypeCatalogFromDb()
        }
        refreshObjectsFromRuntime()
        syncLayoutBounds()
        if (root.panelManager) {
            root.panelManager.setPanelVisible("right-sidebar", root.stateManager.inspectorVisible)
            root.panelManager.setPanelVisible("reference-panel", root.stateManager.navigationVisible)
        }
    }
    onWidthChanged: syncLayoutBounds()
    onHeightChanged: syncLayoutBounds()

    Connections {
        target: MapRuntime
        function onSourcesUpdated() {
            refreshObjectsFromRuntime()
            TileFeed.publishNow()
        }
    }

    Connections {
        target: Polling
        function onTick() {
            if (!Database.connected)
                return
            refreshEventsFromDb()
            refreshEventTypeCatalogFromDb()
            if (root.stateManager && root.stateManager.navigationVisible)
                refreshReferenceTreeFromDb()
        }
    }

    Connections {
        target: Database
        function onConnectionChanged() {
            if (!Database.connected)
                return
            if (!Polling.running)
                Polling.start()
            if (root.panel === "none")
                scheduleConnectedBootstrap()
            else
                pendingDbBootstrap = true
            refreshEventTypeCatalogFromDb()
        }
    }

    Timer {
        id: dbBootstrapMapTimer
        interval: 140
        repeat: false
        onTriggered: runConnectedBootstrapMapPhase()
    }


    Timer {
        id: dbBootstrapMapFullTimer
        interval: 900
        repeat: false
        onTriggered: runConnectedBootstrapMapFullPhase()
    }
    Timer {
        id: dbBootstrapMetaTimer
        interval: 0
        repeat: false
        onTriggered: runConnectedBootstrapMetaPhase()
    }

    Connections {
        target: root.stateManager

        function onInspectorVisibleChanged() {
            if (root.panelManager)
                root.panelManager.setPanelVisible("right-sidebar", root.stateManager.inspectorVisible)
        }

        function onNavigationVisibleChanged() {
            if (root.panelManager)
                root.panelManager.setPanelVisible("reference-panel", root.stateManager.navigationVisible)
        }

        function onActivePanelChanged() {
            if (!root.panelManager)
                return
            if (root.stateManager.activePanel === 1)
                root.panelManager.setActivePanel("right-sidebar")
            else if (root.stateManager.activePanel === 2)
                root.panelManager.setActivePanel("reference-panel")
        }

        function onSelectedStructurePathChanged() {
            if (root.agentHub.structureAgent && root.stateManager.selectedStructurePath !== root.agentHub.structureAgent.selectedPath)
                root.agentHub.structureAgent.selectedPath = root.stateManager.selectedStructurePath
        }
    }

    Timer {
        id: controlsReturnTimer
        interval: 1100
        repeat: false
        onTriggered: {
            if (root.agentHub && root.agentHub.uiStateAgent)
                root.agentHub.uiStateAgent.setControlsVisible(true)
        }
    }

    Item {
        id: preAuthBackdrop
        anchors.fill: parent
        z: 2
        visible: !Auth.loggedIn

        Rectangle {
            anchors.fill: parent
            color: "#040A15"
        }

        RadialGradient {
            anchors.fill: parent
            horizontalRadius: width * 0.56
            verticalRadius: height * 0.56
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#24497A" }
                GradientStop { position: 0.48; color: "#132746" }
                GradientStop { position: 1.0; color: "#050B17" }
            }
        }
    }

    Item {
        id: dragSurface
        anchors.fill: parent
        z: 10
        visible: true
        enabled: true

        Toolbar {
            id: topBar
            z: 40
            visible: Auth.loggedIn
            width: Math.min(parent.width - 32, 680)
            anchors.top: parent.top
            anchors.topMargin: 18
            anchors.horizontalCenter: parent.horizontalCenter
            appState: root.stateManager
            agentHub: root.agentHub
            panelManager: root.panelManager
            backdropSource: mapCanvas.sceneContent
        }

        RightSidebar {
            id: rightSidebar
            z: 30
            visible: Auth.loggedIn && width > 0.5
            x: root.rightSidebarState && root.rightSidebarState.detached
                ? (root.rightSidebarState.x || 16)
                : 16
            y: root.rightSidebarState && root.rightSidebarState.detached
                ? (root.rightSidebarState.y || (topBar.y + topBar.height + 14))
                : (topBar.y + topBar.height + 14)
            height: root.rightSidebarState && root.rightSidebarState.detached
                ? Math.max(72, root.rightSidebarState.height || 300)
                : (statusBar.visible ? (statusBar.y - 12 - y) : (parent.height - 16 - y))
            appState: root.stateManager
            agentHub: root.agentHub
            objects: root.mapObjectListModel
            events: root.mapEvents
            panelManager: root.panelManager
            panelState: root.rightSidebarState
            backdropSource: mapCanvas.sceneContent
            dragSurface: dragSurface
        }

        BottomStatusBar {
            id: statusBar
            z: 35
            visible: Auth.loggedIn
            anchors.left: parent.left
            anchors.leftMargin: root.inspectorDockedLeftVisible ? rightSidebar.width + 28 : 16
            anchors.right: parent.right
            anchors.rightMargin: 16
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 8
            appState: root.stateManager
            panelManager: root.panelManager
            panelState: root.statusBarState
        }

        MapCanvas {
            id: mapCanvas
            z: 10
            visible: Auth.loggedIn
            anchors.left: parent.left
            anchors.leftMargin: root.inspectorDockedLeftVisible ? rightSidebar.width + 28 : 16
            anchors.right: parent.right
            anchors.rightMargin: 16
            anchors.top: topBar.bottom
            anchors.topMargin: 14
            anchors.bottom: statusBar.visible ? statusBar.top : parent.bottom
            anchors.bottomMargin: statusBar.visible ? 8 : 12
            appState: root.stateManager
            agentHub: root.agentHub
            objects: root.mapObjects
            lines: root.mapLines
            polygons: root.mapPolygons
            labels: root.locationLabels
            onInteractionActivity: {
                if (root.agentHub && root.agentHub.uiStateAgent)
                    root.agentHub.uiStateAgent.setControlsVisible(false)
                controlsReturnTimer.restart()
            }
        }

        GlassPanel {
            id: mainMenu
            z: 60
            width: 272
            height: 170
            visible: !Auth.loggedIn
            anchors.centerIn: parent
            backdropSource: mapCanvas.sceneContent
            surfaceColor: "#121a23"
            surfaceOpacity: 0.56

            Column {
                anchors.fill: parent
                spacing: 10

                Text {
                    text: "Главное меню"
                    color: "#f3f7fb"
                    font.pixelSize: 14
                    font.weight: Font.Medium
                }

                Rectangle {
                    width: parent.width
                    height: 44
                    radius: 12
                    color: "#1e2937"
                    border.color: "#334155"

                    Text {
                        anchors.centerIn: parent
                        text: "Авторизация"
                        color: "#f8fafc"
                        font.pixelSize: 14
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: root.panel = "auth"
                    }
                }

                Rectangle {
                    width: parent.width
                    height: 44
                    radius: 12
                    color: "#1e2937"
                    border.color: "#334155"

                    Text {
                        anchors.centerIn: parent
                        text: "Настройки подключения к БД"
                        color: "#f8fafc"
                        font.pixelSize: 14
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: root.panel = "db"
                    }
                }

                Row {
                    width: parent.width
                    spacing: 8

                    Rectangle {
                        width: 8
                        height: 8
                        radius: 4
                        anchors.verticalCenter: parent.verticalCenter
                        color: Database.connected ? "#8be9a8" : "#fca5a5"
                        border.width: 1
                        border.color: Database.connected ? "#b8f5ca" : "#fecaca"
                    }

                    Text {
                        width: parent.width - 16
                        text: Database.connected
                            ? "\u0421\u0442\u0430\u0442\u0443\u0441 \u0411\u0414: \u043f\u043e\u0434\u043a\u043b\u044e\u0447\u0435\u043d\u043e"
                            : "\u0421\u0442\u0430\u0442\u0443\u0441 \u0411\u0414: \u043d\u0435\u0442 \u043f\u043e\u0434\u043a\u043b\u044e\u0447\u0435\u043d\u0438\u044f"
                        color: Database.connected ? "#8be9a8" : "#fca5a5"
                        font.pixelSize: 12
                        wrapMode: Text.WordWrap
                    }
                }

                Text {
                    width: parent.width
                    visible: !Database.connected && Database.lastError && Database.lastError.length > 0
                    text: Database.lastError
                    color: "#94a3b8"
                    font.pixelSize: 11
                    wrapMode: Text.WordWrap
                    maximumLineCount: 3
                    elide: Text.ElideRight
                }
            }
        }

        Item {
            id: panelHost
            z: 65
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 170
            width: Math.min(parent.width * 0.42, 580)
            height: Math.min(parent.height * 0.48, 420)
            visible: !Auth.loggedIn && root.panel !== "none"

            GlassPanel {
                anchors.fill: parent
                backdropSource: mapCanvas.sceneContent
                surfaceColor: "#10161e"
                surfaceOpacity: 0.62

                Loader {
                    id: panelLoader
                    anchors.fill: parent
                    sourceComponent: root.panel === "auth" ? authPanel : dbPanel
                }
            }

            Rectangle {
                width: 28
                height: 28
                radius: 14
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: 8
                color: "#1f2937"
                border.color: "#334155"

                Text {
                    anchors.centerIn: parent
                    text: "×"
                    color: "#e2e8f0"
                    font.pixelSize: 14
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: root.panel = "none"
                }
            }
        }
    }

    Component {
        id: authPanel
        LoginScreen {
            onLoginSucceeded: root.panel = "none"
        }
    }

    Component {
        id: dbPanel
        DBConnectScreen { }
    }

    Shortcut {
        sequence: "Ctrl+1"
        onActivated: {
            if (root.agentHub && root.agentHub.uiStateAgent)
                root.agentHub.uiStateAgent.setMapMode("point")
        }
    }

    Shortcut {
        sequence: "Ctrl+2"
        onActivated: {
            if (root.agentHub && root.agentHub.uiStateAgent)
                root.agentHub.uiStateAgent.setMapMode("heatmap")
        }
    }

    Shortcut {
        sequence: "Escape"
        onActivated: {
            if (root.panel !== "none") {
                root.panel = "none"
                return
            }
            root.agentHub.selectionAgent.clearSelection()
            root.agentHub.selectionAgent.clearEventSelection()
        }
    }
}
