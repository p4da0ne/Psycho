pragma ComponentBehavior: Bound
import QtQuick 2.15
import Saturn.Backend 1.0

// Куст агентов приложения. Вынесен из AppShell.qml ради читаемости.
// Зависимости инжектятся: appState, panelManager и host (корень AppShell —
// для mapObjects/mapEvents/referenceTreeData и его функций-хелперов).
QtObject {
    id: agentHub

    property var appState
    property var panelManager
    property var host

    property var filterAgent: QtObject {
        function parseFilterDateMs(rawValue, endOfDay) {
            var text = String(rawValue || "").trim()
            if (text === "")
                return NaN
            if (text.length === 10 && text.indexOf("T") === -1)
                text += endOfDay ? "T23:59:59" : "T00:00:00"
            var ms = (new Date(text)).getTime()
            return isNaN(ms) ? NaN : ms
        }

        function locationBucket(latValue, lonValue) {
            var lat = Number(latValue)
            var lon = Number(lonValue)
            if (!isFinite(lat) || !isFinite(lon))
                return "unknown"
            if (lat < 41 || lat > 82)
                return "foreign"
            var lon360 = lon
            if (lon360 < 0)
                lon360 += 360
            if (lon360 >= 19 && lon360 <= 191)
                return "russia"
            return "foreign"
        }

        function objectMatchesType(objectData) {
            var selectedType = Number(appState.quickFilterObjectType || 0)
            if (selectedType <= 0)
                return true
            if (selectedType === 8)
                return false
            return Number(objectData.objectType || 0) === selectedType
        }

        function objectMatchesOwnership(objectData) {
            if (appState.quickFilterOwnership === "friendly")
                return String(objectData.side || "") === "friendly"
            if (appState.quickFilterOwnership === "enemy")
                return String(objectData.side || "") === "foreign"
            return true
        }

        function objectMatchesRegion(objectData) {
            var regionScope = String(appState.quickFilterRegionScope || "all")
            if (regionScope === "all")
                return true
            var dbScope = String(objectData.countryScope || "").toLowerCase()
            if (dbScope === "russia")
                return regionScope === "russia"
            if (dbScope === "foreign")
                return regionScope === "foreign"
            var bucket = locationBucket(objectData.lat, objectData.lon)
            if (bucket === "unknown")
                return true
            if (regionScope === "russia")
                return bucket === "russia"
            if (regionScope === "foreign")
                return bucket === "foreign"
            return true
        }

        function objectVisible(objectData) {
            if (!objectData)
                return false
            if (appState.searchText !== "") {
                var needle = appState.searchText.toLowerCase()
                var haystack = (objectData.name + " " + objectData.id + " " + objectData.kind).toLowerCase()
                if (haystack.indexOf(needle) === -1)
                    return false
            }
            if (appState.selectedGroup === "units" && objectData.kind !== "unit")
                return false
            if (appState.selectedGroup === "relays" && objectData.kind !== "relay")
                return false
            if (appState.selectedGroup === "lbs" && objectData.kind !== "lbs")
                return false
            if (objectData.side === "friendly" && !appState.showFriendly)
                return false
            if (objectData.side === "foreign" && !appState.showForeign)
                return false
            if (objectData.side === "infrastructure" && !appState.showBaseStations)
                return false
            if (!objectMatchesType(objectData))
                return false
            if (!objectMatchesOwnership(objectData))
                return false
            if (!objectMatchesRegion(objectData))
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
            var selectedType = Number(appState.quickFilterObjectType || 0)
            if (selectedType > 0 && selectedType !== 8)
                return []
            var source = items || []
            var result = []
            var fromMs = parseFilterDateMs(appState.quickFilterEventFrom, false)
            var toMs = parseFilterDateMs(appState.quickFilterEventTo, true)
            var regionScope = String(appState.quickFilterRegionScope || "all")
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
                if (Number(appState.eventTypeFilterId || 0) > 0
                        && Number(row.idTypeEvent || 0) !== Number(appState.eventTypeFilterId || 0))
                    continue
                if (!isNaN(fromMs) || !isNaN(toMs)) {
                    var rowStartMs = (new Date(String(row.startTimestamp || ""))).getTime()
                    if (!isNaN(fromMs) && !isNaN(rowStartMs) && rowStartMs < fromMs)
                        continue
                    if (!isNaN(toMs) && !isNaN(rowStartMs) && rowStartMs > toMs)
                        continue
                }
                if (regionScope !== "all") {
                    var eventBucket = locationBucket(row.lat, row.lon)
                    if (eventBucket !== "unknown") {
                        if (regionScope === "russia" && eventBucket !== "russia")
                            continue
                        if (regionScope === "foreign" && eventBucket !== "foreign")
                            continue
                    }
                }
                result.push(row)
            }
            return result
        }
    }

    property var selectionAgent: QtObject {
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
                host.loadObjectDetailsForObject(appState.selectedObject)
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
                var objectType = Number(obj.objectType || 0)
                if (objectType <= 0)
                    objectType = host.objectTypeByTableName(obj)
                if (objectType > 0)
                    linked.push(objectType + "-" + Number(obj.idObject || 0))
            }
            appState.selectedEvent = {
                "id": eventData.id,
                "name": eventData.name,
                "type": eventData.type,
                "status": eventData.status,
                "idTypeEvent": Number(eventData.idTypeEvent || (details ? details.idTypeEvent : 0) || 0),
                "idEventStatus": Number(eventData.idEventStatus || (details ? details.idEventStatus : 0) || 0),
                "idSign": Number(eventData.idSign || (details ? details.idSign : 0) || 0),
                "signKey": String(eventData.signKey || (details ? details.signKey : "") || ""),
                "startTimestamp": eventData.startTimestamp,
                "endTimestamp": eventData.endTimestamp,
                "updatedTimestamp": eventData.updatedTimestamp,
                "description": details && details.description ? String(details.description) : (eventData.description || ""),
                "objectIds": linked,
                "details": details || ({})
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

    property var uiStateAgent: QtObject {
        function setInspectorVisible(value) { panelManager.setPanelVisible("right-sidebar", !!value) }
        function toggleInspectorVisible() { setInspectorVisible(!appState.inspectorVisible) }
        function setNavigationVisible(value) { panelManager.setPanelVisible("reference-panel", !!value) }
        function toggleNavigationVisible() { setNavigationVisible(!appState.navigationVisible) }
        function setControlsVisible(value) { appState.controlsVisible = !!value }
        function setSearchText(textValue) { appState.searchText = textValue || "" }
        function focusSearchResult() {
            if (appState.searchText === "") {
                appState.statusMessage = "Поиск: строка пуста"
                return
            }

            var objects = filterAgent.visibleObjects(host.mapObjects || [])
            if (objects.length > 0) {
                var firstObject = objects[0]
                selectionAgent.selectObject(firstObject)
                if (firstObject.lon !== undefined && firstObject.lat !== undefined) {
                    appState.centerLon = Number(firstObject.lon)
                    appState.centerLat = Number(firstObject.lat)
                }
                appState.statusMessage = "Поиск: выбран объект " + (firstObject.name || "")
                return
            }

            var events = filterAgent.visibleEvents(host.mapEvents || [])
            if (events.length > 0) {
                var firstEvent = events[0]
                selectionAgent.selectEvent(firstEvent)
                appState.statusMessage = "Поиск: выбрано событие " + (firstEvent.name || "")
                return
            }

            appState.statusMessage = "Поиск: совпадений не найдено"
        }
        function setFilterState(filterKey, enabled) {
            if (filterKey === "friendly")
                appState.showFriendly = !!enabled
            else if (filterKey === "foreign")
                appState.showForeign = !!enabled
            else if (filterKey === "infrastructure")
                appState.showBaseStations = !!enabled
        }
        function setQuickFilterObjectType(objectTypeValue) { appState.quickFilterObjectType = Number(objectTypeValue || 0) }
        function setQuickFilterOwnership(value) { appState.quickFilterOwnership = String(value || "all") }
        function setQuickFilterRegionScope(value) { appState.quickFilterRegionScope = String(value || "all") }
        function setQuickFilterEventRange(fromValue, toValue) {
            appState.quickFilterEventFrom = String(fromValue || "")
            appState.quickFilterEventTo = String(toValue || "")
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
                var runtimeObject = host.findRuntimeObject(selectedNode.objectType, selectedNode.objectId)
                if (!runtimeObject) {
                    runtimeObject = {
                        "id": selectedNode.objectType + "-" + selectedNode.objectId,
                        "objectType": Number(selectedNode.objectType),
                        "objectId": Number(selectedNode.objectId),
                        "name": selectedNode.label || ("Object " + selectedNode.objectId),
                        "side": host.sideByType(Number(selectedNode.objectType)),
                        "kind": host.kindByType(Number(selectedNode.objectType)),
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

    property var structureAgent: QtObject {
        property var referenceTree: host.referenceTreeData
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
            host.referenceTreeData = referenceTree
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
            host.referenceTreeData = referenceTree
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
            host.referenceTreeData = referenceTree
        }

        onSelectedPathChanged: {
            selectedNode = nodeDetails(selectedPath)
            appState.selectedStructurePath = selectedPath
            canDeleteSelected = selectedPath !== ""
        }
    }

    property var symbolAgent: QtObject {
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

    property var renderAgent: QtObject {
        function shouldRenderHeatmap() { return appState.mapMode === "heatmap" }
        function shouldRenderCoverageLine() { return appState.showCoverageLine }
        function shouldRenderLabels() { return appState.showLabels }
    }
}
