pragma ComponentBehavior: Bound
import QtQuick 2.15
import QtQuick.Controls 2.15

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

    readonly property bool panelVisible: appState && appState.inspectorVisible !== undefined
        ? appState.inspectorVisible
        : (panelState && panelState.visible !== undefined ? panelState.visible : true)
    readonly property bool collapsed: panelState && panelState.collapsed ? panelState.collapsed : false
    readonly property real expandedWidth: panelState && panelState.width ? panelState.width : 372
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
    readonly property var eventTypeOptions: [
        { "value": "all", "label": "Все типы" },
        { "value": "operations", "label": "Операции" },
        { "value": "communications", "label": "Связь" },
        { "value": "monitoring", "label": "Наблюдение" },
        { "value": "logistics", "label": "Логистика" },
        { "value": "infrastructure", "label": "Инфраструктура" }
    ]
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

    onCurrentTabChanged: {
        if (agentHub && agentHub.uiStateAgent)
            agentHub.uiStateAgent.setInspectorTab(currentTab === "events" ? 1 : currentTab === "symbols" ? 2 : 0)
    }

    function openTab(tabName) {
        currentTab = tabName
        if (agentHub && agentHub.uiStateAgent) {
            agentHub.uiStateAgent.setInspectorVisible(true)
            agentHub.uiStateAgent.setActivePanel(1)
            agentHub.uiStateAgent.setInspectorTab(tabName === "events" ? 1 : tabName === "symbols" ? 2 : 0)
        }
    }

    function closePanel() {
        if (agentHub && agentHub.uiStateAgent)
            agentHub.uiStateAgent.setInspectorVisible(false)
    }

    function toggleCollapsed() {
        if (panelManager)
            panelManager.toggleCollapsed("right-sidebar")
    }

    function statusColor(statusValue) {
        switch (statusValue) {
        case "actual":
            return Qt.rgba(0.50, 0.72, 1.0, 1.0)
        case "planned":
            return Qt.rgba(0.58, 0.66, 1.0, 1.0)
        case "past":
            return Qt.rgba(0.58, 0.63, 0.68, 1.0)
        case "cancelled":
            return Qt.rgba(0.71, 0.56, 0.58, 1.0)
        default:
            return Qt.rgba(0.56, 0.64, 0.73, 1.0)
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
        case "operations":
            return "◎"
        case "communications":
            return "≈"
        case "monitoring":
            return "◈"
        case "logistics":
            return "⇄"
        case "infrastructure":
            return "▣"
        default:
            return "⌘"
        }
    }

    function typeLabel(typeValue) {
        switch (typeValue) {
        case "operations":
            return "Операции"
        case "communications":
            return "Связь"
        case "monitoring":
            return "Наблюдение"
        case "logistics":
            return "Логистика"
        case "infrastructure":
            return "Инфраструктура"
        default:
            return "Событие"
        }
    }

    function symbolPreviewColor() {
        return symbolDraftFillColor || "#7298f6"
    }

    function tabNameByIndex(index) {
        return index === 1 ? "events" : index === 2 ? "symbols" : "inspector"
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

    function syncSymbolDraft() {
        symbolDraftName = selectedSymbol && selectedSymbol.name ? selectedSymbol.name : ""
        symbolDraftGlyph = selectedSymbol && selectedSymbol.glyph ? selectedSymbol.glyph : "●"
        symbolDraftFillColor = selectedSymbol && selectedSymbol.fillColor ? selectedSymbol.fillColor : "#7298f6"
        symbolDraftTopColor = selectedSymbol && selectedSymbol.topColor ? selectedSymbol.topColor : "#8DB7FF"
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
    }

    Connections {
        target: root.symbolAgent

        function onSelectedSymbolChanged() {
            root.syncSymbolDraft()
        }
    }

    GlassPanel {
        anchors.fill: parent
        radius: 22
        padding: 0
        backdropSource: root.backdropSource
        surfaceColor: "#141c24"
        surfaceOpacity: 0.50
        shadowOpacity: 0.06
        highlightOpacity: 0.04
        edgeOpacity: 0.05

        Item {
            anchors.fill: parent

            Row {
                visible: !root.collapsed
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: 12
                height: 30
                spacing: 6

                Repeater {
                    model: [
                        { "id": "inspector", "label": "Инспектор" },
                        { "id": "events", "label": "События" },
                        { "id": "symbols", "label": "Символы" }
                    ]

                    delegate: Rectangle {
                        id: tabButton

                        required property var modelData

                        height: 28
                        radius: 14
                        width: tabText.implicitWidth + 16
                        color: root.currentTab === tabButton.modelData.id ? Qt.rgba(1, 1, 1, 0.08) : Qt.rgba(1, 1, 1, 0.025)
                        border.width: 1
                        border.color: root.currentTab === tabButton.modelData.id ? Qt.rgba(1, 1, 1, 0.08) : Qt.rgba(1, 1, 1, 0.03)

                        Text {
                            id: tabText
                            anchors.centerIn: parent
                            text: tabButton.modelData.label
                            color: root.currentTab === tabButton.modelData.id ? Qt.rgba(1, 1, 1, 0.90) : Qt.rgba(1, 1, 1, 0.62)
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
                    width: Math.max(0, parent.width - 232)
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

                HeaderButton {
                    label: root.collapsed ? "‹" : "›"
                    rotationValue: root.collapsed ? 0 : 180
                    onClicked: root.toggleCollapsed()
                }

                HeaderButton {
                    label: "×"
                    onClicked: root.closePanel()
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
                    color: Qt.rgba(1, 1, 1, 0.76)
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
                        color: "#ffffff"
                        opacity: collapsedInspectorButton.down ? 0.10 : collapsedInspectorButton.hovered ? 0.06 : 0.04
                    }
                    contentItem: Text {
                        text: collapsedInspectorButton.text
                        color: "#dce4ed"
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
                        color: "#ffffff"
                        opacity: collapsedEventsButton.down ? 0.10 : collapsedEventsButton.hovered ? 0.06 : 0.04
                    }
                    contentItem: Text {
                        text: collapsedEventsButton.text
                        color: "#dce4ed"
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: root.openTab("events")
                }

                Button {
                    id: collapsedSymbolsButton
                    text: "Знаки"
                    flat: true
                    implicitWidth: 96
                    implicitHeight: 28
                    background: Rectangle {
                        radius: 14
                        color: "#ffffff"
                        opacity: collapsedSymbolsButton.down ? 0.10 : collapsedSymbolsButton.hovered ? 0.06 : 0.04
                    }
                    contentItem: Text {
                        text: collapsedSymbolsButton.text
                        color: "#dce4ed"
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: root.openTab("symbols")
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
                    : root.currentTab === "symbols"
                        ? symbolsTab
                        : inspectorTab
            }

            Rectangle {
                anchors.left: parent.left
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
                        var nextWidth = Math.max(320, root.dragStartWidth - (mouse.x - startMouseX))
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

            SectionTitle { text: root.structureMode ? "Выбранный узел" : "Список объектов" }

            Rectangle {
                width: parent.width
                height: 122
                radius: 18
                color: Qt.rgba(1, 1, 1, 0.018)
                border.width: 1
                border.color: Qt.rgba(1, 1, 1, 0.03)

                Column {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 8

                    Text {
                        text: "Карта и слои"
                        color: Qt.rgba(1, 1, 1, 0.50)
                        font.pixelSize: 11
                        font.weight: Font.Medium
                    }

                    Row {
                        spacing: 6

                        FilterChip {
                            text: "Точки"
                            checked: root.appState && root.appState.mapMode === "point"
                            onClicked: root.appState.mapMode = "point"
                        }

                        FilterChip {
                            text: "Heatmap"
                            checked: root.appState && root.appState.mapMode === "heatmap"
                            onClicked: root.appState.mapMode = "heatmap"
                        }
                    }

                    Row {
                        spacing: 6

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

                        FilterChip {
                            text: "Heat слой"
                            checked: root.appState ? root.appState.showHeatmapLayer : true
                            onClicked: root.toggleBooleanState("showHeatmapLayer")
                        }
                    }
                }
            }

            Rectangle {
                width: parent.width
                height: Math.max(170, parent.height * 0.38)
                radius: 18
                color: Qt.rgba(1, 1, 1, 0.018)
                border.width: 1
                border.color: Qt.rgba(1, 1, 1, 0.03)

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
                        color: objectRow.selected ? Qt.rgba(1, 1, 1, 0.06) : Qt.rgba(1, 1, 1, 0.02)
                        border.width: 1
                        border.color: objectRow.selected ? Qt.rgba(1, 1, 1, 0.09) : Qt.rgba(1, 1, 1, 0.028)

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
                                color: root.structureMode ? Qt.rgba(0.68, 0.76, 0.90, 0.90) : root.agentHub.symbolAgent.markerColor(objectRow.modelData)
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
                                    color: objectRow.selected ? Qt.rgba(1, 1, 1, 0.92) : Qt.rgba(1, 1, 1, 0.76)
                                    font.pixelSize: 13
                                    font.weight: objectRow.selected ? Font.Medium : Font.Normal
                                    elide: Text.ElideRight
                                }

                                Text {
                                    width: parent.width
                                    text: root.structureMode
                                        ? ("Путь: " + (root.selectedStructurePath || "—"))
                                        : (objectRow.modelData.kind === "relay" ? "узел связи" : objectRow.modelData.kind === "lbs" ? "линейный объект" : "подразделение") + " · МППС " + objectRow.modelData.mpps
                                    color: Qt.rgba(1, 1, 1, 0.44)
                                    font.pixelSize: 11
                                    elide: Text.ElideRight
                                }
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (root.structureMode)
                                    return
                                root.selectionAgent.selectObject(objectRow.modelData)
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
                color: Qt.rgba(1, 1, 1, 0.018)
                border.width: 1
                border.color: Qt.rgba(1, 1, 1, 0.03)

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
                            color: Qt.rgba(1, 1, 1, 0.92)
                            font.pixelSize: 16
                            font.weight: Font.Medium
                            elide: Text.ElideRight
                        }

                        Text {
                            width: parent.width
                            text: root.structureMode
                                ? "Узел структуры и связанные объекты"
                                : "Данные объекта из БД (табличный режим)"
                            color: Qt.rgba(1, 1, 1, 0.52)
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
                            color: Qt.rgba(1, 1, 1, 0.015)
                            border.width: 1
                            border.color: Qt.rgba(1, 1, 1, 0.03)

                            Column {
                                anchors.fill: parent
                                anchors.margins: 6
                                spacing: 0

                                Row {
                                    id: tableHeader
                                    width: parent.width
                                    height: 28

                                    Rectangle {
                                        width: Math.max(120, parent.width * 0.38)
                                        height: parent.height
                                        color: Qt.rgba(1, 1, 1, 0.06)

                                        Text {
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.left: parent.left
                                            anchors.leftMargin: 8
                                            text: "Поле"
                                            color: Qt.rgba(1, 1, 1, 0.80)
                                            font.pixelSize: 12
                                            font.weight: Font.Medium
                                        }
                                    }

                                    Rectangle {
                                        width: parent.width - (Math.max(120, parent.width * 0.38))
                                        height: parent.height
                                        color: Qt.rgba(1, 1, 1, 0.06)

                                        Text {
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.left: parent.left
                                            anchors.leftMargin: 8
                                            text: "Значение"
                                            color: Qt.rgba(1, 1, 1, 0.80)
                                            font.pixelSize: 12
                                            font.weight: Font.Medium
                                        }
                                    }
                                }

                                Column {
                                    id: tableRows
                                    width: parent.width
                                    spacing: 0

                                    Repeater {
                                        model: root.detailsRows()

                                        delegate: Row {
                                            required property int index
                                            required property var modelData
                                            width: tableRows.width
                                            height: 26

                                            Rectangle {
                                                width: Math.max(120, parent.width * 0.38)
                                                height: parent.height
                                                color: (index % 2 === 0) ? Qt.rgba(1, 1, 1, 0.025) : Qt.rgba(1, 1, 1, 0.01)

                                                Text {
                                                    anchors.verticalCenter: parent.verticalCenter
                                                    anchors.left: parent.left
                                                    anchors.leftMargin: 8
                                                    anchors.right: parent.right
                                                    anchors.rightMargin: 6
                                                    text: modelData.label
                                                    color: Qt.rgba(1, 1, 1, 0.62)
                                                    font.pixelSize: 11
                                                    elide: Text.ElideRight
                                                }
                                            }

                                            Rectangle {
                                                width: parent.width - (Math.max(120, parent.width * 0.38))
                                                height: parent.height
                                                color: (index % 2 === 0) ? Qt.rgba(1, 1, 1, 0.025) : Qt.rgba(1, 1, 1, 0.01)

                                                Text {
                                                    anchors.verticalCenter: parent.verticalCenter
                                                    anchors.left: parent.left
                                                    anchors.leftMargin: 8
                                                    anchors.right: parent.right
                                                    anchors.rightMargin: 6
                                                    text: modelData.value
                                                    color: Qt.rgba(1, 1, 1, 0.78)
                                                    font.pixelSize: 11
                                                    elide: Text.ElideRight
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
                                : (root.detailsRows().length === 0 ? "Нет данных БД для выбранного объекта." : "")
                            visible: text.length > 0
                            color: Qt.rgba(1, 1, 1, 0.60)
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

        Column {
            spacing: 10

            SectionTitle { text: "Список событий" }

            Rectangle {
                width: parent.width
                height: 98
                radius: 18
                color: Qt.rgba(1, 1, 1, 0.018)
                border.width: 1
                border.color: Qt.rgba(1, 1, 1, 0.03)

                Column {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 6

                    Flow {
                        width: parent.width
                        spacing: 6

                        Repeater {
                            model: root.eventStatusOptions

                            delegate: FilterChip {
                                required property var modelData
                                text: modelData.label
                                checked: root.appState && root.appState.eventStatusFilter === modelData.value
                                onClicked: root.appState.eventStatusFilter = modelData.value
                            }
                        }
                    }

                    Flow {
                        width: parent.width
                        spacing: 6

                        Repeater {
                            model: root.eventTypeOptions

                            delegate: FilterChip {
                                required property var modelData
                                text: modelData.label
                                checked: root.appState && root.appState.eventTypeFilter === modelData.value
                                onClicked: root.appState.eventTypeFilter = modelData.value
                            }
                        }
                    }
                }
            }

            Rectangle {
                width: parent.width
                height: Math.max(170, parent.height * 0.38)
                radius: 18
                color: Qt.rgba(1, 1, 1, 0.018)
                border.width: 1
                border.color: Qt.rgba(1, 1, 1, 0.03)

                ListView {
                    anchors.fill: parent
                    anchors.margins: 8
                    clip: true
                    spacing: 4
                    model: root.filteredEvents
                    boundsBehavior: Flickable.StopAtBounds

                    delegate: Rectangle {
                        id: eventRow

                        required property var modelData

                        readonly property bool selected: root.selectionAgent && root.selectionAgent.isEventSelected(eventRow.modelData)

                        width: ListView.view.width
                        height: 54
                        radius: 14
                        color: eventRow.selected ? Qt.rgba(1, 1, 1, 0.06) : Qt.rgba(1, 1, 1, 0.02)
                        border.width: 1
                        border.color: eventRow.selected ? Qt.rgba(1, 1, 1, 0.09) : Qt.rgba(1, 1, 1, 0.028)

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
                                color: root.statusColor(eventRow.modelData.status)
                            }

                            Column {
                                anchors.verticalCenter: parent.verticalCenter
                                width: Math.max(0, parent.width - 26)
                                spacing: 2

                                Text {
                                    width: parent.width
                                    text: eventRow.modelData.name
                                    color: eventRow.selected ? Qt.rgba(1, 1, 1, 0.92) : Qt.rgba(1, 1, 1, 0.76)
                                    font.pixelSize: 13
                                    font.weight: eventRow.selected ? Font.Medium : Font.Normal
                                    elide: Text.ElideRight
                                }

                                Text {
                                    width: parent.width
                                    text: root.typeIcon(eventRow.modelData.type) + " " + root.typeLabel(eventRow.modelData.type) + " · " + root.statusLabel(eventRow.modelData.status)
                                    color: Qt.rgba(1, 1, 1, 0.44)
                                    font.pixelSize: 11
                                    elide: Text.ElideRight
                                }
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: root.selectionAgent.selectEvent(eventRow.modelData)
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
                color: Qt.rgba(1, 1, 1, 0.018)
                border.width: 1
                border.color: Qt.rgba(1, 1, 1, 0.03)

                Flickable {
                    anchors.fill: parent
                    anchors.margins: 10
                    clip: true
                    contentWidth: width
                    contentHeight: eventDetails.implicitHeight
                    boundsBehavior: Flickable.StopAtBounds

                    Column {
                        id: eventDetails

                        width: parent.width
                        spacing: 10

                        Text {
                            width: parent.width
                            text: root.selectedEvent && root.selectedEvent.name ? root.selectedEvent.name : "Выберите событие"
                            color: Qt.rgba(1, 1, 1, 0.92)
                            font.pixelSize: 16
                            font.weight: Font.Medium
                            elide: Text.ElideRight
                        }

                        Text {
                            width: parent.width
                            text: root.selectedEvent && root.selectedEvent.type
                                ? root.typeIcon(root.selectedEvent.type) + " " + root.typeLabel(root.selectedEvent.type) + " · " + root.statusLabel(root.selectedEvent.status)
                                : "Выберите событие выше для просмотра деталей"
                            color: Qt.rgba(1, 1, 1, 0.52)
                            font.pixelSize: 12
                            wrapMode: Text.WordWrap
                        }

                        InfoField { label: "Начало"; value: root.selectedEvent && root.selectedEvent.startTimestamp ? root.selectedEvent.startTimestamp : "—" }
                        InfoField { label: "Окончание"; value: root.selectedEvent && root.selectedEvent.endTimestamp ? root.selectedEvent.endTimestamp : "—" }
                        InfoField { label: "Обновлено"; value: root.selectedEvent && root.selectedEvent.updatedTimestamp ? root.selectedEvent.updatedTimestamp : "—" }
                        InfoField { label: "Связи"; value: root.selectedEvent && root.selectedEvent.objectIds ? String(root.selectedEvent.objectIds.length) + " объектов" : "—" }

                        Text {
                            width: parent.width
                            text: root.selectedEvent && root.selectedEvent.description ? root.selectedEvent.description : ""
                            visible: text.length > 0
                            color: Qt.rgba(1, 1, 1, 0.60)
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
        id: symbolsTab

        Column {
            spacing: 10

            SectionTitle { text: "Библиотека символов" }

            Rectangle {
                width: parent.width
                height: Math.max(150, parent.height * 0.34)
                radius: 18
                color: Qt.rgba(1, 1, 1, 0.018)
                border.width: 1
                border.color: Qt.rgba(1, 1, 1, 0.03)

                Column {
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 8

                    Text {
                        width: parent.width
                        text: root.symbolAgent ? root.symbolTypeLabel(root.symbolAgent.selectedTypePath) : "Тип не выбран"
                        color: Qt.rgba(1, 1, 1, 0.56)
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
                            color: symbolRow.selected ? Qt.rgba(1, 1, 1, 0.06) : Qt.rgba(1, 1, 1, 0.02)
                            border.width: 1
                            border.color: symbolRow.selected ? Qt.rgba(1, 1, 1, 0.09) : Qt.rgba(1, 1, 1, 0.028)

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
                                        color: symbolRow.selected ? Qt.rgba(1, 1, 1, 0.92) : Qt.rgba(1, 1, 1, 0.76)
                                        font.pixelSize: 13
                                        font.weight: symbolRow.selected ? Font.Medium : Font.Normal
                                        elide: Text.ElideRight
                                    }

                                    Text {
                                        width: parent.width
                                        text: symbolRow.modelData.glyph + " · " + symbolRow.modelData.fillColor
                                        color: Qt.rgba(1, 1, 1, 0.44)
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
                color: Qt.rgba(1, 1, 1, 0.018)
                border.width: 1
                border.color: Qt.rgba(1, 1, 1, 0.03)

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
                                color: Qt.rgba(1, 1, 1, 0.025)
                                border.width: 1
                                border.color: Qt.rgba(1, 1, 1, 0.04)

                                Rectangle {
                                    anchors.centerIn: parent
                                    width: 24
                                    height: 24
                                    radius: 12
                                    gradient: Gradient {
                                        GradientStop { position: 0.0; color: root.symbolDraftTopColor || "#8DB7FF" }
                                        GradientStop { position: 1.0; color: root.symbolPreviewColor() }
                                    }
                                }

                                Text {
                                    anchors.centerIn: parent
                                    text: root.symbolDraftGlyph || "●"
                                    color: Qt.rgba(1, 1, 1, 0.86)
                                    font.pixelSize: 14
                                }
                            }

                            Column {
                                width: parent.width - 72
                                spacing: 3

                                Text {
                                    width: parent.width
                                    text: root.symbolDraftName || "Выберите символ"
                                    color: Qt.rgba(1, 1, 1, 0.92)
                                    font.pixelSize: 15
                                    font.weight: Font.Medium
                                    elide: Text.ElideRight
                                }

                                Text {
                                    width: parent.width
                                    text: root.symbolAgent ? root.symbolTypeLabel(root.symbolAgent.selectedTypePath) : "—"
                                    color: Qt.rgba(1, 1, 1, 0.52)
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
                            color: Qt.rgba(1, 1, 1, 0.022)
                            border.width: 1
                            border.color: Qt.rgba(1, 1, 1, 0.03)

                            Column {
                                anchors.fill: parent
                                anchors.margins: 10
                                spacing: 4

                                Text {
                                    text: "Внутренняя непрозрачность"
                                    color: Qt.rgba(1, 1, 1, 0.48)
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

    component HeaderButton: Button {
        id: headerButton

        property string label: ""
        property real rotationValue: 0
        implicitWidth: 28
        implicitHeight: 28
        flat: true
        background: Rectangle {
            radius: 14
            color: "#ffffff"
            opacity: headerButton.down ? 0.08 : headerButton.hovered ? 0.05 : 0.03
        }
        contentItem: Text {
            text: headerButton.label
            rotation: headerButton.rotationValue
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: "#dce4ed"
            font.pixelSize: 15
        }
    }

    component SectionTitle: Text {
        color: Qt.rgba(1, 1, 1, 0.44)
        font.pixelSize: 11
        font.weight: Font.Medium
        font.letterSpacing: 0.4
    }

    component InfoField: Rectangle {
        id: infoField

        property string label: ""
        property string value: ""
        property bool accent: false

        implicitHeight: 34
        radius: 14
        color: Qt.rgba(1, 1, 1, 0.02)
        border.width: 1
        border.color: Qt.rgba(1, 1, 1, 0.03)

        Row {
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            spacing: 10

            Text {
                width: 82
                anchors.verticalCenter: parent.verticalCenter
                text: infoField.label
                color: Qt.rgba(1, 1, 1, 0.46)
                font.pixelSize: 11
                elide: Text.ElideRight
            }

            Text {
                width: Math.max(0, parent.width - 92)
                anchors.verticalCenter: parent.verticalCenter
                text: infoField.value
                color: infoField.accent ? Qt.rgba(1, 1, 1, 0.90) : Qt.rgba(1, 1, 1, 0.76)
                font.pixelSize: 12
                font.weight: infoField.accent ? Font.Medium : Font.Normal
                elide: Text.ElideRight
            }
        }
    }

    component EditField: Rectangle {
        id: editField

        property string label: ""
        property string value: ""
        signal valueEdited(string nextValue)

        implicitHeight: 48
        radius: 16
        color: Qt.rgba(1, 1, 1, 0.02)
        border.width: 1
        border.color: Qt.rgba(1, 1, 1, 0.03)

        Column {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 4

            Text {
                text: editField.label
                color: Qt.rgba(1, 1, 1, 0.46)
                font.pixelSize: 11
                font.weight: Font.Medium
            }

            TextField {
                width: parent.width
                text: editField.value
                color: Qt.rgba(1, 1, 1, 0.84)
                font.pixelSize: 12
                padding: 0
                background: Item {}
                onTextEdited: editField.valueEdited(text)
            }
        }
    }

    component FilterChip: Rectangle {
        id: filterChip
        property string text: ""
        property bool checked: false
        signal clicked()

        width: chipText.implicitWidth + 16
        height: 28
        radius: 14
        color: checked ? Qt.rgba(1, 1, 1, 0.10) : Qt.rgba(1, 1, 1, 0.03)
        border.width: 1
        border.color: checked ? Qt.rgba(1, 1, 1, 0.12) : Qt.rgba(1, 1, 1, 0.04)

        Text {
            id: chipText
            anchors.centerIn: parent
            text: filterChip.text
            color: filterChip.checked ? Qt.rgba(1, 1, 1, 0.90) : Qt.rgba(1, 1, 1, 0.62)
            font.pixelSize: 12
            font.weight: filterChip.checked ? Font.Medium : Font.Normal
        }

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: filterChip.clicked()
        }
    }
}
