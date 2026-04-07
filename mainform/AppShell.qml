pragma ComponentBehavior: Bound
import QtQuick 2.15
import QtQuick.Window 2.15
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
    property var mapEvents: []
    property var referenceTreeData: []
    property bool pendingDbBootstrap: false

    QtObject {
        id: appState

        property real centerLon: 37.6176
        property real centerLat: 55.7558
        property real cursorLon: 37.6176
        property real cursorLat: 55.7558
        property real zoomLevel: 11.8
        property string mapMode: "point"
        property string activeTool: "navigate"
        property string searchText: ""
        property bool showFriendly: true
        property bool showForeign: true
        property bool showBaseStations: true
        property bool showHeatmapLayer: true
        property bool showCoverageLine: true
        property bool showLabels: true
        property bool controlsVisible: true
        property bool inspectorVisible: true
        property bool navigationVisible: true
        property int activePanel: 0
        property int activeInspectorTab: 0
        property string selectedGroup: ""
        property string selectedStructurePath: ""
        property string eventStatusFilter: "all"
        property string eventTypeFilter: "all"
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

    QtObject {
        id: panelManager
        property var panels: ({
            "reference-panel": {
                "id": "reference-panel",
                "visible": true,
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
                "x": root.width - 388,
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

    QtObject {
        id: filterAgent

        function objectVisible(objectData) {
            if (!objectData)
                return false
            if (appState.searchText !== "") {
                var needle = appState.searchText.toLowerCase()
                var haystack = (objectData.name + " " + objectData.id + " " + objectData.kind).toLowerCase()
                if (haystack.indexOf(needle) === -1)
                    return false
            }
            if (objectData.side === "friendly" && !appState.showFriendly)
                return false
            if (objectData.side === "foreign" && !appState.showForeign)
                return false
            if (objectData.side === "infrastructure" && !appState.showBaseStations)
                return false
            return true
        }

        function visibleObjects(items) {
            var source = items || []
            var result = []
            for (var i = 0; i < source.length; ++i) {
                if (objectVisible(source[i]))
                    result.push(source[i])
            }
            return result
        }

        function visibleObjectCount(items) {
            return visibleObjects(items).length
        }

        function averageMpps(items) {
            var visible = visibleObjects(items)
            if (visible.length === 0)
                return 0
            var total = 0
            for (var i = 0; i < visible.length; ++i)
                total += Number(visible[i].mpps || 0)
            return Math.round(total / visible.length)
        }

        function visibleEvents(items) {
            var source = items || []
            var result = []
            for (var i = 0; i < source.length; ++i) {
                var row = source[i]
                if (appState.searchText !== "") {
                    var needle = appState.searchText.toLowerCase()
                    var haystack = String((row.name || "") + " " + (row.type || "") + " " + (row.status || "")).toLowerCase()
                    if (haystack.indexOf(needle) === -1)
                        continue
                }
                if (appState.eventStatusFilter !== "all" && String(row.status || "") !== appState.eventStatusFilter)
                    continue
                if (appState.eventTypeFilter !== "all" && String(row.type || "") !== appState.eventTypeFilter)
                    continue
                result.push(row)
            }
            return result
        }
    }

    QtObject {
        id: selectionAgent

        function isSelected(objectData) {
            if (!objectData || !appState.selectedObjects)
                return false
            for (var i = 0; i < appState.selectedObjects.length; ++i) {
                if (appState.selectedObjects[i].id === objectData.id)
                    return true
            }
            return false
        }

        function selectionCount() {
            return appState.selectedObjects ? appState.selectedObjects.length : 0
        }

        function selectObject(objectData) {
            appState.selectObject(objectData)
        }

        function toggleObjectSelection(objectData) {
            if (!objectData)
                return
            var current = appState.selectedObjects ? appState.selectedObjects.slice(0) : []
            var idx = -1
            for (var i = 0; i < current.length; ++i) {
                if (current[i].id === objectData.id) {
                    idx = i
                    break
                }
            }
            if (idx >= 0)
                current.splice(idx, 1)
            else
                current.push(objectData)
            appState.selectedObjects = current
            appState.selectedObject = current.length > 0 ? current[0] : null
            if (appState.selectedObject)
                root.loadObjectDetailsForObject(appState.selectedObject)
            else
                appState.selectedObjectDetails = ({})
            appState.selectionType = current.length > 1 ? "multi" : (current.length === 1 ? "object" : "none")
        }

        function clearSelection() {
            appState.clearSelection()
        }

        function setHoveredObject(objectData) {
            appState.hoveredObject = objectData && objectData.id !== undefined ? objectData : null
        }

        function selectEvent(eventData) {
            if (!eventData) {
                appState.selectedEvent = null
                appState.selectionType = "none"
                return
            }
            appState.selectedObject = null
            appState.selectedObjectDetails = ({})
            var details = EventsRepo.eventDetails(Number(eventData.id || 0))
            var linked = []
            var objects = details && details.objects ? details.objects : []
            for (var i = 0; i < objects.length; ++i) {
                var obj = objects[i]
                var objectType = objectTypeByTableName(obj.tableName)
                if (objectType > 0)
                    linked.push(objectType + "-" + Number(obj.idObject || 0))
            }
            appState.selectedEvent = {
                "id": eventData.id,
                "name": eventData.name,
                "type": eventData.type,
                "status": eventData.status,
                "startTimestamp": eventData.startTimestamp,
                "endTimestamp": eventData.endTimestamp,
                "updatedTimestamp": eventData.updatedTimestamp,
                "description": details && details.description ? String(details.description) : (eventData.description || ""),
                "objectIds": linked
            }
            appState.selectionType = eventData ? "event" : "none"
            appState.activeInspectorTab = 1
        }

        function isEventSelected(eventData) {
            if (!eventData || !appState.selectedEvent)
                return false
            return String(appState.selectedEvent.id) === String(eventData.id)
        }

        function clearEventSelection() {
            appState.selectedEvent = null
            if (appState.selectionType === "event")
                appState.selectionType = "none"
        }
    }

    QtObject {
        id: uiStateAgent

        function setInspectorVisible(value) { panelManager.setPanelVisible("right-sidebar", !!value) }
        function toggleInspectorVisible() { setInspectorVisible(!appState.inspectorVisible) }
        function setNavigationVisible(value) { panelManager.setPanelVisible("reference-panel", !!value) }
        function toggleNavigationVisible() { setNavigationVisible(!appState.navigationVisible) }
        function setControlsVisible(value) { appState.controlsVisible = !!value }
        function setSearchText(textValue) { appState.searchText = textValue || "" }
        function focusSearchResult() { appState.statusMessage = appState.searchText === "" ? "Поиск: строка пуста" : "Поиск: " + appState.searchText }
        function setFilterState(filterKey, enabled) {
            if (filterKey === "friendly")
                appState.showFriendly = !!enabled
            else if (filterKey === "foreign")
                appState.showForeign = !!enabled
            else if (filterKey === "infrastructure")
                appState.showBaseStations = !!enabled
        }
        function setSelectedGroup(groupKey) { appState.selectedGroup = groupKey || "" }
        function clearSelectedGroup() { appState.selectedGroup = "" }
        function setSelectedStructurePath(pathValue) {
            structureAgent.selectedPath = pathValue || ""
            appState.selectedStructurePath = structureAgent.selectedPath
            if (appState.selectedStructurePath === "") {
                appState.selectionType = "none"
                return
            }

            var selectedNode = structureAgent.nodeDetails(appState.selectedStructurePath)
            if (!selectedNode || !selectedNode.nodeKind) {
                appState.selectionType = "structure"
                return
            }

            if (selectedNode.nodeKind === "object") {
                var runtimeObject = root.findRuntimeObject(selectedNode.objectType, selectedNode.objectId)
                if (!runtimeObject) {
                    runtimeObject = {
                        "id": selectedNode.objectType + "-" + selectedNode.objectId,
                        "objectType": Number(selectedNode.objectType),
                        "objectId": Number(selectedNode.objectId),
                        "name": selectedNode.label || ("Object " + selectedNode.objectId),
                        "side": sideByType(Number(selectedNode.objectType)),
                        "kind": kindByType(Number(selectedNode.objectType)),
                        "lat": 0,
                        "lon": 0,
                        "mpps": 0,
                        "speed": 0,
                        "course": 0,
                        "source": "db",
                        "notes": selectedNode.subtitle || "",
                        "structurePath": appState.selectedStructurePath
                    }
                }
                selectionAgent.selectObject(runtimeObject)
                appState.statusMessage = "Выбрана запись БД: " + (runtimeObject.name || "")
                return
            }

            if (selectedNode.nodeKind === "event") {
                selectionAgent.selectEvent({
                    "id": Number(selectedNode.eventId),
                    "name": selectedNode.label || ("Событие " + selectedNode.eventId),
                    "type": selectedNode.eventType || "monitoring",
                    "status": selectedNode.eventStatus || "actual",
                    "startTimestamp": selectedNode.eventStart || "",
                    "endTimestamp": selectedNode.eventEnd || "",
                    "updatedTimestamp": ""
                })
                appState.statusMessage = "Выбрано событие: " + (selectedNode.label || "")
                return
            }

            appState.selectionType = "structure"
        }
        function syncSelectedStructurePath(pathValue) { setSelectedStructurePath(pathValue) }
        function setActivePanel(panelIndex) {
            appState.activePanel = panelIndex
            if (panelIndex === 1)
                panelManager.setActivePanel("right-sidebar")
            else if (panelIndex === 2)
                panelManager.setActivePanel("reference-panel")
            else
                panelManager.setActivePanel("")
        }
        function setInspectorTab(tabIndex) { appState.activeInspectorTab = tabIndex }
        function setMapMode(modeValue) { appState.mapMode = modeValue }
    }

    QtObject {
        id: structureAgent

        property var referenceTree: root.referenceTreeData
        property string selectedPath: ""
        property var selectedNode: ({})
        property bool canDeleteSelected: selectedPath !== ""

        function nodeDetails(pathValue) {
            if (!pathValue || pathValue === "")
                return ({})
            var parts = pathValue.split("/")
            var currentList = referenceTree || []
            var currentNode = null
            for (var i = 0; i < parts.length; ++i) {
                currentNode = null
                for (var j = 0; j < currentList.length; ++j) {
                    if (currentList[j].id === parts[i]) {
                        currentNode = currentList[j]
                        break
                    }
                }
                if (!currentNode)
                    return ({})
                currentList = currentNode.children || []
            }
            return currentNode || ({})
        }

        function renameNode(pathValue, labelValue) {
            if (!pathValue || !labelValue)
                return
            var parts = pathValue.split("/")
            var tree = referenceTree
            var cursor = null
            for (var i = 0; i < parts.length; ++i) {
                var list = (i === 0 && !cursor) ? tree : (cursor.children || [])
                cursor = null
                for (var j = 0; j < list.length; ++j) {
                    if (list[j].id === parts[i]) {
                        cursor = list[j]
                        break
                    }
                }
                if (!cursor)
                    return
            }
            cursor.label = labelValue
            referenceTree = referenceTree
            root.referenceTreeData = referenceTree
            selectedNode = nodeDetails(selectedPath)
        }

        function renameSelectedNode(labelValue) {
            renameNode(selectedPath, labelValue)
        }

        function addChild(pathValue, labelValue) {
            var targetPath = pathValue || selectedPath
            if (!targetPath)
                return
            var target = nodeDetails(targetPath)
            if (!target)
                return
            var children = target.children || []
            children.push({
                "id": "node_" + Date.now(),
                "label": labelValue || "Новый узел",
                "children": []
            })
            target.children = children
            referenceTree = referenceTree
            root.referenceTreeData = referenceTree
            selectedNode = nodeDetails(selectedPath)
        }

        function deleteSelectedNode() {
            if (!selectedPath)
                return
            var parts = selectedPath.split("/")
            if (parts.length === 0)
                return
            if (parts.length === 1) {
                for (var r = 0; r < referenceTree.length; ++r) {
                    if (referenceTree[r].id === parts[0]) {
                        referenceTree.splice(r, 1)
                        break
                    }
                }
            } else {
                var parentPath = parts.slice(0, parts.length - 1).join("/")
                var targetId = parts[parts.length - 1]
                var parentNode = nodeDetails(parentPath)
                if (parentNode && parentNode.children) {
                    for (var c = 0; c < parentNode.children.length; ++c) {
                        if (parentNode.children[c].id === targetId) {
                            parentNode.children.splice(c, 1)
                            break
                        }
                    }
                }
            }
            selectedPath = ""
            appState.selectedStructurePath = ""
            selectedNode = ({})
            referenceTree = referenceTree
            root.referenceTreeData = referenceTree
        }

        onSelectedPathChanged: {
            selectedNode = nodeDetails(selectedPath)
            appState.selectedStructurePath = selectedPath
            canDeleteSelected = selectedPath !== ""
        }
    }

    QtObject {
        id: symbolAgent
        property int revision: 1
        property string selectedTypePath: "units/friendly"
        property int selectedSymbolId: 1
        property var typeTree: [
            {
                "id": "units",
                "label": "Подразделения",
                "children": [
                    { "id": "friendly", "label": "Свои", "children": [] },
                    { "id": "foreign", "label": "Чужие", "children": [] }
                ]
            },
            {
                "id": "relays",
                "label": "Узлы связи",
                "children": [
                    { "id": "friendly", "label": "Свои", "children": [] }
                ]
            },
            {
                "id": "infrastructure",
                "label": "Инфраструктура",
                "children": [
                    { "id": "lbs", "label": "Линейные объекты", "children": [] }
                ]
            }
        ]
        property var symbolsForSelectedType: [
            { "id": 1, "name": "Дружественный узел", "glyph": "●", "fillColor": "#7298f6", "topColor": "#8DB7FF", "innerOpacity": 0.82 },
            { "id": 2, "name": "Иностранный объект", "glyph": "◆", "fillColor": "#d7dee7", "topColor": "#EEF4FB", "innerOpacity": 0.74 },
            { "id": 3, "name": "Инфраструктура", "glyph": "▣", "fillColor": "#9ba8b5", "topColor": "#BAC6D2", "innerOpacity": 0.78 }
        ]
        property var selectedSymbol: symbolsForSelectedType[0]

        function updateSelectedSymbol(nameValue, glyphValue, fillColorValue, topColorValue, innerOpacityValue) {
            var updated = symbolsForSelectedType.slice(0)
            for (var i = 0; i < updated.length; ++i) {
                if (updated[i].id === selectedSymbolId) {
                    updated[i] = {
                        "id": updated[i].id,
                        "name": nameValue,
                        "glyph": glyphValue,
                        "fillColor": fillColorValue,
                        "topColor": topColorValue,
                        "innerOpacity": innerOpacityValue
                    }
                    selectedSymbol = updated[i]
                    break
                }
            }
            symbolsForSelectedType = updated
            revision = revision + 1
        }

        function addSymbol() {
            var nextId = symbolsForSelectedType.length > 0
                ? (symbolsForSelectedType[symbolsForSelectedType.length - 1].id + 1)
                : 1
            var copy = symbolsForSelectedType.slice(0)
            copy.push({
                "id": nextId,
                "name": "Новый символ",
                "glyph": "●",
                "fillColor": "#7298f6",
                "topColor": "#8DB7FF",
                "innerOpacity": 0.82
            })
            symbolsForSelectedType = copy
            selectedSymbolId = nextId
            selectedSymbol = copy[copy.length - 1]
            revision = revision + 1
        }

        function deleteSelectedSymbol() {
            if (symbolsForSelectedType.length <= 1)
                return
            var copy = []
            for (var i = 0; i < symbolsForSelectedType.length; ++i) {
                if (symbolsForSelectedType[i].id !== selectedSymbolId)
                    copy.push(symbolsForSelectedType[i])
            }
            symbolsForSelectedType = copy
            selectedSymbolId = copy[0].id
            selectedSymbol = copy[0]
            revision = revision + 1
        }

        function markerColor(objectData) {
            if (!objectData)
                return "#909ba7"
            if (objectData.side === "friendly")
                return "#7298f6"
            if (objectData.side === "foreign")
                return "#d7dee7"
            return "#909ba7"
        }

        function markerTopColor(objectData) {
            if (!objectData)
                return "#ACB6C2"
            if (objectData.side === "friendly")
                return "#8DB7FF"
            if (objectData.side === "foreign")
                return "#EEF4FB"
            return "#ACB6C2"
        }

        function markerInnerOpacity(objectData) {
            if (objectData && objectData.side === "foreign")
                return 0.72
            return 0.88
        }

        onSelectedSymbolIdChanged: {
            for (var i = 0; i < symbolsForSelectedType.length; ++i) {
                if (symbolsForSelectedType[i].id === selectedSymbolId) {
                    selectedSymbol = symbolsForSelectedType[i]
                    break
                }
            }
        }
    }

    QtObject {
        id: renderAgent
        function shouldRenderHeatmap() { return appState.mapMode === "heatmap" && appState.showHeatmapLayer }
        function shouldRenderCoverageLine() { return appState.showCoverageLine }
        function shouldRenderLabels() { return appState.showLabels }
    }

    QtObject {
        id: agentHub
        property var uiStateAgent: uiStateAgent
        property var filterAgent: filterAgent
        property var selectionAgent: selectionAgent
        property var symbolAgent: symbolAgent
        property var structureAgent: structureAgent
        property var renderAgent: renderAgent
        property var panelManager: panelManager
    }

    property var stateManager: appState
    property bool leftDockedVisible: root.referencePanelState
        && root.referencePanelState.visible === true
        && root.referencePanelState.detached !== true
    property bool rightDockedVisible: root.rightSidebarState
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

    function objectTypeByTableName(tableName) {
        var key = String(tableName || "").toLowerCase()
        if (key === "ls")
            return 1
        if (key === "special_conditions")
            return 2
        if (key === "mpo_pso")
            return 3
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
            appState.statusMessage = "Информация недоступна: объект не выбран"
            return
        }
        appState.selectObject(objectData)
        if (root.agentHub && root.agentHub.uiStateAgent) {
            root.agentHub.uiStateAgent.setInspectorVisible(true)
            root.agentHub.uiStateAgent.setInspectorTab(0)
            root.agentHub.uiStateAgent.setActivePanel(1)
        }
        var details = appState.selectedObjectDetails || {}
        var payload = details.payload || {}
        var count = Object.keys(payload).length
        appState.statusMessage = "Информация загружена: " + (objectData.name || "") + " (" + count + " полей)"
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

    function buildLineEntriesFromFeature(feature) {
        var entries = []
        if (!feature || !feature.geometry)
            return entries

        var geometry = feature.geometry
        var props = feature.properties || {}
        var objectType = Number(props.objectType || 0)
        var objectId = Number(props.objectId || 0)
        var base = {
            "id": objectType + "-" + objectId,
            "objectType": objectType,
            "objectId": objectId,
            "name": props.title || ("Object " + objectId),
            "side": sideByType(objectType),
            "kind": kindByType(objectType),
            "role": String(props.geometryRole || ""),
            "calcSource": String(props.calcSource || ""),
            "legacyScoreRaw": Number(props.legacyScoreRaw !== undefined ? props.legacyScoreRaw : 0.5),
            "legacyScoreNormalized": Number(props.legacyScoreNormalized !== undefined ? props.legacyScoreNormalized : 0.5),
            "heatValue": Number(props.heatValue !== undefined ? props.heatValue : 0.5),
            "heatClass": Number(props.heatClass !== undefined ? props.heatClass : 0)
        }

        if (geometry.type === "LineString") {
            var linePath = []
            var coords = geometry.coordinates || []
            for (var i = 0; i < coords.length; ++i) {
                var point = coordinatePairToPoint(coords[i])
                if (point)
                    linePath.push(point)
            }
            if (linePath.length >= 2)
                entries.push(Object.assign({ "path": linePath }, base))
            return entries
        }

        if (geometry.type === "MultiLineString") {
            var lineList = geometry.coordinates || []
            for (var l = 0; l < lineList.length; ++l) {
                var segment = lineList[l] || []
                var segmentPath = []
                for (var p = 0; p < segment.length; ++p) {
                    var segmentPoint = coordinatePairToPoint(segment[p])
                    if (segmentPoint)
                        segmentPath.push(segmentPoint)
                }
                if (segmentPath.length >= 2) {
                    entries.push(Object.assign({
                        "id": base.id + "-l" + l,
                        "path": segmentPath
                    }, base))
                }
            }
        }
        return entries
    }

    function buildPolygonEntriesFromFeature(feature) {
        var entries = []
        if (!feature || !feature.geometry)
            return entries

        var geometry = feature.geometry
        var props = feature.properties || {}
        var objectType = Number(props.objectType || 0)
        var objectId = Number(props.objectId || 0)
        var base = {
            "id": objectType + "-" + objectId,
            "objectType": objectType,
            "objectId": objectId,
            "name": props.title || ("Object " + objectId),
            "side": sideByType(objectType),
            "kind": kindByType(objectType),
            "role": String(props.geometryRole || ""),
            "calcSource": String(props.calcSource || ""),
            "legacyScoreRaw": Number(props.legacyScoreRaw !== undefined ? props.legacyScoreRaw : 0.5),
            "legacyScoreNormalized": Number(props.legacyScoreNormalized !== undefined ? props.legacyScoreNormalized : 0.5),
            "heatValue": Number(props.heatValue !== undefined ? props.heatValue : 0.5),
            "heatClass": Number(props.heatClass !== undefined ? props.heatClass : 0)
        }

        function ringToPath(ring) {
            var path = []
            for (var i = 0; i < ring.length; ++i) {
                var point = coordinatePairToPoint(ring[i])
                if (point)
                    path.push(point)
            }
            return path
        }

        if (geometry.type === "Polygon") {
            var rings = geometry.coordinates || []
            if (rings.length > 0) {
                var outer = ringToPath(rings[0])
                if (outer.length >= 3)
                    entries.push(Object.assign({ "path": outer }, base))
            }
            return entries
        }

        if (geometry.type === "MultiPolygon") {
            var polygons = geometry.coordinates || []
            for (var g = 0; g < polygons.length; ++g) {
                var polyRings = polygons[g] || []
                if (polyRings.length === 0)
                    continue
                var polyOuter = ringToPath(polyRings[0])
                if (polyOuter.length >= 3) {
                    entries.push(Object.assign({
                        "id": base.id + "-p" + g,
                        "path": polyOuter
                    }, base))
                }
            }
        }
        return entries
    }

    function refreshReferenceTreeFromDb() {
        if (!Database.connected)
            return
        var objectsLimit = 1500
        var personnelLimit = 1500
        var eventsLimit = 1500
        var objectTypeNodes = []
        var mapTypes = [1, 3, 4, 5, 6, 9, 10]
        for (var i = 0; i < mapTypes.length; ++i) {
            var mapType = mapTypes[i]
            var mapRows = MapObjectsRepo.listObjectsByType(mapType, objectsLimit)
            var mapLeaves = []
            for (var j = 0; j < mapRows.length; ++j) {
                var mapRow = mapRows[j]
                mapLeaves.push({
                    "id": "obj_" + mapType + "_" + mapRow.id,
                    "label": mapRow.name ? String(mapRow.name) : (typeLabel(mapType) + " #" + mapRow.id),
                    "subtitle": mapRow.subtitle ? String(mapRow.subtitle) : "",
                    "nodeKind": "object",
                    "objectType": mapType,
                    "objectId": Number(mapRow.id),
                    "children": []
                })
            }
            objectTypeNodes.push({
                "id": "map_type_" + mapType,
                "label": typeLabel(mapType) + " [" + mapLeaves.length + "]",
                "children": mapLeaves
            })
        }

        var personnelRows = PersonnelRepo.listPersonnel("", personnelLimit, 0)
        var personnelGroups = ({})
        for (var p = 0; p < personnelRows.length; ++p) {
            var person = personnelRows[p]
            var personType = person.typeName ? String(person.typeName) : "Без типа"
            if (!personnelGroups[personType])
                personnelGroups[personType] = []
            personnelGroups[personType].push({
                "id": "pers_" + Number(person.id),
                "label": person.name ? String(person.name) : ("Персоналия #" + person.id),
                "subtitle": person.rank ? String(person.rank) : "",
                "nodeKind": "object",
                "objectType": 7,
                "objectId": Number(person.id),
                "children": []
            })
        }
        var personnelNodes = []
        var personnelTypes = Object.keys(personnelGroups)
        personnelTypes.sort()
        for (var pt = 0; pt < personnelTypes.length; ++pt) {
            var personnelTypeName = personnelTypes[pt]
            var personnelLeaves = personnelGroups[personnelTypeName]
            personnelNodes.push({
                "id": "pers_type_" + pt,
                "label": personnelTypeName + " [" + personnelLeaves.length + "]",
                "children": personnelLeaves
            })
        }

        var specialRows = MapObjectsRepo.listObjectsByType(2, objectsLimit)
        var specialGroups = ({})
        for (var s = 0; s < specialRows.length; ++s) {
            var sc = specialRows[s]
            var scType = sc.subtitle ? String(sc.subtitle) : "Без типа"
            if (!specialGroups[scType])
                specialGroups[scType] = []
            specialGroups[scType].push({
                "id": "sc_" + Number(sc.id),
                "label": sc.name ? String(sc.name) : ("Особое условие #" + sc.id),
                "subtitle": sc.subtitle ? String(sc.subtitle) : "",
                "nodeKind": "object",
                "objectType": 2,
                "objectId": Number(sc.id),
                "children": []
            })
        }
        var specialNodes = []
        var specialTypes = Object.keys(specialGroups)
        specialTypes.sort()
        for (var st = 0; st < specialTypes.length; ++st) {
            var specialTypeName = specialTypes[st]
            var specialLeaves = specialGroups[specialTypeName]
            specialNodes.push({
                "id": "sc_type_" + st,
                "label": specialTypeName + " [" + specialLeaves.length + "]",
                "children": specialLeaves
            })
        }

        var eventRows = EventsRepo.listEvents("", eventsLimit, 0)
        var eventStatusGroups = ({})
        for (var e = 0; e < eventRows.length; ++e) {
            var eventRow = eventRows[e]
            var statusKey = eventStatusValue(eventRow.statusName, eventRow.start, eventRow.end)
            if (!eventStatusGroups[statusKey])
                eventStatusGroups[statusKey] = ({})
            var typeKey = eventTypeValue(eventRow.typeName)
            if (!eventStatusGroups[statusKey][typeKey])
                eventStatusGroups[statusKey][typeKey] = []
            eventStatusGroups[statusKey][typeKey].push({
                "id": "event_" + Number(eventRow.id),
                "label": eventRow.name ? String(eventRow.name) : ("Событие #" + eventRow.id),
                "nodeKind": "event",
                "eventId": Number(eventRow.id),
                "eventType": typeKey,
                "eventStatus": statusKey,
                "eventStart": eventRow.start ? String(eventRow.start) : "",
                "eventEnd": eventRow.end ? String(eventRow.end) : "",
                "children": []
            })
        }
        var eventNodes = []
        var statusOrder = ["actual", "planned", "past", "cancelled"]
        for (var so = 0; so < statusOrder.length; ++so) {
            var statusName = statusOrder[so]
            var typeGroups = eventStatusGroups[statusName]
            if (!typeGroups)
                continue
            var typeNodes = []
            var typeKeys = Object.keys(typeGroups)
            typeKeys.sort()
            for (var tk = 0; tk < typeKeys.length; ++tk) {
                var typeName = typeKeys[tk]
                var eventLeaves = typeGroups[typeName]
                typeNodes.push({
                    "id": "event_type_" + statusName + "_" + typeName,
                    "label": typeName + " [" + eventLeaves.length + "]",
                    "children": eventLeaves
                })
            }
            eventNodes.push({
                "id": "event_status_" + statusName,
                "label": eventStatusLabel(statusName) + " [" + typeNodes.length + "]",
                "children": typeNodes
            })
        }

        var roots = [{
            "id": "db_entities",
            "label": "Сущности БД",
            "children": [
                { "id": "db_map_objects", "label": "Объекты", "children": objectTypeNodes },
                { "id": "db_personnel", "label": "Персоналии", "children": personnelNodes },
                { "id": "db_special_conditions", "label": "Особые условия", "children": specialNodes },
                { "id": "db_events", "label": "События", "children": eventNodes }
            ]
        }]
        referenceTreeData = roots
        if (structureAgent)
            structureAgent.referenceTree = roots
    }

    function refreshEventsFromDb() {
        if (!Database.connected)
            return
        var list = EventsRepo.listEvents("", 500, 0)
        var mapped = []
        for (var i = 0; i < list.length; ++i) {
            var row = list[i]
            var eventId = Number(row.id || 0)
            mapped.push({
                "id": eventId,
                "name": row.name || ("Событие " + eventId),
                "type": eventTypeValue(row.typeName),
                "status": eventStatusValue(row.statusName, row.start, row.end),
                "startTimestamp": row.start ? String(row.start) : "",
                "endTimestamp": row.end ? String(row.end) : "",
                "updatedTimestamp": "",
                "description": "",
                "objectIds": []
            })
        }
        mapEvents = mapped
    }

    function refreshObjectsFromRuntime() {
        var pointsJson = MapRuntime.pointsSource
        var linesJson = MapRuntime.linesSource
        var polygonsJson = MapRuntime.polygonsSource
        if (!pointsJson || pointsJson.length === 0) {
            mapObjects = []
            mapLines = []
            mapPolygons = []
            locationLabels = []
            return
        }

        var parsed = JSON.parse(pointsJson)
        var features = parsed.features || []
        var objects = []
        var objectByKey = ({})
        var lines = []
        var polygons = []
        var labels = []

        for (var i = 0; i < features.length; ++i) {
            var feature = features[i]
            if (!feature || !feature.geometry || feature.geometry.type !== "Point")
                continue

            var coords = feature.geometry.coordinates || []
            if (coords.length < 2)
                continue

            var props = feature.properties || {}
            var objectType = Number(props.objectType || 0)
            var objectId = Number(props.objectId || 0)
            if (objectType <= 0 || objectId <= 0)
                continue
            var legacyScore = Number(props.legacyScoreNormalized !== undefined ? props.legacyScoreNormalized : 0.5)
            var heatValue = Number(props.heatValue !== undefined ? props.heatValue : (1.0 - legacyScore))
            var geometryRole = String(props.geometryRole || "").toLowerCase()
            var key = objectType + "-" + objectId
            var row = {
                "id": key,
                "objectType": objectType,
                "objectId": objectId,
                "name": props.title || ("Object " + objectId),
                "side": sideByType(objectType),
                "kind": kindByType(objectType),
                "role": geometryRole,
                "lat": Number(coords[1]),
                "lon": Number(coords[0]),
                "mpps": Math.round(legacyScore * 100),
                "speed": 0,
                "course": 0,
                "source": "backend",
                "notes": props.subtitle || "",
                "structurePath": structurePathByType(objectType),
                "calcSource": String(props.calcSource || ""),
                "legacyScoreRaw": Number(props.legacyScoreRaw !== undefined ? props.legacyScoreRaw : legacyScore),
                "legacyScoreNormalized": legacyScore,
                "heatValue": heatValue,
                "heatClass": Number(props.heatClass !== undefined ? props.heatClass : 0)
            }
            var existing = objectByKey[key]
            if (!existing) {
                objectByKey[key] = row
            } else {
                var existingIsPosition = existing.role === "position"
                var currentIsPosition = geometryRole === "position"
                if (currentIsPosition && !existingIsPosition)
                    objectByKey[key] = row
            }
        }

        var objectKeys = Object.keys(objectByKey)
        objectKeys.sort()
        for (var ok = 0; ok < objectKeys.length; ++ok) {
            var objectRow = objectByKey[objectKeys[ok]]
            objects.push(objectRow)
            if (labels.length < 24)
                labels.push({ "name": objectRow.name, "lat": objectRow.lat, "lon": objectRow.lon })
        }

        mapObjects = objects
        if (linesJson && linesJson.length > 0) {
            var linesCollection = JSON.parse(linesJson)
            var lineFeatures = linesCollection.features || []
            for (var l = 0; l < lineFeatures.length; ++l) {
                var lineEntries = buildLineEntriesFromFeature(lineFeatures[l])
                for (var le = 0; le < lineEntries.length; ++le)
                    lines.push(lineEntries[le])
            }
        }

        if (polygonsJson && polygonsJson.length > 0) {
            var polygonsCollection = JSON.parse(polygonsJson)
            var polygonFeatures = polygonsCollection.features || []
            for (var p = 0; p < polygonFeatures.length; ++p) {
                var polygonEntries = buildPolygonEntriesFromFeature(polygonFeatures[p])
                for (var pe = 0; pe < polygonEntries.length; ++pe)
                    polygons.push(polygonEntries[pe])
            }
        }

        mapLines = lines
        mapPolygons = polygons
        locationLabels = labels
        if (mapEvents.length > 0)
            mapEvents[0].objectIds = objects.length > 0 ? [objects[0].id] : []
        appState.statusMessage = "Объектов: " + objects.length + ", линий: " + lines.length + ", полигонов: " + polygons.length
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
        dbBootstrapTimer.restart()
    }

    function runConnectedBootstrap() {
        if (!Database.connected)
            return
        appState.statusMessage = "Загрузка данных из БД..."
        MapRuntime.refreshNow()
        refreshEventsFromDb()
        refreshReferenceTreeFromDb()
        pendingDbBootstrap = false
    }

    onPanelChanged: {
        if (panel === "none" && pendingDbBootstrap && Database.connected)
            scheduleConnectedBootstrap()
    }

    Component.onCompleted: {
        Polling.intervalMs = 30000
        if (Database.connected) {
            if (!Polling.running)
                Polling.start()
            if (panel === "none")
                scheduleConnectedBootstrap()
            else
                pendingDbBootstrap = true
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
        }
    }

    Connections {
        target: Polling
        function onTick() {
            if (!Database.connected)
                return
            refreshEventsFromDb()
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
        }
    }

    Timer {
        id: dbBootstrapTimer
        interval: 0
        repeat: false
        onTriggered: runConnectedBootstrap()
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
            if (root.structureAgent && root.stateManager.selectedStructurePath !== root.structureAgent.selectedPath)
                root.structureAgent.selectedPath = root.stateManager.selectedStructurePath
        }
    }

    Timer {
        id: controlsReturnTimer
        interval: 1100
        repeat: false
        onTriggered: root.agentHub.uiStateAgent.setControlsVisible(true)
    }

    Item {
        id: dragSurface
        anchors.fill: parent

        Toolbar {
            id: topBar
            z: 40
            width: Math.min(parent.width - 32, 680)
            anchors.top: parent.top
            anchors.topMargin: 18
            anchors.horizontalCenter: parent.horizontalCenter
            appState: root.stateManager
            agentHub: root.agentHub
            panelManager: root.panelManager
            backdropSource: mapCanvas.sceneContent
        }

        Item {
            id: inspectorReopenHandle
            z: 55
            visible: root.stateManager ? !root.stateManager.inspectorVisible : false
            width: 36
            height: 138
            anchors.right: parent.right
            anchors.rightMargin: 8
            anchors.verticalCenter: parent.verticalCenter

            GlassPanel {
                anchors.fill: parent
                radius: 18
                padding: 0
                backdropSource: mapCanvas.sceneContent
                surfaceColor: "#141c24"
                surfaceOpacity: reopenMouseArea.pressed ? 0.68 : reopenMouseArea.containsMouse ? 0.62 : 0.56
                shadowOpacity: 0.08
                highlightOpacity: 0.05
                edgeOpacity: 0.07

                Text {
                    anchors.centerIn: parent
                    rotation: -90
                    text: "Инспектор"
                    color: Qt.rgba(1, 1, 1, 0.90)
                    font.pixelSize: 10
                    font.weight: Font.Medium
                }
            }

            MouseArea {
                id: reopenMouseArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    if (root.agentHub && root.agentHub.uiStateAgent)
                        root.agentHub.uiStateAgent.setInspectorVisible(true)
                }
            }
        }

        ReferenceSidebar {
            id: leftSidebar
            z: 30
            x: root.referencePanelState && root.referencePanelState.detached
                ? (root.referencePanelState.x || 16)
                : 16
            y: root.referencePanelState && root.referencePanelState.detached
                ? (root.referencePanelState.y || (topBar.y + topBar.height + 14))
                : (topBar.y + topBar.height + 14)
            height: root.referencePanelState && root.referencePanelState.detached
                ? Math.max(72, root.referencePanelState.height || 300)
                : (statusBar.visible ? (statusBar.y - 12 - y) : (parent.height - 16 - y))
            panelManager: root.panelManager
            panelState: root.referencePanelState
            appState: root.stateManager
            agentHub: root.agentHub
            objects: root.mapObjects
            structureAgent: root.structureAgent
            backdropSource: mapCanvas.sceneContent
            dragSurface: dragSurface
        }

        RightSidebar {
            id: rightSidebar
            z: 30
            x: root.rightSidebarState && root.rightSidebarState.detached
                ? (root.rightSidebarState.x || (parent.width - width - 16))
                : (parent.width - width - 16)
            y: root.rightSidebarState && root.rightSidebarState.detached
                ? (root.rightSidebarState.y || (topBar.y + topBar.height + 14))
                : (topBar.y + topBar.height + 14)
            height: root.rightSidebarState && root.rightSidebarState.detached
                ? Math.max(72, root.rightSidebarState.height || 300)
                : (statusBar.visible ? (statusBar.y - 12 - y) : (parent.height - 16 - y))
            appState: root.stateManager
            agentHub: root.agentHub
            objects: root.mapObjects
            events: root.mapEvents
            panelManager: root.panelManager
            panelState: root.rightSidebarState
            backdropSource: mapCanvas.sceneContent
            dragSurface: dragSurface
        }

        BottomStatusBar {
            id: statusBar
            z: 35
            anchors.left: parent.left
            anchors.leftMargin: root.leftDockedVisible ? leftSidebar.width + 28 : 16
            anchors.right: parent.right
            anchors.rightMargin: root.rightDockedVisible ? rightSidebar.width + 28 : 16
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 8
            appState: root.stateManager
            panelManager: root.panelManager
            panelState: root.statusBarState
        }

        MapCanvas {
            id: mapCanvas
            z: 10
            anchors.left: parent.left
            anchors.leftMargin: root.leftDockedVisible ? leftSidebar.width + 28 : 16
            anchors.right: parent.right
            anchors.rightMargin: root.rightDockedVisible ? rightSidebar.width + 28 : 16
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
            anchors.left: parent.left
            anchors.leftMargin: 18
            anchors.top: parent.top
            anchors.topMargin: 18
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
            }
        }

        Item {
            id: panelHost
            z: 65
            anchors.right: parent.right
            anchors.rightMargin: 18
            anchors.top: parent.top
            anchors.topMargin: 18
            width: Math.min(parent.width * 0.42, 580)
            height: Math.min(parent.height * 0.48, 420)
            visible: root.panel !== "none"

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
        onActivated: root.agentHub.uiStateAgent.setMapMode("point")
    }

    Shortcut {
        sequence: "Ctrl+2"
        onActivated: root.agentHub.uiStateAgent.setMapMode("heatmap")
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
