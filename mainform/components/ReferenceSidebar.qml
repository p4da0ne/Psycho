pragma ComponentBehavior: Bound
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root

    property var panelManager
    property var structureAgent
    property var appState
    property var agentHub
    property var objects: []
    property Item backdropSource
    property Item dragSurface
    property var panelState: ({})

    readonly property bool panelVisible: panelState && panelState.visible !== undefined ? panelState.visible : true
    readonly property bool collapsed: panelState && panelState.collapsed ? panelState.collapsed : false
    readonly property real expandedWidth: panelState && panelState.width ? panelState.width : 284
    readonly property real targetWidth: panelVisible ? (collapsed ? 124 : expandedWidth) : 0
    readonly property string selectedPath: structureAgent ? structureAgent.selectedPath : ""
    readonly property var treeData: structureAgent ? structureAgent.referenceTree : []
    property var expandedMap: ({
        "countries": true,
        "countries/russia": true
    })
    property real dragStartWidth: 0

    implicitWidth: targetWidth
    width: targetWidth
    visible: width > 0.5
    clip: true

    Behavior on width {
        NumberAnimation { duration: 180 }
    }

    function isExpanded(path) {
        return !!expandedMap[path]
    }

    function toggleNode(path) {
        var nextMap = Object.assign({}, expandedMap)
        nextMap[path] = !nextMap[path]
        expandedMap = nextMap
    }

    function visibleNodes() {
        var output = []

        function walk(nodes, depth, prefix) {
            for (var i = 0; i < nodes.length; ++i) {
                var current = nodes[i]
                var path = prefix === "" ? current.id : prefix + "/" + current.id
                var children = current.children ? current.children : []
                var hasChildrenValue = children.length > 0
                output.push({
                    "label": current.label,
                    "path": path,
                    "depth": depth,
                    "hasChildren": hasChildrenValue,
                    "expanded": hasChildrenValue && root.isExpanded(path)
                })

                if (hasChildrenValue && root.isExpanded(path))
                    walk(children, depth + 1, path)
            }
        }

        walk(root.treeData || [], 0, "")
        return output
    }

    function objectCount(predicate) {
        var count = 0
        var items = root.objects || []
        for (var i = 0; i < items.length; ++i) {
            if (predicate(items[i]))
                count += 1
        }
        return count
    }

    function closePanel() {
        if (root.agentHub && root.agentHub.uiStateAgent)
            root.agentHub.uiStateAgent.setNavigationVisible(false)
    }

    function toggleCollapsed() {
        if (root.panelManager)
            root.panelManager.toggleCollapsed("reference-panel")
    }

    function selectGroup(groupKey) {
        if (!root.agentHub || !root.agentHub.uiStateAgent)
            return
        if (groupKey === "")
            root.agentHub.uiStateAgent.clearSelectedGroup()
        else
            root.agentHub.uiStateAgent.setSelectedGroup(groupKey)
    }

    Component.onCompleted: {
        if (root.appState
                && root.appState.selectedStructurePath === ""
                && root.structureAgent
                && root.structureAgent.selectedPath
                && root.agentHub
                && root.agentHub.uiStateAgent) {
            root.agentHub.uiStateAgent.syncSelectedStructurePath(root.structureAgent.selectedPath)
        }
    }

    Connections {
        target: root.appState

        function onSelectedStructurePathChanged() {
            if (!root.structureAgent || !root.appState)
                return
            if (root.structureAgent.selectedPath !== root.appState.selectedStructurePath)
                root.structureAgent.selectedPath = root.appState.selectedStructurePath
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

            Column {
                anchors.fill: parent
                anchors.leftMargin: 12
                anchors.rightMargin: 12
                anchors.topMargin: 12
                anchors.bottomMargin: 12
                spacing: 10

                Row {
                    width: parent.width
                    height: 28
                    spacing: 8

                    Text {
                        id: titleText
                        width: Math.max(0, parent.width - 68)
                        anchors.verticalCenter: parent.verticalCenter
                        text: "Навигация"
                        color: Qt.rgba(1, 1, 1, 0.90)
                        font.pixelSize: 12
                        font.weight: Font.Medium
                        elide: Text.ElideRight

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
                                dragOffsetX = mouse.x
                                dragOffsetY = mouse.y
                                root.panelManager.beginDrag("reference-panel")
                            }

                            onPositionChanged: function(mouse) {
                                if (!pressed || !root.panelManager)
                                    return
                                var targetItem = root.dragSurface ? root.dragSurface : root.parent
                                var point = titleText.mapToItem(targetItem, mouse.x, mouse.y)
                                root.panelManager.updateDrag("reference-panel",
                                                             point.x - dragOffsetX,
                                                             point.y - dragOffsetY)
                            }

                            onReleased: {
                                if (root.panelManager)
                                    root.panelManager.endDrag("reference-panel")
                            }
                        }
                    }

                    PanelButton {
                        label: root.collapsed ? "›" : "‹"
                        onClicked: root.toggleCollapsed()
                    }

                    PanelButton {
                        label: "×"
                        onClicked: root.closePanel()
                    }
                }

                Column {
                    visible: !root.collapsed
                    width: parent.width
                    spacing: 10

                    SectionTitle { text: "Быстрые фильтры" }

                    Column {
                        width: parent.width
                        spacing: 6

                        FilterRow {
                            width: parent.width
                            label: "Свои"
                            count: root.objectCount(function(item) { return item.side === "friendly" })
                            active: root.appState ? root.appState.showFriendly : false
                            onToggled: function(value) {
                                if (root.agentHub && root.agentHub.uiStateAgent)
                                    root.agentHub.uiStateAgent.setFilterState("friendly", value)
                            }
                        }

                        FilterRow {
                            width: parent.width
                            label: "Чужие"
                            count: root.objectCount(function(item) { return item.side === "foreign" })
                            active: root.appState ? root.appState.showForeign : false
                            onToggled: function(value) {
                                if (root.agentHub && root.agentHub.uiStateAgent)
                                    root.agentHub.uiStateAgent.setFilterState("foreign", value)
                            }
                        }

                        FilterRow {
                            width: parent.width
                            label: "Инфраструктура"
                            count: root.objectCount(function(item) { return item.kind === "lbs" })
                            active: root.appState ? root.appState.showBaseStations : false
                            onToggled: function(value) {
                                if (root.agentHub && root.agentHub.uiStateAgent)
                                    root.agentHub.uiStateAgent.setFilterState("infrastructure", value)
                            }
                        }
                    }

                    SectionTitle { text: "Группы объектов" }

                    Column {
                        width: parent.width
                        spacing: 6

                        GroupRow {
                            width: parent.width
                            label: "Подразделения"
                            meta: "Свои и чужие"
                            count: root.objectCount(function(item) { return item.kind === "unit" })
                            active: root.appState && root.appState.selectedGroup === "units"
                            onClicked: root.selectGroup(root.appState && root.appState.selectedGroup === "units" ? "" : "units")
                        }

                        GroupRow {
                            width: parent.width
                            label: "Узлы связи"
                            meta: "Связные точки"
                            count: root.objectCount(function(item) { return item.kind === "relay" })
                            active: root.appState && root.appState.selectedGroup === "relays"
                            onClicked: root.selectGroup(root.appState && root.appState.selectedGroup === "relays" ? "" : "relays")
                        }

                        GroupRow {
                            width: parent.width
                            label: "Линейные объекты"
                            meta: "Инфраструктурная линия"
                            count: root.objectCount(function(item) { return item.kind === "lbs" })
                            active: root.appState && root.appState.selectedGroup === "lbs"
                            onClicked: root.selectGroup(root.appState && root.appState.selectedGroup === "lbs" ? "" : "lbs")
                        }
                    }

                    SectionTitle { text: "Структура" }

                    ListView {
                        width: parent.width
                        height: Math.max(0, parent.height - y)
                        clip: true
                        spacing: 2
                        model: root.visibleNodes()
                        boundsBehavior: Flickable.StopAtBounds

                        delegate: Rectangle {
                            id: nodeRow

                            required property var modelData

                            readonly property bool selected: root.selectedPath === nodeRow.modelData.path

                            width: ListView.view.width
                            height: 30
                            radius: 15
                            color: nodeRow.selected ? Qt.rgba(1, 1, 1, 0.06) : Qt.rgba(1, 1, 1, 0.018)
                            border.width: 1
                            border.color: nodeRow.selected ? Qt.rgba(1, 1, 1, 0.09) : Qt.rgba(1, 1, 1, 0.025)

                            Row {
                                anchors.fill: parent
                                anchors.leftMargin: 10 + nodeRow.modelData.depth * 14
                                anchors.rightMargin: 10
                                spacing: 8

                                Text {
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: 10
                                    text: nodeRow.modelData.hasChildren ? (nodeRow.modelData.expanded ? "▾" : "▸") : ""
                                    color: Qt.rgba(1, 1, 1, 0.46)
                                    font.pixelSize: 10
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: Math.max(0, parent.width - 18)
                                    text: nodeRow.modelData.label
                                    color: nodeRow.selected ? Qt.rgba(1, 1, 1, 0.90) : Qt.rgba(1, 1, 1, 0.68)
                                    font.pixelSize: 10
                                    font.weight: nodeRow.selected ? Font.Medium : Font.Normal
                                    elide: Text.ElideRight
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: function(mouse) {
                                    if (nodeRow.modelData.hasChildren && mouse.x < 26 + nodeRow.modelData.depth * 14)
                                        root.toggleNode(nodeRow.modelData.path)
                                    if (root.agentHub && root.agentHub.uiStateAgent)
                                        root.agentHub.uiStateAgent.setSelectedStructurePath(nodeRow.modelData.path)
                                }
                            }
                        }

                        ScrollBar.vertical: ScrollBar {
                            policy: ScrollBar.AsNeeded
                            width: 6
                        }
                    }
                }

                Column {
                    visible: root.collapsed
                    width: parent.width
                    spacing: 8

                    Text {
                        width: parent.width
                        horizontalAlignment: Text.AlignHCenter
                        text: "Панель свернута"
                        color: Qt.rgba(1, 1, 1, 0.52)
                        font.pixelSize: 10
                        elide: Text.ElideRight
                    }

                    Button {
                        id: openCollapsedButton
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "Открыть"
                        flat: true
                        implicitWidth: 84
                        implicitHeight: 28
                        background: Rectangle {
                            radius: 14
                            color: "#ffffff"
                            opacity: openCollapsedButton.down ? 0.10 : openCollapsedButton.hovered ? 0.06 : 0.04
                        }
                        contentItem: Text {
                            text: openCollapsedButton.text
                            color: "#dce4ed"
                            font.pixelSize: 10
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        onClicked: root.toggleCollapsed()
                    }
                }
            }

            Rectangle {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
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
                        var nextWidth = Math.max(240, root.dragStartWidth + mouse.x - startMouseX)
                        root.panelManager.resizePanel("reference-panel", nextWidth, root.height)
                    }
                }
            }
        }
    }

    component PanelButton: Button {
        id: panelButton

        property string label: ""
        implicitWidth: 28
        implicitHeight: 28
        flat: true
        background: Rectangle {
            radius: 14
            color: "#ffffff"
            opacity: panelButton.down ? 0.08 : panelButton.hovered ? 0.05 : 0.03
        }
        contentItem: Text {
            text: panelButton.label
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: "#dce4ed"
            font.pixelSize: 13
        }
    }

    component SectionTitle: Text {
        color: Qt.rgba(1, 1, 1, 0.44)
        font.pixelSize: 9
        font.weight: Font.Medium
        font.letterSpacing: 0.4
    }

    component FilterRow: Rectangle {
        id: filterRow

        property string label: ""
        property int count: 0
        property bool active: false
        signal toggled(bool value)

        height: 30
        radius: 15
        color: Qt.rgba(1, 1, 1, 0.022)
        border.width: 1
        border.color: Qt.rgba(1, 1, 1, 0.03)

        Row {
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            spacing: 8

            Text {
                anchors.verticalCenter: parent.verticalCenter
                width: Math.max(0, parent.width - 68)
                text: filterRow.label
                color: Qt.rgba(1, 1, 1, 0.76)
                font.pixelSize: 10
                elide: Text.ElideRight
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: String(filterRow.count)
                color: Qt.rgba(1, 1, 1, 0.40)
                font.pixelSize: 10
            }

            Switch {
                anchors.verticalCenter: parent.verticalCenter
                checked: filterRow.active
                scale: 0.72
                onToggled: filterRow.toggled(checked)
            }
        }
    }

    component GroupRow: Rectangle {
        id: groupRow

        property string label: ""
        property string meta: ""
        property int count: 0
        property bool active: false
        signal clicked()

        height: 38
        radius: 16
        color: groupRow.active ? Qt.rgba(1, 1, 1, 0.06) : Qt.rgba(1, 1, 1, 0.02)
        border.width: 1
        border.color: groupRow.active ? Qt.rgba(1, 1, 1, 0.08) : Qt.rgba(1, 1, 1, 0.028)

        Row {
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            spacing: 10

            Column {
                anchors.verticalCenter: parent.verticalCenter
                width: Math.max(0, parent.width - 40)
                spacing: 1

                Text {
                    width: parent.width
                    text: groupRow.label
                    color: groupRow.active ? Qt.rgba(1, 1, 1, 0.88) : Qt.rgba(1, 1, 1, 0.76)
                    font.pixelSize: 10
                    font.weight: groupRow.active ? Font.Medium : Font.Normal
                    elide: Text.ElideRight
                }

                Text {
                    width: parent.width
                    text: groupRow.meta
                    color: Qt.rgba(1, 1, 1, 0.42)
                    font.pixelSize: 9
                    elide: Text.ElideRight
                }
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: String(groupRow.count)
                color: groupRow.active ? Qt.rgba(1, 1, 1, 0.62) : Qt.rgba(1, 1, 1, 0.48)
                font.pixelSize: 10
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: groupRow.clicked()
        }
    }
}
