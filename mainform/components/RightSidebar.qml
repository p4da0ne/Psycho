pragma ComponentBehavior: Bound
import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/theme"

Item {
    id: root

    property var appState
    property var agentHub
    property var objects: []
    property var events: []
    property Item backdropSource
    property Item dragSurface
    property var panelManager
    property var panelState: ({})
    property string currentTab: "inspector"
    property real dragStartWidth: 0
    property string symbolDraftName: ""
    property string symbolDraftGlyph: ""
    property string symbolDraftFillColor: ""
    property string symbolDraftTopColor: ""
    property real symbolDraftInnerOpacity: 0.8
    property bool uiCollapsed: panelState && panelState.collapsed === true

    readonly property var effectivePanelState: {
        if (panelManager && panelManager.panelById) {
            panelManager.panels
            return panelManager.panelById("right-sidebar")
        }
        return panelState || ({})
    }
    readonly property bool panelVisible: appState && appState.inspectorVisible !== undefined
        ? appState.inspectorVisible
        : (effectivePanelState && effectivePanelState.visible !== undefined ? effectivePanelState.visible : true)
    readonly property bool collapsed: uiCollapsed
    readonly property real expandedWidth: effectivePanelState && effectivePanelState.width ? effectivePanelState.width : 372
    readonly property real targetWidth: panelVisible ? (collapsed ? 132 : expandedWidth) : 0
    readonly property var filterAgent: agentHub ? agentHub.filterAgent : null
    readonly property var selectionAgent: agentHub ? agentHub.selectionAgent : null
    readonly property var symbolAgent: agentHub ? agentHub.symbolAgent : null
    readonly property var structureAgent: agentHub ? agentHub.structureAgent : null
    readonly property var filteredObjects: filterAgent ? filterAgent.visibleObjects(objects || []) : []
    readonly property var filteredEvents: filterAgent ? filterAgent.visibleEvents(events || []) : []
    readonly property var selectedObject: appState ? appState.selectedObject : ({})
    readonly property var selectedObjectDetails: appState ? (appState.selectedObjectDetails || ({})) : ({})
    readonly property var selectedEvent: appState ? appState.selectedEvent : ({})
    readonly property var eventStatusOptions: [
        { "value": "all", "label": "Все" },
        { "value": "actual", "label": "Актуальные" },
        { "value": "planned", "label": "План" },
        { "value": "past", "label": "Прошлые" },
        { "value": "cancelled", "label": "Отмененные" }
    ]
    property var eventTypeOptions: [{ "idTypeEvent": 0, "label": "Все типы" }]
    readonly property string selectedStructurePath: appState ? appState.selectedStructurePath : ""
    readonly property var selectedStructureNode: structureAgent ? structureAgent.nodeDetails(selectedStructurePath) : ({})
    readonly property bool structureMode: appState && appState.selectionType === "structure" && selectedStructurePath !== ""
    readonly property var symbolList: symbolAgent ? symbolAgent.symbolsForSelectedType : []
    readonly property var selectedSymbol: symbolAgent ? symbolAgent.selectedSymbol : ({})

    implicitWidth: targetWidth
    width: targetWidth
    visible: width > 0.5
    clip: true
    enabled: panelVisible

    Behavior on width {
        NumberAnimation { duration: 180 }
    }

    WheelHandler {
        target: null
        onWheel: function(event) {
            event.accepted = true
        }
    }

    onCurrentTabChanged: {
        if (agentHub && agentHub.uiStateAgent)
            agentHub.uiStateAgent.setInspectorTab(currentTab === "events" ? 1 : 0)
    }

    function openTab(tabName) {
        currentTab = tabName
        if (agentHub && agentHub.uiStateAgent) {
            agentHub.uiStateAgent.setInspectorVisible(true)
            agentHub.uiStateAgent.setActivePanel(1)
            agentHub.uiStateAgent.setInspectorTab(tabName === "events" ? 1 : 0)
        }
    }

    function closePanel() {
        var nextCollapsed = !root.uiCollapsed
        root.uiCollapsed = nextCollapsed
        if (root.panelManager && root.panelManager.setPanelState)
            root.panelManager.setPanelState("right-sidebar", { "visible": true, "collapsed": nextCollapsed })
        if (root.appState)
            root.appState.inspectorVisible = true
    }

    function toggleCollapsed() {
        root.closePanel()
    }

    function statusColor(statusValue) {
        switch (statusValue) {
        case "actual":
            return Theme.accentVivid
        case "planned":
            return Theme.friendlyRgb
        case "past":
            return Theme.muteRgb
        case "cancelled":
            return Theme.dangerRgb
        default:
            return Theme.slateRgb
        }
    }

    function statusLabel(statusValue) {
        switch (statusValue) {
        case "actual":
            return "Актуально"
        case "planned":
            return "План"
        case "past":
            return "Прошло"
        case "cancelled":
            return "Отменено"
        default:
            return "Неизвестно"
        }
    }

    function typeIcon(typeValue) {
        switch (typeValue) {
        case "cyber":
            return "≈"
        case "fire":
            return "◎"
        case "intel":
            return "◈"
        case "other":
            return "⌁"
        default:
            return "⌘"
        }
    }

    function typeLabel(typeValue) {
        switch (typeValue) {
        case "cyber":
            return "Кибер"
        case "fire":
            return "Огневые"
        case "intel":
            return "Разведка"
        case "other":
            return "Прочие"
        default:
            return "Событие"
        }
    }

    function symbolPreviewColor() {
        return symbolDraftFillColor || Theme.friendly
    }

    function tabNameByIndex(index) {
        return index === 1 ? "events" : "inspector"
    }

    function symbolTypeLabel(typePath) {
        switch (typePath) {
        case "units/friendly":
            return "Подразделения / Свои"
        case "units/foreign":
            return "Подразделения / Чужие"
        case "relays/friendly":
            return "Узлы связи / Свои"
        case "infrastructure/lbs":
            return "Инфраструктура / Линейные объекты"
        default:
            return "Тип не выбран"
        }
    }

    function structureKindLabel(nodeData) {
        if (!nodeData || !nodeData.id)
            return "—"
        return (nodeData.children && nodeData.children.length > 0) ? "Группа" : "Элемент"
    }

    function scalarToText(value) {
        if (value === null || value === undefined)
            return "—"
        return String(value)
    }

    function appendObjectRows(rows, prefix, objectValue, level) {
        if (!objectValue || level > 3)
            return
        var keys = Object.keys(objectValue)
        keys.sort()
        for (var i = 0; i < keys.length; ++i) {
            var key = keys[i]
            var value = objectValue[key]
            var label = prefix !== "" ? (prefix + "." + key) : key
            if (value === null || value === undefined)
                continue
            if (Array.isArray(value)) {
                rows.push({ "label": label, "value": "[" + value.length + "]" })
                var limit = Math.min(value.length, 12)
                for (var a = 0; a < limit; ++a) {
                    var arrayItem = value[a]
                    if (arrayItem !== null && typeof arrayItem === "object")
                        appendObjectRows(rows, label + "#" + (a + 1), arrayItem, level + 1)
                    else
                        rows.push({ "label": label + "#" + (a + 1), "value": scalarToText(arrayItem) })
                }
                if (value.length > limit) {
                    rows.push({ "label": label, "value": "... +" + (value.length - limit) + " записей" })
                }
                continue
            }
            if (typeof value === "object") {
                appendObjectRows(rows, label, value, level + 1)
                continue
            }
            if (String(value) === "")
                continue
            rows.push({ "label": label, "value": scalarToText(value) })
        }
    }

    function toggleBooleanState(key) {
        if (!root.appState)
            return
        root.appState[key] = !root.appState[key]
    }

    function detailsRows() {
        var rows = []
        if (!root.selectedObjectDetails)
            return rows

        var dbPayload = root.selectedObjectDetails.fullRow
        if (!dbPayload || Object.keys(dbPayload).length === 0)
            dbPayload = root.selectedObjectDetails.payload
        appendObjectRows(rows, "", dbPayload, 0)
        return rows
    }

    // Соответствие object_type → русское название (синхронизировано с Toolbar.objectTypeFilters).
    function objectTypeLabel(typeId) {
        var t = Number(typeId || 0)
        if (t === 1) return "Формирование"
        if (t === 2) return "Особое условие"
        if (t === 3) return "Средство СМИ"
        if (t === 4) return "Средство формирования"
        if (t === 5) return "Средство группы"
        if (t === 6) return "Регион"
        if (t === 7) return "Персоналия"
        if (t === 8) return "Событие"
        if (t === 9) return "Группа"
        if (t === 10) return "СМИ"
        return ""
    }

    // Whitelist полей БД с человеческими лейблами.
    // Возвращает массив {label, value} только для непустых и осмысленных значений.
    function readableDetailsRows() {
        var rows = []
        function add(label, value) {
            if (value === null || value === undefined)
                return
            var text = String(value).trim()
            if (text === "" || text === "0" || text === "0.0" || text === "NaN" || text === "null")
                return
            rows.push({ "label": label, "value": text })
        }

        // 1. Тип объекта — из selectedObject (русский лейбл).
        if (root.selectedObject && root.selectedObject.objectType) {
            var typeLabel = objectTypeLabel(root.selectedObject.objectType)
            if (typeLabel !== "")
                add("Тип объекта", typeLabel)
        }

        var details = root.selectedObjectDetails || {}
        var payload = details.fullRow || details.payload || {}
        if (!payload || typeof payload !== "object")
            return rows

        // 2. Имя объекта — приоритет name_* (любого), затем name, затем title.
        var nameKey = ""
        for (var k in payload) {
            if (k === "name" || /^name_/.test(k) || k === "title") {
                if (String(payload[k] || "").trim() !== "") {
                    nameKey = k
                    add("Название", payload[k])
                    break
                }
            }
        }

        // 3. Короткое имя.
        for (var k2 in payload) {
            if (k2 === "short_name" || /^short_name_/.test(k2)) {
                add("Краткое имя", payload[k2])
                break
            }
        }

        // 4. Дополнительные типы / роли (имена справочников, без id).
        var typeKeys = [
            "type_name", "rank_name", "name_type_event",
            "name_type_special_conditions", "name_type_ls",
            "name_type_mpo_pso", "name_type_persones",
            "name_type_region", "nametype_smi", "name_type_office_smi",
            "name_type_broadcast_smi", "name_theme_smi", "name_level_smi",
            "name_position_smi", "name_form_groups", "name_sphere_groups",
            "name_trend_groups", "sign_name"
        ]
        for (var i = 0; i < typeKeys.length; ++i) {
            var key = typeKeys[i]
            if (payload[key] !== undefined) {
                var label = key === "sign_name" ? "Знак"
                          : key === "rank_name" ? "Звание"
                          : key === "type_name" ? "Тип"
                          : "Подтип"
                add(label, payload[key])
            }
        }

        // 5. Описание / резюме.
        for (var k3 in payload) {
            if (k3 === "description" || /^description_/.test(k3)) {
                add("Описание", payload[k3])
                break
            }
        }
        if (payload.resume_event !== undefined)
            add("Резюме", payload.resume_event)

        // 6. Персональные поля (если объект — Персоналия).
        if (payload.rank_persones !== undefined && payload.rank_name === undefined)
            add("Звание", payload.rank_persones)
        if (payload.birth_date !== undefined)
            add("Дата рождения", payload.birth_date)
        if (payload.birth_place !== undefined)
            add("Место рождения", payload.birth_place)
        if (payload.nationality !== undefined)
            add("Национальность", payload.nationality)

        // 7. СМИ-специфичные.
        if (payload.site_smi !== undefined)
            add("Сайт", payload.site_smi)
        if (payload.frequency_smi !== undefined)
            add("Частота", payload.frequency_smi)

        // 8. Комментарии — любое поле, содержащее "comment".
        for (var k4 in payload) {
            if (/comment/i.test(k4) && payload[k4]) {
                add("Комментарии", payload[k4])
                break
            }
        }

        return rows
    }

    function objectLegacyRows() {
        var rows = []
        if (!root.selectedObject)
            return rows

        function addRow(label, value) {
            if (value === null || value === undefined)
                return
            var text = String(value)
            if (text === "" || text === "0" || text === "0.0" || text === "NaN")
                return
            rows.push({ "label": label, "value": text })
        }

        addRow("ID объекта", root.selectedObject.objectId)
        addRow("Тип объекта", root.selectedObject.objectType)
        addRow("ID знака", root.selectedObject.idSign)
        addRow("Код знака", root.selectedObject.signKey)
        addRow("Семантика 17501", root.selectedObject.legacySemantic17501)
        addRow("Семантика 17502", root.selectedObject.legacySemantic17502)
        addRow("Семантика 105", root.selectedObject.legacySemantic105)
        addRow("Семантика 18", root.selectedObject.legacySemantic18)
        addRow("Семантика 19", root.selectedObject.legacySemantic19)
        addRow("Семантика 32811", root.selectedObject.legacySemantic32811)
        addRow("Семантика 32852", root.selectedObject.legacySemantic32852)
        return rows
    }

    function rebuildEventTypeOptions() {
        var options = [{ "idTypeEvent": 0, "label": "Все типы" }]
        var catalog = root.appState && root.appState.eventTypeCatalog ? root.appState.eventTypeCatalog : []
        for (var i = 0; i < catalog.length; ++i) {
            var row = catalog[i]
            var idTypeEvent = Number(row.idTypeEvent || 0)
            if (idTypeEvent <= 0)
                continue
            options.push({
                "idTypeEvent": idTypeEvent,
                "label": String(row.typeName || ("Тип #" + idTypeEvent))
            })
        }
        root.eventTypeOptions = options
        if (root.appState && Number(root.appState.eventTypeFilterId || 0) > 0) {
            var exists = false
            for (var k = 0; k < options.length; ++k) {
                if (Number(options[k].idTypeEvent) === Number(root.appState.eventTypeFilterId)) {
                    exists = true
                    break
                }
            }
            if (!exists)
                root.appState.eventTypeFilterId = 0
        }
    }

    function eventDetailsRows() {
        var rows = []
        if (!root.selectedEvent)
            return rows
        var detailsPayload = root.selectedEvent.details || ({})
        appendObjectRows(rows, "", detailsPayload, 0)
        return rows
    }

    function eventLegacyRows() {
        var rows = []
        if (!root.selectedEvent)
            return rows

        function addRow(label, value) {
            if (value === null || value === undefined)
                return
            var text = String(value)
            if (text === "")
                return
            rows.push({ "label": label, "value": text })
        }

        var details = root.selectedEvent.details || ({})
        var objects = details.objects || []
        var coordinates = details.coordinates || []
        var media = details.media || []
        var initiators = 0
        var participants = 0
        for (var i = 0; i < objects.length; ++i) {
            if (objects[i] && objects[i].isEventSource)
                initiators += 1
            else
                participants += 1
        }

        addRow("ID события", root.selectedEvent.id)
        addRow("ID типа", root.selectedEvent.idTypeEvent)
        addRow("ID статуса", root.selectedEvent.idEventStatus)
        addRow("ID знака", root.selectedEvent.idSign)
        addRow("Код знака", root.selectedEvent.signKey)
        addRow("Инициаторов", initiators)
        addRow("Участников", participants)
        addRow("Координат", coordinates.length)
        addRow("Медиа", media.length)
        return rows
    }

    function syncSymbolDraft() {
        symbolDraftName = selectedSymbol && selectedSymbol.name ? selectedSymbol.name : ""
        symbolDraftGlyph = selectedSymbol && selectedSymbol.glyph ? selectedSymbol.glyph : "●"
        symbolDraftFillColor = selectedSymbol && selectedSymbol.fillColor ? selectedSymbol.fillColor : Theme.friendly
        symbolDraftTopColor = selectedSymbol && selectedSymbol.topColor ? selectedSymbol.topColor : Theme.friendlyLight
        symbolDraftInnerOpacity = selectedSymbol && selectedSymbol.innerOpacity !== undefined
            ? selectedSymbol.innerOpacity
            : 0.82
    }

    function commitSymbolDraft() {
        if (!symbolAgent || !selectedSymbol || !selectedSymbol.id)
            return
        symbolAgent.updateSelectedSymbol(symbolDraftName,
                                         symbolDraftGlyph,
                                         symbolDraftFillColor,
                                         symbolDraftTopColor,
                                         symbolDraftInnerOpacity)
    }

    Component.onCompleted: {
        syncSymbolDraft()
        rebuildEventTypeOptions()
        if (root.appState)
            root.currentTab = root.tabNameByIndex(root.appState.activeInspectorTab)
    }

    Connections {
        target: root.appState

        function onSelectionTypeChanged() {
            if (!root.appState)
                return
            if (root.appState.selectionType === "event")
                root.currentTab = "events"
            else if (root.appState.selectionType === "structure")
                root.currentTab = "inspector"
            else if (root.appState.selectionType === "object" || root.appState.selectionType === "multi")
                root.currentTab = "inspector"
        }

        function onActiveInspectorTabChanged() {
            if (!root.appState)
                return
            root.currentTab = root.tabNameByIndex(root.appState.activeInspectorTab)
        }

        function onEventTypeCatalogChanged() {
            root.rebuildEventTypeOptions()
        }
    }

    Connections {
        target: root.symbolAgent

        function onSelectedSymbolChanged() {
            root.syncSymbolDraft()
        }
    }

    onPanelStateChanged: {
        if (panelState && panelState.collapsed !== undefined)
            uiCollapsed = panelState.collapsed === true
    }

    onEffectivePanelStateChanged: {
        if (effectivePanelState && effectivePanelState.collapsed !== undefined)
            uiCollapsed = effectivePanelState.collapsed === true
    }

    GlassPanel {
        anchors.fill: parent
        radius: 22
        padding: 0
        backdropSource: root.backdropSource
        surfaceColor: Theme.surface
        surfaceOpacity: 1.0
        shadowOpacity: 0.06
        highlightOpacity: 0.04
        edgeOpacity: 0.05

        Item {
            anchors.fill: parent

            Row {
                visible: !root.collapsed
                anchors.left: parent.left
                anchors.right: hidePanelButton.left
                anchors.rightMargin: 8
                anchors.top: parent.top
                anchors.margins: 12
                height: 30
                spacing: 6

                Repeater {
                    model: [
                        { "id": "inspector", "label": "Инспектор" },
                        { "id": "events", "label": "События" }
                    ]

                    delegate: Rectangle {
                        id: tabButton

                        required property var modelData

                        height: 28
                        radius: 14
                        width: tabText.implicitWidth + 16
                        color: root.currentTab === tabButton.modelData.id ? Theme.ink(0.08) : Theme.ink(0.025)
                        border.width: 1
                        border.color: root.currentTab === tabButton.modelData.id ? Theme.ink(0.08) : Theme.ink(0.03)

                        Text {
                            id: tabText
                            anchors.centerIn: parent
                            text: tabButton.modelData.label
                            color: root.currentTab === tabButton.modelData.id ? Theme.ink(0.90) : Theme.ink(0.62)
                            font.pixelSize: 12
                            font.weight: root.currentTab === tabButton.modelData.id ? Font.Medium : Font.Normal
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: root.openTab(tabButton.modelData.id)
                        }
                    }
                }

                Item {
                    id: dragZone
                    width: Math.max(0, parent.width - x - 6)
                    height: parent.height

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.OpenHandCursor
                        preventStealing: true
                        acceptedButtons: Qt.LeftButton
                        property real dragOffsetX: 0
                        property real dragOffsetY: 0

                        onPressed: function(mouse) {
                            if (!root.panelManager)
                                return
                            dragOffsetX = mouse.x + dragZone.x
                            dragOffsetY = mouse.y + dragZone.y
                            root.panelManager.beginDrag("right-sidebar")
                        }

                        onPositionChanged: function(mouse) {
                            if (!pressed || !root.panelManager)
                                return
                            var targetItem = root.dragSurface ? root.dragSurface : root.parent
                            var point = dragZone.mapToItem(targetItem, mouse.x, mouse.y)
                            root.panelManager.updateDrag("right-sidebar",
                                                         point.x - dragOffsetX,
                                                         point.y - dragOffsetY)
                        }

                        onReleased: {
                            if (root.panelManager)
                                root.panelManager.endDrag("right-sidebar")
                        }
                    }
                }

                Item {
                    width: 2
                    height: 1
                }
            }

            Rectangle {
                id: hidePanelButton
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.topMargin: 12
                anchors.rightMargin: 12
                width: 28
                height: 28
                radius: 12
                z: 5
                color: hideButtonMouse.pressed
                    ? Theme.ink(0.12)
                    : hideButtonMouse.containsMouse
                        ? Theme.ink(0.08)
                        : Theme.ink(0.06)

                Text {
                    anchors.centerIn: parent
                    text: root.collapsed ? ">" : "<"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: Theme.textLight2
                    font.pixelSize: 14
                    font.weight: Font.DemiBold
                }

                MouseArea {
                    id: hideButtonMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: root.toggleCollapsed()
                }
            }

            Column {
                visible: root.collapsed
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 12
                spacing: 8

                Text {
                    text: "Инспектор"
                    color: Theme.ink(0.76)
                    font.pixelSize: 12
                    font.weight: Font.Medium
                    horizontalAlignment: Text.AlignHCenter
                    width: parent.width
                    elide: Text.ElideRight
                }

                Button {
                    id: collapsedInspectorButton
                    text: "Объекты"
                    flat: true
                    implicitWidth: 96
                    implicitHeight: 28
                    background: Rectangle {
                        radius: 14
                        color: Theme.white
                        opacity: collapsedInspectorButton.down ? 0.10 : collapsedInspectorButton.hovered ? 0.06 : 0.04
                    }
                    contentItem: Text {
                        text: collapsedInspectorButton.text
                        color: Theme.textSoft3
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: root.openTab("inspector")
                }

                Button {
                    id: collapsedEventsButton
                    text: "События"
                    flat: true
                    implicitWidth: 96
                    implicitHeight: 28
                    background: Rectangle {
                        radius: 14
                        color: Theme.white
                        opacity: collapsedEventsButton.down ? 0.10 : collapsedEventsButton.hovered ? 0.06 : 0.04
                    }
                    contentItem: Text {
                        text: collapsedEventsButton.text
                        color: Theme.textSoft3
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: root.openTab("events")
                }

            }

            Loader {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.topMargin: root.collapsed ? 0 : 44
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                anchors.bottomMargin: 10
                active: true
                visible: !root.collapsed
                sourceComponent: root.currentTab === "events"
                    ? eventsTab
                    : inspectorTab
            }

            Rectangle {
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: 8
                color: "transparent"

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.SizeHorCursor
                    enabled: !root.collapsed
                    property real startMouseX: 0

                    onPressed: function(mouse) {
                        startMouseX = mouse.x
                        root.dragStartWidth = root.expandedWidth
                    }

                    onPositionChanged: function(mouse) {
                        if (!pressed || !root.panelManager)
                            return
                        var nextWidth = Math.max(320, root.dragStartWidth + (mouse.x - startMouseX))
                        root.panelManager.resizePanel("right-sidebar", nextWidth, root.height)
                    }
                }
            }
        }
    }

    Component {
        id: inspectorTab

        Column {
            spacing: 10

            SectionTitle {
                text: root.structureMode
                    ? "Выбранный узел"
                    : "Список объектов (" + (root.filteredObjects ? root.filteredObjects.length : 0) + ")"
            }

            Rectangle {
                width: parent.width
                implicitHeight: layersCardColumn.implicitHeight + 20
                radius: 18
                color: Theme.ink(0.018)
                border.width: 1
                border.color: Theme.ink(0.03)

                Column {
                    id: layersCardColumn
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 8

                    Text {
                        text: "Карта и слои"
                        color: Theme.ink(0.50)
                        font.pixelSize: 11
                        font.weight: Font.Medium
                    }

                    // Режим: точки vs тепловая карта (слой heat завязан на этот же переключатель).
                    Row {
                        spacing: 8
                        anchors.left: parent.left
                        anchors.right: parent.right

                        Text {
                            text: "Режим"
                            color: Theme.ink(0.55)
                            font.pixelSize: 11
                            width: 72
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Row {
                            spacing: 6
                            anchors.verticalCenter: parent.verticalCenter

                            FilterChip {
                                text: "Точки"
                                checked: root.appState && root.appState.mapMode === "point"
                                onClicked: {
                                    if (root.appState)
                                        root.appState.mapMode = "point"
                                }
                            }

                            FilterChip {
                                text: "Тепловая карта"
                                checked: root.appState && root.appState.mapMode === "heatmap"
                                onClicked: {
                                    if (root.appState)
                                        root.appState.mapMode = "heatmap"
                                }
                            }
                        }
                    }

                    Rectangle {
                        width: parent.width
                        height: 1
                        color: Theme.ink(0.05)
                    }

                    Row {
                        spacing: 8
                        anchors.left: parent.left
                        anchors.right: parent.right

                        Text {
                            text: "Видимость"
                            color: Theme.ink(0.55)
                            font.pixelSize: 11
                            width: 72
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Row {
                            spacing: 6
                            anchors.verticalCenter: parent.verticalCenter

                            FilterChip {
                                text: "Линии"
                                checked: root.appState ? root.appState.showCoverageLine : true
                                onClicked: root.toggleBooleanState("showCoverageLine")
                            }

                            FilterChip {
                                text: "Подписи"
                                checked: root.appState ? root.appState.showLabels : true
                                onClicked: root.toggleBooleanState("showLabels")
                            }
                        }
                    }

                    Rectangle {
                        width: parent.width
                        height: 1
                        color: Theme.ink(0.05)
                    }

                    Row {
                        spacing: 8
                        anchors.left: parent.left
                        anchors.right: parent.right

                        Text {
                            text: "Стиль"
                            color: Theme.ink(0.55)
                            font.pixelSize: 11
                            width: 72
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Row {
                            spacing: 6
                            anchors.verticalCenter: parent.verticalCenter

                            FilterChip {
                                text: "MapTiler Basic"
                                checked: root.appState && root.appState.mapStyleName === "maptiler-basic"
                                onClicked: {
                                    if (root.appState)
                                        root.appState.mapStyleName = "maptiler-basic"
                                }
                            }

                            FilterChip {
                                text: "OSM Bright"
                                checked: root.appState && root.appState.mapStyleName === "osm-bright"
                                onClicked: {
                                    if (root.appState)
                                        root.appState.mapStyleName = "osm-bright"
                                }
                            }
                        }
                    }
                }
            }

            Rectangle {
                width: parent.width
                height: Math.max(170, parent.height * 0.38)
                radius: 18
                color: Theme.ink(0.018)
                border.width: 1
                border.color: Theme.ink(0.03)

                ListView {
                    anchors.fill: parent
                    anchors.margins: 8
                    clip: true
                    spacing: 4
                    model: root.structureMode
                        ? (root.selectedStructureNode && root.selectedStructureNode.id ? [root.selectedStructureNode] : [])
                        : root.filteredObjects
                    boundsBehavior: Flickable.StopAtBounds

                    delegate: Rectangle {
                        id: objectRow

                        required property var modelData

                        readonly property bool selected: root.structureMode
                            ? true
                            : root.selectionAgent && root.selectionAgent.isSelected(objectRow.modelData)

                        width: ListView.view.width
                        height: 48
                        radius: 14
                        color: objectRow.selected ? Theme.ink(0.06) : Theme.ink(0.02)
                        border.width: 1
                        border.color: objectRow.selected ? Theme.ink(0.09) : Theme.ink(0.028)

                        Row {
                            anchors.fill: parent
                            anchors.leftMargin: 10
                            anchors.rightMargin: 10
                            spacing: 8

                            Rectangle {
                                anchors.verticalCenter: parent.verticalCenter
                                width: 8
                                height: 8
                                radius: 4
                                color: root.structureMode ? Theme.structureDot : root.agentHub.symbolAgent.markerColor(objectRow.modelData)
                            }

                            Column {
                                anchors.verticalCenter: parent.verticalCenter
                                width: Math.max(0, parent.width - 26)
                                spacing: 2

                                Text {
                                    width: parent.width
                                    text: root.structureMode
                                        ? (objectRow.modelData.label || "Узел структуры")
                                        : objectRow.modelData.name
                                    color: objectRow.selected ? Theme.ink(0.92) : Theme.ink(0.76)
                                    font.pixelSize: 13
                                    font.weight: objectRow.selected ? Font.Medium : Font.Normal
                                    elide: Text.ElideRight
                                }

                                Text {
                                    width: parent.width
                                    text: root.structureMode
                                        ? ("Путь: " + (root.selectedStructurePath || "—"))
                                        : (root.objectTypeLabel(objectRow.modelData.objectType) || "Объект") + " · МППС " + objectRow.modelData.mpps
                                    color: Theme.ink(0.44)
                                    font.pixelSize: 11
                                    elide: Text.ElideRight
                                }
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                if (root.structureMode)
                                    return
                                var item = objectRow.modelData
                                // Центрируем карту: appState.center* → MyMapView.map.center (one-way binding).
                                if (root.appState && item) {
                                    var lat = Number(item.lat)
                                    var lon = Number(item.lon)
                                    if (isFinite(lat) && isFinite(lon)) {
                                        root.appState.centerLat = lat
                                        root.appState.centerLon = lon
                                    }
                                }
                                // Выбор объекта (триггерит loadObjectDetailsForObject → таблица "Детали").
                                if (root.selectionAgent)
                                    root.selectionAgent.selectObject(item)
                            }
                        }
                    }

                    ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded; width: 6 }
                }
            }

            SectionTitle { text: "Детали" }

            Rectangle {
                width: parent.width
                height: Math.max(0, parent.height - y)
                radius: 18
                color: Theme.ink(0.018)
                border.width: 1
                border.color: Theme.ink(0.03)

                Flickable {
                    anchors.fill: parent
                    anchors.margins: 10
                    clip: true
                    contentWidth: width
                    contentHeight: objectDetails.implicitHeight
                    boundsBehavior: Flickable.StopAtBounds

                    Column {
                        id: objectDetails

                        width: parent.width
                        spacing: 10

                        Text {
                            width: parent.width
                            text: root.structureMode
                                ? (root.selectedStructureNode && root.selectedStructureNode.label ? root.selectedStructureNode.label : "Выберите узел структуры")
                                : (root.selectedObject && root.selectedObject.name ? root.selectedObject.name : "Выберите объект")
                            color: Theme.ink(0.92)
                            font.pixelSize: 16
                            font.weight: Font.Medium
                            elide: Text.ElideRight
                        }

                        Text {
                            width: parent.width
                            text: root.structureMode
                                ? "Узел структуры и связанные объекты"
                                : "Данные объекта из БД (табличный режим)"
                            color: Theme.ink(0.52)
                            font.pixelSize: 12
                            wrapMode: Text.WordWrap
                        }

                        InfoField {
                            visible: root.structureMode
                            label: "Путь"
                            value: root.selectedStructurePath || "—"
                        }
                        InfoField {
                            visible: root.structureMode
                            label: "Тип"
                            value: root.structureKindLabel(root.selectedStructureNode)
                        }
                        InfoField {
                            visible: root.structureMode
                            label: "Дочерних узлов"
                            value: root.selectedStructureNode && root.selectedStructureNode.children
                                ? String(root.selectedStructureNode.children.length)
                                : "0"
                        }

                        Rectangle {
                            visible: !root.structureMode
                            width: parent.width
                            implicitHeight: tableHeader.implicitHeight + tableRows.implicitHeight + 12
                            radius: 14
                            color: Theme.ink(0.015)
                            border.width: 1
                            border.color: Theme.ink(0.03)

                            Column {
                                anchors.fill: parent
                                anchors.margins: 6
                                spacing: 0

                                Row {
                                    id: tableHeader
                                    width: parent.width
                                    height: 32

                                    Rectangle {
                                        width: Math.max(140, parent.width * 0.38)
                                        height: parent.height
                                        color: Theme.ink(0.06)

                                        Text {
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.left: parent.left
                                            anchors.leftMargin: 8
                                            text: "Поле"
                                            color: Theme.ink(0.80)
                                            font.pixelSize: 14
                                            font.weight: Font.Medium
                                        }
                                    }

                                    Rectangle {
                                        width: parent.width - (Math.max(140, parent.width * 0.38))
                                        height: parent.height
                                        color: Theme.ink(0.06)

                                        Text {
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.left: parent.left
                                            anchors.leftMargin: 8
                                            text: "Значение"
                                            color: Theme.ink(0.80)
                                            font.pixelSize: 14
                                            font.weight: Font.Medium
                                        }
                                    }
                                }

                                Column {
                                    id: tableRows
                                    width: parent.width
                                    spacing: 0

                                    Repeater {
                                        model: root.readableDetailsRows()

                                        delegate: Row {
                                            required property int index
                                            required property var modelData
                                            width: tableRows.width
                                            height: Math.max(30, valueText.implicitHeight + 8)

                                            Rectangle {
                                                width: Math.max(140, parent.width * 0.38)
                                                height: parent.height
                                                color: (index % 2 === 0) ? Theme.ink(0.025) : Theme.ink(0.01)

                                                Text {
                                                    anchors.verticalCenter: parent.verticalCenter
                                                    anchors.left: parent.left
                                                    anchors.leftMargin: 8
                                                    anchors.right: parent.right
                                                    anchors.rightMargin: 6
                                                    text: modelData.label
                                                    color: Theme.ink(0.74)
                                                    font.pixelSize: 13
                                                    elide: Text.ElideRight
                                                }
                                            }

                                            Rectangle {
                                                width: parent.width - (Math.max(140, parent.width * 0.38))
                                                height: parent.height
                                                color: (index % 2 === 0) ? Theme.ink(0.025) : Theme.ink(0.01)

                                                Text {
                                                    id: valueText
                                                    anchors.verticalCenter: parent.verticalCenter
                                                    anchors.left: parent.left
                                                    anchors.leftMargin: 8
                                                    anchors.right: parent.right
                                                    anchors.rightMargin: 6
                                                    text: modelData.value
                                                    color: Theme.ink(0.92)
                                                    font.pixelSize: 13
                                                    wrapMode: Text.WordWrap
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        Text {
                            width: parent.width
                            text: root.structureMode
                                ? "Выберите объект на карте или в структуре для перехода к детальным параметрам."
                                : (root.readableDetailsRows().length === 0 ? "Нет данных БД для выбранного объекта." : "")
                            visible: text.length > 0
                            color: Theme.ink(0.60)
                            font.pixelSize: 12
                            lineHeight: 1.24
                            wrapMode: Text.WordWrap
                        }
                    }

                    ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded; width: 6 }
                }
            }
        }
    }

    Component {
        id: eventsTab
        EventsTab { host: root }
    }

    Component {
        id: symbolsTab

        Column {
            spacing: 10

            SectionTitle { text: "Библиотека символов" }

            Rectangle {
                width: parent.width
                height: Math.max(150, parent.height * 0.34)
                radius: 18
                color: Theme.ink(0.018)
                border.width: 1
                border.color: Theme.ink(0.03)

                Column {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 8

                    Text {
                        width: parent.width
                        text: root.symbolAgent ? root.symbolTypeLabel(root.symbolAgent.selectedTypePath) : "Тип не выбран"
                        color: Theme.ink(0.56)
                        font.pixelSize: 12
                        elide: Text.ElideRight
                    }

                    ListView {
                        width: parent.width
                        height: parent.height - y
                        clip: true
                        spacing: 4
                        model: root.symbolList

                        delegate: Rectangle {
                            id: symbolRow

                            required property var modelData

                            readonly property bool selected: root.symbolAgent && root.symbolAgent.selectedSymbolId === symbolRow.modelData.id

                            width: ListView.view.width
                            height: 46
                            radius: 14
                            color: symbolRow.selected ? Theme.ink(0.06) : Theme.ink(0.02)
                            border.width: 1
                            border.color: symbolRow.selected ? Theme.ink(0.09) : Theme.ink(0.028)

                            Row {
                                anchors.fill: parent
                                anchors.leftMargin: 10
                                anchors.rightMargin: 10
                                spacing: 8

                                Rectangle {
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: 18
                                    height: 18
                                    radius: 9
                                    gradient: Gradient {
                                        GradientStop { position: 0.0; color: symbolRow.modelData.topColor }
                                        GradientStop { position: 1.0; color: symbolRow.modelData.fillColor }
                                    }
                                }

                                Column {
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: Math.max(0, parent.width - 26)
                                    spacing: 2

                                    Text {
                                        width: parent.width
                                        text: symbolRow.modelData.name
                                        color: symbolRow.selected ? Theme.ink(0.92) : Theme.ink(0.76)
                                        font.pixelSize: 13
                                        font.weight: symbolRow.selected ? Font.Medium : Font.Normal
                                        elide: Text.ElideRight
                                    }

                                    Text {
                                        width: parent.width
                                        text: symbolRow.modelData.glyph + " · " + symbolRow.modelData.fillColor
                                        color: Theme.ink(0.44)
                                        font.pixelSize: 11
                                        elide: Text.ElideRight
                                    }
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: root.symbolAgent.selectedSymbolId = symbolRow.modelData.id
                            }
                        }
                    }
                }
            }

            SectionTitle { text: "Детали" }

            Rectangle {
                width: parent.width
                height: Math.max(0, parent.height - y)
                radius: 18
                color: Theme.ink(0.018)
                border.width: 1
                border.color: Theme.ink(0.03)

                Flickable {
                    anchors.fill: parent
                    anchors.margins: 10
                    clip: true
                    contentWidth: width
                    contentHeight: symbolDetails.implicitHeight
                    boundsBehavior: Flickable.StopAtBounds

                    Column {
                        id: symbolDetails

                        width: parent.width
                        spacing: 10

                        Row {
                            width: parent.width
                            spacing: 10

                            Rectangle {
                                width: 62
                                height: 62
                                radius: 18
                                color: Theme.ink(0.025)
                                border.width: 1
                                border.color: Theme.ink(0.04)

                                Rectangle {
                                    anchors.centerIn: parent
                                    width: 24
                                    height: 24
                                    radius: 12
                                    gradient: Gradient {
                                        GradientStop { position: 0.0; color: root.symbolDraftTopColor || Theme.friendlyLight }
                                        GradientStop { position: 1.0; color: root.symbolPreviewColor() }
                                    }
                                }

                                Text {
                                    anchors.centerIn: parent
                                    text: root.symbolDraftGlyph || "●"
                                    color: Theme.ink(0.86)
                                    font.pixelSize: 14
                                }
                            }

                            Column {
                                width: parent.width - 72
                                spacing: 3

                                Text {
                                    width: parent.width
                                    text: root.symbolDraftName || "Выберите символ"
                                    color: Theme.ink(0.92)
                                    font.pixelSize: 15
                                    font.weight: Font.Medium
                                    elide: Text.ElideRight
                                }

                                Text {
                                    width: parent.width
                                    text: root.symbolAgent ? root.symbolTypeLabel(root.symbolAgent.selectedTypePath) : "—"
                                    color: Theme.ink(0.52)
                                    font.pixelSize: 12
                                    elide: Text.ElideRight
                                }
                            }
                        }

                        EditField {
                            width: parent.width
                            label: "Название"
                            value: root.symbolDraftName
                            onValueEdited: function(nextValue) {
                                root.symbolDraftName = nextValue
                                root.commitSymbolDraft()
                            }
                        }

                        Row {
                            width: parent.width
                            spacing: 8

                            EditField {
                                width: parent.width * 0.28
                                label: "Знак"
                                value: root.symbolDraftGlyph
                                onValueEdited: function(nextValue) {
                                    root.symbolDraftGlyph = nextValue.length > 0 ? nextValue[0] : "●"
                                    root.commitSymbolDraft()
                                }
                            }

                            EditField {
                                width: parent.width * 0.34 - 4
                                label: "Заливка"
                                value: root.symbolDraftFillColor
                                onValueEdited: function(nextValue) {
                                    root.symbolDraftFillColor = nextValue
                                    root.commitSymbolDraft()
                                }
                            }

                            EditField {
                                width: parent.width * 0.38 - 4
                                label: "Верх"
                                value: root.symbolDraftTopColor
                                onValueEdited: function(nextValue) {
                                    root.symbolDraftTopColor = nextValue
                                    root.commitSymbolDraft()
                                }
                            }
                        }

                        Rectangle {
                            width: parent.width
                            implicitHeight: 50
                            radius: 16
                            color: Theme.ink(0.022)
                            border.width: 1
                            border.color: Theme.ink(0.03)

                            Column {
                                anchors.fill: parent
                                anchors.margins: 10
                                spacing: 4

                                Text {
                                    text: "Внутренняя непрозрачность"
                                    color: Theme.ink(0.48)
                                    font.pixelSize: 11
                                }

                                Slider {
                                    width: parent.width
                                    from: 0.2
                                    to: 1.0
                                    value: root.symbolDraftInnerOpacity
                                    onMoved: {
                                        root.symbolDraftInnerOpacity = value
                                        root.commitSymbolDraft()
                                    }
                                }
                            }
                        }
                    }

                    ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded; width: 6 }
                }
            }
        }
    }
}
