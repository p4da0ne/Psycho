pragma ComponentBehavior: Bound
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root

    property Item backdropSource
    property real topInset: 80
    property var treeData: []

    property string panelState: "hidden"
    property bool dragging: false
    property real dragVelocity: 0
    property real panelX: stateX(panelState)
    property string selectedPath: "countries/russia/moscow"
    property var expandedMap: ({
        "countries": true,
        "countries/russia": true
    })

    readonly property real panelWidth: 320
    readonly property real peekWidth: 24
    readonly property real sideMargin: 16
    readonly property real topMargin: 8
    readonly property real headerHeight: 44
    readonly property real panelHeight: Math.max(280, height - topInset - 16)
    readonly property real hiddenX: width + 6
    readonly property real peekX: width - peekWidth
    readonly property real expandedX: width - panelWidth - sideMargin
    readonly property real reservedWidth: Math.max(0, width - panelX - 6)

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
                var hasChildren = !!current.children && current.children.length > 0
                output.push({
                    "label": current.label,
                    "path": path,
                    "depth": depth,
                    "hasChildren": hasChildren,
                    "expanded": hasChildren && root.isExpanded(path)
                })

                if (hasChildren && root.isExpanded(path))
                    walk(current.children, depth + 1, path)
            }
        }

        walk(root.treeData || [], 0, "")
        return output
    }

    onPanelStateChanged: {
        if (!dragging)
            panelX = stateX(panelState)
    }

    onWidthChanged: {
        if (!dragging)
            panelX = stateX(panelState)
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
            SpringAnimation {
                spring: 3.2
                damping: 0.36
            }
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

            ListView {
                id: treeView

                width: parent.width
                height: Math.max(0, parent.height - y)
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
                    height: 32
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
                        anchors.rightMargin: 10
                        spacing: 8

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

                        Text {
                            width: parent.width - x
                            anchors.verticalCenter: parent.verticalCenter
                            text: rowRoot.modelData.label
                            color: root.selectedPath === rowRoot.modelData.path
                                ? Qt.rgba(1, 1, 1, 0.94)
                                : Qt.rgba(1, 1, 1, 0.80)
                            font.pixelSize: 10
                            font.weight: root.selectedPath === rowRoot.modelData.path ? Font.Medium : Font.Normal
                            lineHeight: 1.18
                            elide: Text.ElideRight
                        }
                    }

                    MouseArea {
                        id: rowMouse

                        anchors.fill: parent
                        hoverEnabled: true

                        onClicked: {
                            if (rowRoot.modelData.hasChildren)
                                root.toggleNode(rowRoot.modelData.path)
                            root.selectedPath = rowRoot.modelData.path
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
}
