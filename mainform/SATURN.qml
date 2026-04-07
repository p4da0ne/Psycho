pragma ComponentBehavior: Bound
import QtQuick 2.15
import QtQuick.Window 2.15
import "qrc:/components"

Window {
    id: root

    visible: true
    width: 1540
    height: 920
    minimumWidth: 1180
    minimumHeight: 760
    title: "САТУРН"
    color: "#0B0F14"

    property var stateManager: appState
    property var repository: dataLayer
    property var agentHub: agents
    property var panelManager: root.agentHub ? root.agentHub.panelManager : null
    property var mapObjects: root.repository.objects
    property var locationLabels: root.repository.labels
    property var mapEvents: root.repository.events
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

    Component.onCompleted: {
        syncLayoutBounds()
        if (root.panelManager) {
            root.panelManager.setPanelVisible("right-sidebar", root.stateManager.inspectorVisible)
            root.panelManager.setPanelVisible("reference-panel", root.stateManager.navigationVisible)
        }
    }
    onWidthChanged: syncLayoutBounds()
    onHeightChanged: syncLayoutBounds()

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
            structureAgent: root.agentHub.structureAgent
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
            anchors.bottomMargin: 16
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
            anchors.bottomMargin: statusBar.visible ? 12 : 16
            appState: root.stateManager
            agentHub: root.agentHub
            objects: root.mapObjects
            labels: root.locationLabels
            onInteractionActivity: {
                root.agentHub.uiStateAgent.setControlsVisible(false)
                controlsReturnTimer.restart()
            }
        }
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
            root.agentHub.selectionAgent.clearSelection()
            root.agentHub.selectionAgent.clearEventSelection()
        }
    }
}
