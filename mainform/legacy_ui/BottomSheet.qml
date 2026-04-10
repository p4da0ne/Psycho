pragma ComponentBehavior: Bound
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root

    property var appState
    property var agentHub
    property var objects: []
    property int visibleCount: 0
    property int averageMppsValue: 0
    property Item backdropSource
    property real topInset: 80
    property real rightReservedWidth: 0

    property string sheetState: "medium"
    property string previousExpandedState: "medium"
    property string dockMode: "bottom"
    property bool dragging: false
    property real dragVelocity: 0
    property string currentDockHint: ""

    property real floatX: 0
    property real floatY: 0
    property real floatWidth: 700
    property real floatHeight: 420

    readonly property real safeMargin: 16
    readonly property real dockMargin: 8
    readonly property real edgeDockThreshold: 124
    readonly property real headerDragHeight: 96
    readonly property real sideDockWidth: 360
    readonly property real collapsedHeaderHeight: 84
    readonly property real collapsedSideWidth: 220

    function visibleHeightForState(stateName) {
        if (stateName === "expanded")
            return Math.max(380, height * 0.88)
        if (stateName === "collapsed")
            return collapsedHeaderHeight
        return Math.max(300, height * 0.50)
    }

    function sideWidthForState(stateName) {
        if (stateName === "collapsed")
            return collapsedSideWidth
        return Math.min(sideDockWidth, width * 0.34)
    }

    function floatingExpandedWidth() {
        return Math.min(700, width - safeMargin * 2 - rightReservedWidth)
    }

    function floatingExpandedHeight() {
        return Math.min(420, height - topInset - safeMargin)
    }

    function isCollapsed() {
        return sheetState === "collapsed"
    }

    function applyFloatingSizeForState() {
        if (sheetState === "collapsed") {
            floatWidth = Math.min(360, width - safeMargin * 2 - rightReservedWidth)
            floatHeight = collapsedHeaderHeight
        } else {
            floatWidth = Math.max(floatWidth, floatingExpandedWidth())
            floatHeight = Math.max(floatHeight, floatingExpandedHeight())
        }
        clampFloatingRect()
    }

    function toggleCollapsed() {
        if (sheetState === "collapsed") {
            sheetState = previousExpandedState
            if (dockMode === "floating")
                applyFloatingSizeForState()
            return
        }

        previousExpandedState = sheetState === "expanded" ? "expanded" : "medium"
        sheetState = "collapsed"
        if (dockMode === "floating")
            applyFloatingSizeForState()
    }

    function availableWidth() {
        return Math.max(320, width - safeMargin * 2 - rightReservedWidth)
    }

    function dockRect(modeName) {
        var panelHeight = visibleHeightForState(sheetState)
        var bottomWidth = Math.min(700, availableWidth())
        var centeredX = dockMargin + Math.max(0, (availableWidth() - bottomWidth) / 2)

        if (modeName === "top") {
            return Qt.rect(centeredX,
                           topInset + dockMargin,
                           bottomWidth,
                           Math.min(panelHeight, height - topInset - safeMargin))
        }

        if (modeName === "left") {
            return Qt.rect(dockMargin,
                           topInset + dockMargin,
                           sideWidthForState(sheetState),
                           Math.max(280, height - topInset - safeMargin - dockMargin))
        }

        if (modeName === "right") {
            var rightWidth = sideWidthForState(sheetState)
            return Qt.rect(Math.max(dockMargin, width - rightReservedWidth - rightWidth - dockMargin),
                           topInset + dockMargin,
                           rightWidth,
                           Math.max(280, height - topInset - safeMargin - dockMargin))
        }

        return Qt.rect(centeredX,
                       height - panelHeight - dockMargin,
                       bottomWidth,
                       panelHeight)
    }

    function currentRect() {
        if (dockMode === "floating") {
            var currentFloatWidth = sheetState === "collapsed"
                ? Math.min(floatWidth, 360)
                : floatWidth
            var currentFloatHeight = sheetState === "collapsed"
                ? collapsedHeaderHeight
                : floatHeight
            return Qt.rect(floatX, floatY, currentFloatWidth, currentFloatHeight)
        }
        return dockRect(dockMode)
    }

    function clampFloatingRect() {
        floatWidth = Math.max(sheetState === "collapsed" ? collapsedSideWidth : 320,
                              Math.min(floatWidth, width - safeMargin * 2 - rightReservedWidth))
        floatHeight = Math.max(sheetState === "collapsed" ? collapsedHeaderHeight : 260,
                               Math.min(floatHeight, height - topInset - safeMargin))
        floatX = Math.max(safeMargin, Math.min(floatX, width - rightReservedWidth - floatWidth - safeMargin))
        floatY = Math.max(topInset + dockMargin, Math.min(floatY, height - floatHeight - safeMargin))
    }

    function adoptCurrentRectAsFloating() {
        var rect = currentRect()
        floatX = rect.x
        floatY = rect.y
        floatWidth = rect.width
        floatHeight = rect.height
        clampFloatingRect()
    }

    function updateDockHintForPoint(xPos, yPos) {
        if (yPos <= topInset + edgeDockThreshold)
            currentDockHint = "top"
        else if (xPos <= edgeDockThreshold)
            currentDockHint = "left"
        else if (xPos >= width - rightReservedWidth - edgeDockThreshold)
            currentDockHint = "right"
        else if (yPos >= height - edgeDockThreshold)
            currentDockHint = "bottom"
        else
            currentDockHint = ""
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
        if (objectData.side === "friendly" && !appState.showFriendly)
            return false
        if (objectData.side === "foreign" && !appState.showForeign)
            return false
        if (objectData.side === "infrastructure" && !appState.showBaseStations)
            return false
        return true
    }

    function visibleObjects() {
        return objects.filter(function(objectData) {
            return objectVisible(objectData)
        })
    }

    function selectedVisibleIndex() {
        if (!appState.selectedObject)
            return -1

        var current = visibleObjects()
        for (var i = 0; i < current.length; ++i) {
            if (current[i].id === appState.selectedObject.id)
                return i
        }
        return -1
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: root.edgeDockThreshold
        color: "#ffffff"
        opacity: root.dragging && root.currentDockHint === "bottom" ? 0.05 : 0.0

        Behavior on opacity { NumberAnimation { duration: 120 } }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: root.edgeDockThreshold
        color: "#ffffff"
        opacity: root.dragging && root.currentDockHint === "left" ? 0.05 : 0.0

        Behavior on opacity { NumberAnimation { duration: 120 } }
    }

    Rectangle {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: root.edgeDockThreshold
        color: "#ffffff"
        opacity: root.dragging && root.currentDockHint === "right" ? 0.05 : 0.0

        Behavior on opacity { NumberAnimation { duration: 120 } }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        y: root.topInset
        height: root.edgeDockThreshold
        color: "#ffffff"
        opacity: root.dragging && root.currentDockHint === "top" ? 0.05 : 0.0

        Behavior on opacity { NumberAnimation { duration: 120 } }
    }

    GlassPanel {
        id: sheet

        x: root.currentRect().x
        y: root.currentRect().y
        width: root.currentRect().width
        height: root.currentRect().height
        radius: 24
        padding: 0
        backdropSource: root.backdropSource
        surfaceColor: "#141c24"
        surfaceOpacity: root.dragging ? 0.58 : root.appState.controlsVisible ? 0.55 : 0.50
        shadowOpacity: root.dockMode === "floating" || root.dragging ? 0.15 : 0.08
        highlightOpacity: root.dragging ? 0.08 : 0.05
        opacity: root.appState.controlsVisible ? 1 : 0.62
        scale: root.appState.controlsVisible ? 1 : 0.996

        Behavior on x {
            enabled: !root.dragging
            SpringAnimation { spring: 3.1; damping: 0.34 }
        }

        Behavior on y {
            enabled: !root.dragging
            SpringAnimation { spring: 3.1; damping: 0.34 }
        }

        Behavior on width {
            enabled: !root.dragging
            SpringAnimation { spring: 3.0; damping: 0.35 }
        }

        Behavior on height {
            enabled: !root.dragging
            SpringAnimation { spring: 3.0; damping: 0.35 }
        }

        Behavior on opacity {
            NumberAnimation { duration: 260 }
        }

        Behavior on scale {
            NumberAnimation { duration: 260 }
        }

        Item {
            anchors.fill: parent

            MouseArea {
                id: dragArea

                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                height: Math.min(root.headerDragHeight, parent.height)
                cursorShape: Qt.OpenHandCursor
                hoverEnabled: true
                acceptedButtons: Qt.LeftButton
                preventStealing: true

                property real startRootX: 0
                property real startRootY: 0
                property real startPanelX: 0
                property real startPanelY: 0
                property string startDockMode: ""
                property real lastRootX: 0
                property real lastTimestamp: 0
                property bool promotedToFloating: false

                onPressed: function(mouse) {
                    var point = dragArea.mapToItem(root, mouse.x, mouse.y)
                    startRootX = point.x
                    startRootY = point.y
                    startDockMode = root.dockMode
                    promotedToFloating = root.dockMode === "floating"
                    root.dragging = true
                    root.currentDockHint = ""
                    root.dragVelocity = 0
                    lastRootX = point.x
                    lastTimestamp = Date.now()

                    if (root.dockMode === "floating")
                        root.clampFloatingRect()
                    else
                        root.adoptCurrentRectAsFloating()

                    startPanelX = root.floatX
                    startPanelY = root.floatY
                    root.appState.controlsVisible = true
                }

                onPositionChanged: function(mouse) {
                    if (!pressed)
                        return

                    var point = dragArea.mapToItem(root, mouse.x, mouse.y)
                    var dx = point.x - startRootX
                    var dy = point.y - startRootY
                    var now = Date.now()
                    var deltaTime = Math.max(1, now - lastTimestamp)
                    root.dragVelocity = (point.x - lastRootX) / deltaTime
                    lastRootX = point.x
                    lastTimestamp = now

                    if (!promotedToFloating && (Math.abs(dx) > 6 || Math.abs(dy) > 6)) {
                        root.dockMode = "floating"
                        promotedToFloating = true
                    }

                    if (root.dockMode === "floating") {
                        root.floatX = startPanelX + dx
                        root.floatY = startPanelY + dy
                        root.clampFloatingRect()
                        root.updateDockHintForPoint(point.x, point.y)
                    }
                }

                onReleased: {
                    root.dragging = false

                    if (root.currentDockHint !== "") {
                        root.dockMode = root.currentDockHint
                    } else if (promotedToFloating) {
                        root.dockMode = "floating"
                        root.clampFloatingRect()
                    } else {
                        root.dockMode = startDockMode
                    }

                    root.currentDockHint = ""
                }
            }

            Rectangle {
                width: 32
                height: 4
                radius: 2
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 12
                color: "#ffffff"
                opacity: 0.14
            }

            Column {
                anchors.fill: parent
                anchors.leftMargin: 14
                anchors.rightMargin: 14
                anchors.topMargin: 22
                anchors.bottomMargin: 12
                spacing: 8

                Row {
                    width: parent.width
                    spacing: 12

                    Column {
                        width: Math.max(0, parent.width - 96)
                        spacing: 2

                        Text {
                            width: parent.width
                            text: root.appState.selectedObject ? root.appState.selectedObject.name : "Objects"
                            color: Qt.rgba(1, 1, 1, 0.92)
                            font.pixelSize: 15
                            font.weight: Font.Medium
                            lineHeight: 1.16
                            elide: Text.ElideRight
                        }

                        Text {
                            width: parent.width
                            text: root.appState.selectedObject
                                ? root.appState.selectedObject.kind + " · " + root.appState.selectedObject.side
                                : root.visibleCount + " visible · MPPS " + root.averageMppsValue
                            color: Qt.rgba(1, 1, 1, 0.62)
                            font.pixelSize: 10
                            lineHeight: 1.18
                            elide: Text.ElideRight
                        }
                    }

                    Button {
                        id: collapseButton

                        anchors.verticalCenter: parent.verticalCenter
                        implicitWidth: 30
                        implicitHeight: 30
                        flat: true
                        background: Rectangle {
                            radius: 15
                            color: "#ffffff"
                            opacity: collapseButton.down ? 0.09 : collapseButton.hovered ? 0.06 : 0.04
                        }
                        contentItem: Text {
                            text: root.sheetState === "collapsed" ? "+" : "−"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            color: "#dce4ed"
                            font.pixelSize: 15
                            font.weight: Font.Medium
                        }
                        onClicked: root.toggleCollapsed()
                    }

                    Button {
                        id: filterButton

                        anchors.verticalCenter: parent.verticalCenter
                        implicitWidth: 30
                        implicitHeight: 30
                        flat: true
                        background: Rectangle {
                            radius: 15
                            color: "#ffffff"
                            opacity: filterButton.down ? 0.09 : filterButton.hovered ? 0.06 : 0.04
                        }
                        contentItem: Text {
                            text: "⋯"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            color: "#dce4ed"
                            font.pixelSize: 16
                        }
                        onClicked: filterMenu.open()
                    }
                }

                Flow {
                    width: parent.width
                    spacing: 5
                    visible: root.sheetState !== "collapsed"
                    opacity: visible ? 1 : 0

                    SegmentedPill {
                        leftText: "Point"
                        rightText: "Heat"
                        leftChecked: root.appState.mapMode === "point"
                        rightChecked: root.appState.mapMode === "heatmap"
                        onLeftActivated: root.appState.mapMode = "point"
                        onRightActivated: root.appState.mapMode = "heatmap"
                    }

                    SegmentedPill {
                        leftText: "Move"
                        middleText: "Select"
                        rightText: "Measure"
                        leftChecked: root.appState.activeTool === "navigate"
                        middleChecked: root.appState.activeTool === "select"
                        rightChecked: root.appState.activeTool === "measure"
                        threeWay: true
                        onLeftActivated: root.appState.activeTool = "navigate"
                        onMiddleActivated: root.appState.activeTool = "select"
                        onRightActivated: root.appState.activeTool = "measure"
                    }
                }

                ListView {
                    id: objectsList

                    width: parent.width
                    height: Math.max(0, parent.height - y)
                    visible: root.sheetState !== "collapsed"
                    opacity: visible ? 1 : 0
                    clip: true
                    spacing: 2
                    model: root.visibleObjects()
                    boundsBehavior: Flickable.StopAtBounds
                    topMargin: 2
                    bottomMargin: 6

                    ScrollIndicator.vertical: ScrollIndicator {
                        active: objectsList.moving || objectsList.flicking
                    }

                    delegate: Item {
                        id: rowRoot

                        required property var modelData

                        width: ListView.view.width
                        height: 36

                        readonly property bool selected: root.appState.selectedObject
                            && root.agentHub.selectionAgent.isSelected(modelData)
                        readonly property bool hovered: root.appState.hoveredObject
                            && root.appState.hoveredObject.id === modelData.id
                        readonly property bool muted: root.agentHub.selectionAgent.selectionCount() > 0 && !selected

                        Rectangle {
                            anchors.fill: parent
                            radius: 12
                            color: rowRoot.selected
                                ? Qt.rgba(1, 1, 1, 0.078)
                                : rowRoot.hovered
                                    ? Qt.rgba(1, 1, 1, 0.030)
                                    : "transparent"
                            opacity: rowRoot.muted ? 0.56 : 1.0
                            scale: rowRoot.selected ? 1.006 : 1.0
                            border.width: 1
                            border.color: rowRoot.selected
                                ? Qt.rgba(1, 1, 1, 0.050)
                                : "transparent"

                            Behavior on scale {
                                NumberAnimation { duration: 160 }
                            }

                            Behavior on opacity {
                                NumberAnimation { duration: 180 }
                            }

                            Row {
                                anchors.fill: parent
                                anchors.leftMargin: 10
                                anchors.rightMargin: 10
                                spacing: 8

                                Rectangle {
                                    width: 5
                                    height: 5
                                    radius: 3
                                    color: rowRoot.modelData.side === "friendly"
                                        ? "#6f96f8"
                                        : rowRoot.modelData.side === "foreign"
                                            ? "#dbe4ee"
                                            : "#8c98a6"
                                    anchors.verticalCenter: parent.verticalCenter
                                }

                                Column {
                                    width: Math.max(0, parent.width - 72)
                                    anchors.verticalCenter: parent.verticalCenter
                                    spacing: 0

                                    Text {
                                        width: parent.width
                                        text: rowRoot.modelData.name
                                        color: rowRoot.selected ? Qt.rgba(1, 1, 1, 0.94) : Qt.rgba(1, 1, 1, 0.76)
                                        font.pixelSize: 10
                                        font.weight: Font.Medium
                                        lineHeight: 1.18
                                        elide: Text.ElideRight
                                    }

                                    Text {
                                        width: parent.width
                                        text: rowRoot.modelData.id + " · " + rowRoot.modelData.kind
                                        color: rowRoot.selected ? Qt.rgba(1, 1, 1, 0.62) : Qt.rgba(1, 1, 1, 0.46)
                                        font.pixelSize: 8
                                        lineHeight: 1.18
                                        elide: Text.ElideRight
                                    }
                                }

                                Text {
                                    width: 38
                                    anchors.verticalCenter: parent.verticalCenter
                                    horizontalAlignment: Text.AlignRight
                                    text: rowRoot.modelData.mpps
                                    color: rowRoot.selected ? "#A9C4FF" : Qt.rgba(1, 1, 1, 0.58)
                                    font.pixelSize: 9
                                    font.weight: rowRoot.selected ? Font.Medium : Font.Normal
                                    elide: Text.ElideRight
                                }
                            }
                        }

                        MouseArea {
                            id: rowMouse
                            anchors.fill: parent
                            hoverEnabled: true

                            onEntered: root.agentHub.selectionAgent.setHoveredObject(rowRoot.modelData)
                            onExited: {
                                if (root.appState.hoveredObject && root.appState.hoveredObject.id === rowRoot.modelData.id)
                                    root.agentHub.selectionAgent.setHoveredObject({})
                            }

                            onClicked: function(mouse) {
                                var additive = (mouse.modifiers & Qt.ControlModifier) || (mouse.modifiers & Qt.MetaModifier)
                                if (additive)
                                    root.agentHub.selectionAgent.toggleObjectSelection(rowRoot.modelData)
                                else
                                    root.agentHub.selectionAgent.selectObject(rowRoot.modelData)
                            }
                        }
                    }
                }
            }
        }
    }

    Connections {
        target: root.appState

        function onSelectedObjectChanged() {
            var index = root.selectedVisibleIndex()
            if (index >= 0) {
                objectsList.positionViewAtIndex(index, ListView.Contain)
                if (root.dockMode === "bottom" && root.sheetState === "collapsed")
                    root.sheetState = "medium"
            }
        }
    }

    onWidthChanged: {
        if (dockMode === "floating")
            clampFloatingRect()
    }

    onHeightChanged: {
        if (dockMode === "floating")
            clampFloatingRect()
    }

    onRightReservedWidthChanged: {
        if (dockMode === "floating")
            clampFloatingRect()
    }

    onDockModeChanged: {
        if (dockMode === "floating")
            applyFloatingSizeForState()
    }

    Menu {
        id: filterMenu

        background: Rectangle {
            color: "#121821"
            border.color: Qt.rgba(1, 1, 1, 0.08)
            border.width: 1
            radius: 14
        }

        MenuItem {
            text: root.appState.showFriendly ? "Hide friendly" : "Show friendly"
            onTriggered: root.appState.showFriendly = !root.appState.showFriendly
        }

        MenuItem {
            text: root.appState.showForeign ? "Hide foreign" : "Show foreign"
            onTriggered: root.appState.showForeign = !root.appState.showForeign
        }

        MenuItem {
            text: root.appState.showBaseStations ? "Hide infrastructure" : "Show infrastructure"
            onTriggered: root.appState.showBaseStations = !root.appState.showBaseStations
        }

        MenuItem {
            text: root.appState.showCoverageLine ? "Hide line" : "Show line"
            onTriggered: root.appState.showCoverageLine = !root.appState.showCoverageLine
        }

        MenuItem {
            text: root.appState.showLabels ? "Hide labels" : "Show labels"
            onTriggered: root.appState.showLabels = !root.appState.showLabels
        }
    }

    component SegmentedPill: GlassPanel {
        id: segmented

        property string leftText: ""
        property string middleText: ""
        property string rightText: ""
        property bool leftChecked: false
        property bool middleChecked: false
        property bool rightChecked: false
        property bool threeWay: false
        signal leftActivated()
        signal middleActivated()
        signal rightActivated()

        radius: 20
        padding: 2
        surfaceColor: "#131922"
        surfaceOpacity: 0.38
        shadowOpacity: 0.02
        highlightOpacity: 0.05
        implicitHeight: 32
        implicitWidth: threeWay ? 282 : 170

        Row {
            anchors.fill: parent
            spacing: 2

            SegmentButton {
                width: segmented.threeWay ? 88 : 80
                text: segmented.leftText
                active: segmented.leftChecked
                onClicked: segmented.leftActivated()
            }

            SegmentButton {
                visible: segmented.threeWay
                width: 88
                text: segmented.middleText
                active: segmented.middleChecked
                onClicked: segmented.middleActivated()
            }

            SegmentButton {
                width: segmented.threeWay ? 94 : 80
                text: segmented.rightText
                active: segmented.rightChecked
                onClicked: segmented.rightActivated()
            }
        }
    }

    component SegmentButton: Button {
        id: segment

        property bool active: false

        background: Rectangle {
            radius: 14
            color: segment.active ? Qt.rgba(1, 1, 1, 0.10) : "transparent"
            border.width: segment.active ? 1 : 0
            border.color: Qt.rgba(1, 1, 1, 0.07)
        }

        contentItem: Text {
            text: segment.text
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: segment.active ? "#f5f8fc" : "#96a2af"
            font.pixelSize: 10
            font.weight: segment.active ? Font.Medium : Font.Normal
            lineHeight: 1.1
            elide: Text.ElideRight
            leftPadding: 4
            rightPadding: 4
        }
    }
}
