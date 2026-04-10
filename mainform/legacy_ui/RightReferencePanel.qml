pragma ComponentBehavior: Bound
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root

    property Item backdropSource
    property real topInset: 80
    property var structureAgent: null

    property string panelState: "hidden"
    property bool dragging: false
    property real dragVelocity: 0
    property real panelX: stateX(panelState)
    property var treeData: structureAgent ? structureAgent.referenceTree : []
    property string selectedPath: structureAgent ? structureAgent.selectedPath : ""
    property var selectedNode: structureAgent ? structureAgent.selectedNode : ({})
    property var expandedMap: ({
        "countries": true,
        "countries/russia": true
    })
    property string editingPath: ""

    readonly property real panelWidth: 620
    readonly property real peekWidth: 24
    readonly property real sideMargin: 16
    readonly property real topMargin: 8
    readonly property real headerHeight: 44
    readonly property real panelHeight: Math.max(360, height - topInset - 16)
    readonly property real hiddenX: width + 6
    readonly property real peekX: width - peekWidth
    readonly property real expandedX: width - panelWidth - sideMargin
    readonly property real reservedWidth: Math.max(0, width - panelX - 6)
    readonly property bool hasSelection: !!selectedPath && selectedPath.length > 0
    readonly property int childCount: selectedNode && selectedNode.children ? selectedNode.children.length : 0
    readonly property bool hasChildren: childCount > 0

    function stateX(stateName) {
        if (stateName === "expanded")
            return expandedX
        if (stateName === "peek")
            return peekX
        return hiddenX
    }

    function nearestStateForX(xValue) {
        var hiddenDistance = Math.abs(xValue - hiddenX)
        var peekDistance = Math.abs(xValue - peekX)
        var expandedDistance = Math.abs(xValue - expandedX)

        if (expandedDistance <= peekDistance && expandedDistance <= hiddenDistance)
            return "expanded"
        if (peekDistance <= hiddenDistance && peekDistance <= expandedDistance)
            return "peek"
        return "hidden"
    }

    function clampPanelX(xValue) {
        return Math.max(expandedX, Math.min(hiddenX, xValue))
    }

    function settlePanelState() {
        if (dragVelocity < -0.45)
            panelState = "expanded"
        else if (dragVelocity > 0.45)
            panelState = panelX > peekX ? "hidden" : "peek"
        else
            panelState = nearestStateForX(panelX)
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
                    "id": current.id,
                    "label": current.label,
                    "path": path,
                    "depth": depth,
                    "hasChildren": hasChildrenValue,
                    "childCount": children.length,
                    "expanded": hasChildrenValue && root.isExpanded(path)
                })

                if (hasChildrenValue && root.isExpanded(path))
                    walk(children, depth + 1, path)
            }
        }

        walk(root.treeData || [], 0, "")
        return output
    }

    function beginInlineEdit(path, label) {
        editingPath = path
        if (root.structureAgent)
            root.structureAgent.selectedPath = path
    }

    function finishInlineEdit(labelText) {
        if (!editingPath || !root.structureAgent) {
            editingPath = ""
            return
        }
        root.structureAgent.renameNode(editingPath, labelText)
        editingPath = ""
    }

    onPanelStateChanged: {
        if (!dragging)
            panelX = stateX(panelState)
    }

    onWidthChanged: {
        if (!dragging)
            panelX = stateX(panelState)
    }

    Connections {
        target: root.structureAgent

        function onSelectedPathChanged() {
            root.selectedPath = root.structureAgent.selectedPath
        }

        function onSelectedNodeChanged() {
            root.selectedNode = root.structureAgent.selectedNode
        }

        function onReferenceTreeChanged() {
            root.treeData = root.structureAgent.referenceTree
        }
    }

    GlassPanel {
        id: panel

        width: root.panelWidth
        height: root.panelHeight
        x: root.panelX
        y: root.topInset + root.topMargin
        radius: 24
        padding: 0
        z: 50
        backdropSource: root.backdropSource
        surfaceColor: "#141c24"
        surfaceOpacity: root.dragging ? 0.58 : 0.55
        shadowOpacity: root.dragging ? 0.14 : 0.08
        highlightOpacity: 0.05

        Behavior on x {
            enabled: !root.dragging
            SpringAnimation { spring: 3.2; damping: 0.36 }
        }

        Rectangle {
            width: 4
            height: 44
            radius: 2
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            color: "#ffffff"
            opacity: 0.12
        }

        Column {
            anchors.fill: parent
            anchors.leftMargin: 18
            anchors.rightMargin: 14
            anchors.topMargin: 18
            anchors.bottomMargin: 14
            spacing: 10

            Row {
                width: parent.width
                height: root.headerHeight
                spacing: 8

                Text {
                    width: Math.max(0, parent.width - 40)
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Reference"
                    color: Qt.rgba(1, 1, 1, 0.92)
                    font.pixelSize: 14
                    font.weight: Font.Medium
                    lineHeight: 1.16
                    elide: Text.ElideRight
                }

                Button {
                    id: collapseButton

                    anchors.verticalCenter: parent.verticalCenter
                    implicitWidth: 28
                    implicitHeight: 28
                    flat: true
                    background: Rectangle {
                        radius: 14
                        color: "#ffffff"
                        opacity: collapseButton.down ? 0.08 : collapseButton.hovered ? 0.05 : 0.03
                    }
                    contentItem: Text {
                        text: root.panelState === "expanded" ? "›" : "×"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: Qt.rgba(1, 1, 1, 0.74)
                        font.pixelSize: root.panelState === "expanded" ? 15 : 13
                        rotation: root.panelState === "expanded" ? 180 : 0
                    }
                    onClicked: {
                        if (root.panelState === "expanded")
                            root.panelState = "peek"
                        else
                            root.panelState = "hidden"
                    }
                }
            }

            Row {
                width: parent.width
                height: Math.max(0, parent.height - y)
                spacing: 12

                Column {
                    width: 278
                    height: parent.height
                    spacing: 8

                    Text {
                        text: "Hierarchy"
                        color: Qt.rgba(1, 1, 1, 0.48)
                        font.pixelSize: 9
                        font.weight: Font.Medium
                    }

                    ListView {
                        id: treeView

                        width: parent.width
                        height: parent.height - y
                        clip: true
                        spacing: 2
                        boundsBehavior: Flickable.StopAtBounds
                        model: root.visibleNodes()

                        ScrollIndicator.vertical: ScrollIndicator {
                            active: treeView.moving || treeView.flicking
                        }

                        add: Transition {
                            NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 150 }
                        }

                        remove: Transition {
                            NumberAnimation { property: "opacity"; from: 1; to: 0; duration: 120 }
                        }

                        displaced: Transition {
                            NumberAnimation { properties: "x,y"; duration: 160 }
                        }

                        delegate: Rectangle {
                            id: rowRoot

                            required property var modelData

                            width: ListView.view.width
                            height: 34
                            radius: 12
                            color: root.selectedPath === modelData.path
                                ? Qt.rgba(1, 1, 1, 0.078)
                                : rowMouse.containsMouse
                                    ? Qt.rgba(1, 1, 1, 0.030)
                                    : "transparent"
                            border.width: root.selectedPath === modelData.path ? 1 : 0
                            border.color: Qt.rgba(1, 1, 1, 0.05)

                            Behavior on color {
                                ColorAnimation { duration: 160 }
                            }

                            Row {
                                anchors.fill: parent
                                anchors.leftMargin: 12 + rowRoot.modelData.depth * 14
                                anchors.rightMargin: 8
                                spacing: 6

                                Item {
                                    width: 12
                                    height: parent.height
                                    visible: rowRoot.modelData.hasChildren

                                    Text {
                                        anchors.centerIn: parent
                                        text: "›"
                                        color: Qt.rgba(1, 1, 1, 0.64)
                                        font.pixelSize: 12
                                        rotation: rowRoot.modelData.expanded ? 90 : 0

                                        Behavior on rotation {
                                            NumberAnimation { duration: 160 }
                                        }
                                    }
                                }

                                Item {
                                    width: rowRoot.modelData.hasChildren ? 0 : 12
                                    height: 1
                                }

                                Item {
                                    width: Math.max(0, parent.width - 58)
                                    height: parent.height

                                    Text {
                                        visible: root.editingPath !== rowRoot.modelData.path
                                        anchors.verticalCenter: parent.verticalCenter
                                        width: parent.width
                                        text: rowRoot.modelData.label
                                        color: root.selectedPath === rowRoot.modelData.path
                                            ? Qt.rgba(1, 1, 1, 0.94)
                                            : Qt.rgba(1, 1, 1, 0.80)
                                        font.pixelSize: 10
                                        font.weight: root.selectedPath === rowRoot.modelData.path ? Font.Medium : Font.Normal
                                        lineHeight: 1.18
                                        elide: Text.ElideRight
                                    }

                                    TextField {
                                        id: inlineEditor

                                        visible: root.editingPath === rowRoot.modelData.path
                                        anchors.verticalCenter: parent.verticalCenter
                                        width: parent.width
                                        height: 26
                                        text: rowRoot.modelData.label
                                        color: Qt.rgba(1, 1, 1, 0.90)
                                        font.pixelSize: 10
                                        background: Rectangle {
                                            radius: 10
                                            color: Qt.rgba(1, 1, 1, 0.05)
                                            border.width: 1
                                            border.color: Qt.rgba(1, 1, 1, 0.06)
                                        }
                                        selectByMouse: true
                                        onAccepted: root.finishInlineEdit(text)
                                        onEditingFinished: root.finishInlineEdit(text)
                                        Component.onCompleted: if (visible) forceActiveFocus()
                                    }
                                }

                                Row {
                                    anchors.verticalCenter: parent.verticalCenter
                                    spacing: 4
                                    opacity: rowMouse.containsMouse || root.selectedPath === rowRoot.modelData.path ? 1 : 0

                                    Behavior on opacity {
                                        NumberAnimation { duration: 120 }
                                    }

                                    Button {
                                        id: addChildButton

                                        implicitWidth: 18
                                        implicitHeight: 18
                                        flat: true
                                        background: Rectangle {
                                            radius: 9
                                            color: "#ffffff"
                                            opacity: addChildButton.down ? 0.08 : addChildButton.hovered ? 0.05 : 0.02
                                        }
                                        contentItem: Text {
                                            text: "+"
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                            color: Qt.rgba(1, 1, 1, 0.68)
                                            font.pixelSize: 11
                                        }
                                        onClicked: root.structureAgent.addChild(rowRoot.modelData.path, "New node")
                                    }

                                    Button {
                                        id: editNodeButton

                                        implicitWidth: 18
                                        implicitHeight: 18
                                        flat: true
                                        background: Rectangle {
                                            radius: 9
                                            color: "#ffffff"
                                            opacity: editNodeButton.down ? 0.08 : editNodeButton.hovered ? 0.05 : 0.02
                                        }
                                        contentItem: Text {
                                            text: "✎"
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                            color: Qt.rgba(1, 1, 1, 0.68)
                                            font.pixelSize: 9
                                        }
                                        onClicked: root.beginInlineEdit(rowRoot.modelData.path, rowRoot.modelData.label)
                                    }
                                }
                            }

                            MouseArea {
                                id: rowMouse
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: {
                                    if (rowRoot.modelData.hasChildren)
                                        root.toggleNode(rowRoot.modelData.path)
                                    if (root.structureAgent)
                                        root.structureAgent.selectedPath = rowRoot.modelData.path
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    width: 1
                    height: parent.height
                    color: Qt.rgba(1, 1, 1, 0.05)
                }

                Column {
                    width: parent.width - 291
                    height: parent.height
                    spacing: 10

                    Text {
                        text: "Inspector"
                        color: Qt.rgba(1, 1, 1, 0.48)
                        font.pixelSize: 9
                        font.weight: Font.Medium
                    }

                    ScrollView {
                        width: parent.width
                        height: parent.height - y
                        clip: true

                        Column {
                            width: parent.width - 12
                            spacing: 12

                            Text {
                                width: parent.width
                                text: root.hasSelection ? root.selectedNode.label : "Select a node"
                                color: Qt.rgba(1, 1, 1, 0.94)
                                font.pixelSize: 15
                                font.weight: Font.Medium
                                lineHeight: 1.16
                                elide: Text.ElideRight
                            }

                            InspectorSection {
                                width: parent.width
                                title: "Node"

                                InspectorField {
                                    width: parent.width
                                    label: "Path"
                                    value: root.selectedPath
                                    mono: true
                                }

                                InspectorField {
                                    width: parent.width
                                    label: "Kind"
                                    value: root.hasSelection ? (root.hasChildren ? "Group" : "Leaf") : ""
                                }

                                InspectorField {
                                    width: parent.width
                                    label: "Children"
                                    value: root.hasSelection ? String(root.childCount) : "0"
                                }
                            }

                            InspectorSection {
                                width: parent.width
                                title: "Edit"

                                TextField {
                                    id: inspectorEditor

                                    width: parent.width
                                    height: 34
                                    enabled: root.hasSelection
                                    text: root.hasSelection ? root.selectedNode.label : ""
                                    color: Qt.rgba(1, 1, 1, 0.90)
                                    font.pixelSize: 11
                                    background: Rectangle {
                                        radius: 12
                                        color: Qt.rgba(1, 1, 1, 0.05)
                                        border.width: 1
                                        border.color: Qt.rgba(1, 1, 1, 0.06)
                                    }
                                    selectByMouse: true
                                    onEditingFinished: {
                                        if (root.hasSelection)
                                            root.structureAgent.renameSelectedNode(text)
                                    }
                                }

                                Row {
                                    width: parent.width
                                    spacing: 6

                                    ActionChip {
                                        text: "Add child"
                                        enabled: root.hasSelection
                                        onClicked: root.structureAgent.addChild(root.selectedPath, "New node")
                                    }

                                    ActionChip {
                                        text: "Delete"
                                        enabled: root.structureAgent ? root.structureAgent.canDeleteSelected : false
                                        destructive: true
                                        onClicked: root.structureAgent.deleteSelectedNode()
                                    }
                                }
                            }

                            InspectorSection {
                                width: parent.width
                                title: "Details"

                                InspectorField {
                                    width: parent.width
                                    label: "Identifier"
                                    value: root.hasSelection ? root.selectedNode.id : ""
                                    mono: true
                                }

                                InspectorField {
                                    width: parent.width
                                    label: "Mode"
                                    value: "Inspector-driven reference editing"
                                }
                            }
                        }
                    }
                }
            }
        }

        MouseArea {
            id: headerDragArea

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 50
            anchors.top: parent.top
            height: 62
            cursorShape: Qt.OpenHandCursor
            hoverEnabled: true
            acceptedButtons: Qt.LeftButton
            preventStealing: true

            property real startX: 0
            property real startPanelX: 0
            property real lastX: 0
            property real lastTimestamp: 0
            property bool movedEnough: false

            onPressed: function(mouse) {
                root.dragging = true
                startX = mouse.x
                startPanelX = panel.x
                lastX = mouse.x
                lastTimestamp = Date.now()
                root.dragVelocity = 0
                movedEnough = false
                root.panelX = root.clampPanelX(root.panelX)
            }

            onPositionChanged: function(mouse) {
                if (!pressed)
                    return

                var now = Date.now()
                var deltaTime = Math.max(1, now - lastTimestamp)
                root.dragVelocity = (mouse.x - lastX) / deltaTime
                lastX = mouse.x
                lastTimestamp = now
                movedEnough = movedEnough || Math.abs(mouse.x - startX) > 4

                var nextX = startPanelX + (mouse.x - startX)
                root.panelX = root.clampPanelX(nextX)
            }

            onReleased: {
                root.dragging = false
                root.settlePanelState()
            }

            onClicked: {
                if (!root.dragging && !pressed && !movedEnough)
                    root.panelState = root.panelState === "expanded" ? "peek" : "expanded"
            }
        }

        MouseArea {
            id: sideDragHandle

            anchors.left: parent.left
            anchors.top: headerDragArea.bottom
            anchors.bottom: parent.bottom
            width: 22
            cursorShape: Qt.OpenHandCursor
            hoverEnabled: true

            property real startX: 0
            property real startPanelX: 0
            property real lastX: 0
            property real lastTimestamp: 0

            onPressed: function(mouse) {
                root.dragging = true
                startX = mouse.x
                startPanelX = root.panelX
                lastX = mouse.x
                lastTimestamp = Date.now()
                root.dragVelocity = 0
            }

            onPositionChanged: function(mouse) {
                if (!pressed)
                    return

                var now = Date.now()
                var deltaTime = Math.max(1, now - lastTimestamp)
                root.dragVelocity = (mouse.x - lastX) / deltaTime
                lastX = mouse.x
                lastTimestamp = now
                root.panelX = root.clampPanelX(startPanelX + (mouse.x - startX))
            }

            onReleased: {
                root.dragging = false
                root.settlePanelState()
            }
        }
    }

    MouseArea {
        id: edgeSensor

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: 18
        hoverEnabled: true

        property real startX: 0
        property real lastX: 0
        property real lastTimestamp: 0
        property bool movedEnough: false

        onPressed: function(mouse) {
            root.dragging = true
            startX = mouse.x
            lastX = mouse.x
            lastTimestamp = Date.now()
            root.dragVelocity = 0
            movedEnough = false
            root.panelX = root.hiddenX
        }

        onPositionChanged: function(mouse) {
            if (!pressed)
                return

            var now = Date.now()
            var deltaTime = Math.max(1, now - lastTimestamp)
            root.dragVelocity = (mouse.x - lastX) / deltaTime
            lastX = mouse.x
            lastTimestamp = now
            movedEnough = movedEnough || Math.abs(mouse.x - startX) > 4

            var globalPoint = edgeSensor.mapToItem(root, mouse.x, mouse.y)
            root.panelX = root.clampPanelX(globalPoint.x - root.panelWidth)
        }

        onReleased: {
            root.dragging = false
            root.settlePanelState()
        }

        onClicked: {
            if (!root.dragging && !movedEnough)
                root.panelState = root.panelState === "hidden" ? "peek" : "expanded"
        }
    }

    Rectangle {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        width: 3
        height: 44
        radius: 2
        color: "#ffffff"
        opacity: root.panelState === "hidden" ? 0.10 : 0.0

        Behavior on opacity {
            NumberAnimation { duration: 160 }
        }
    }

    component InspectorSection: Column {
        property string title: ""
        width: 200
        spacing: 8

        Text {
            text: parent.title
            color: Qt.rgba(1, 1, 1, 0.48)
            font.pixelSize: 9
            font.weight: Font.Medium
        }
    }

    component InspectorField: Item {
        id: inspectorField

        property string label: ""
        property string value: ""
        property bool mono: false

        implicitWidth: 220
        implicitHeight: valueText.paintedHeight + labelText.paintedHeight + 6

        Column {
            width: parent.width
            spacing: 2

            Text {
                id: labelText
                width: parent.width
                text: inspectorField.label
                color: Qt.rgba(1, 1, 1, 0.46)
                font.pixelSize: 9
                elide: Text.ElideRight
            }

            Text {
                id: valueText
                width: parent.width
                text: inspectorField.value
                color: Qt.rgba(1, 1, 1, 0.88)
                font.pixelSize: 10
                font.family: inspectorField.mono ? "Menlo" : ""
                lineHeight: 1.2
                wrapMode: Text.Wrap
                maximumLineCount: 4
                elide: Text.ElideRight
            }
        }
    }

    component ActionChip: Button {
        id: actionChip

        property bool destructive: false

        implicitHeight: 28
        implicitWidth: 90
        flat: true
        background: Rectangle {
            radius: 14
            color: "#ffffff"
            opacity: actionChip.down ? 0.08 : actionChip.hovered ? 0.05 : 0.03
        }
        contentItem: Text {
            text: actionChip.text
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: actionChip.destructive ? "#f2b0b0" : Qt.rgba(1, 1, 1, 0.78)
            font.pixelSize: 10
            font.weight: Font.Medium
        }
    }
}
