pragma ComponentBehavior: Bound
import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/theme"

Item {
    id: root

    property var appState
    property var panelManager
    property var panelState: ({})
    property real dragStartHeight: 0

    readonly property bool panelVisible: panelState && panelState.visible !== undefined ? panelState.visible : true
    readonly property bool collapsed: panelState && panelState.collapsed ? panelState.collapsed : false
    readonly property real expandedHeight: panelState && panelState.height ? panelState.height : 24
    readonly property real targetHeight: panelVisible ? (collapsed ? 18 : expandedHeight) : 0

    implicitHeight: targetHeight
    height: targetHeight
    visible: height > 0.5

    Behavior on height {
        NumberAnimation { duration: 160 }
    }

    function approximateScale() {
        if (!appState)
            return "-"
        var exactScale = Number(appState.mapScaleDenominator || 0)
        if (exactScale > 0)
            return "1:" + Math.max(1, Math.round(exactScale))
        var fallback = Math.max(500, Math.round(160000 / Math.pow(1.45, Math.max(0, appState.zoomLevel - 8))))
        return "1:" + fallback
    }

    GlassPanel {
        anchors.fill: parent
        radius: 16
        padding: 0
        surfaceColor: Theme.surface
        surfaceOpacity: 0.42
        shadowOpacity: 0.03
        highlightOpacity: 0.03
        edgeOpacity: 0.04

        Item {
            anchors.fill: parent
            clip: true

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                height: 6
                color: "transparent"

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.SizeVerCursor
                    enabled: !root.collapsed
                    property real startMouseY: 0

                    onPressed: function(mouse) {
                        startMouseY = mouse.y
                        root.dragStartHeight = root.expandedHeight
                    }

                    onPositionChanged: function(mouse) {
                        if (!pressed || !root.panelManager)
                            return
                        var nextHeight = Math.max(18, root.dragStartHeight - (mouse.y - startMouseY))
                        root.panelManager.resizePanel("status-bar", root.width, nextHeight)
                    }
                }
            }

            Row {
                id: leftMetrics
                visible: !root.collapsed
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                width: Math.max(200, Math.min(parent.width * 0.52, 520))
                clip: true
                spacing: 10

                Text {
                    width: 152
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Долгота: " + Number(root.appState ? root.appState.cursorLon : 0).toFixed(5)
                    color: Theme.ink(0.76)
                    font.pixelSize: 11
                    elide: Text.ElideRight
                }

                Text {
                    width: 150
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Широта: " + Number(root.appState ? root.appState.cursorLat : 0).toFixed(5)
                    color: Theme.ink(0.76)
                    font.pixelSize: 11
                    elide: Text.ElideRight
                }

                Text {
                    width: 116
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Масштаб: " + root.approximateScale()
                    color: Theme.ink(0.56)
                    font.pixelSize: 11
                    elide: Text.ElideRight
                }
            }

            Text {
                id: statusText
                visible: !root.collapsed
                anchors.left: leftMetrics.right
                anchors.leftMargin: 12
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text: "Статус: " + (root.appState ? root.appState.statusMessage : "")
                color: Theme.ink(0.50)
                font.pixelSize: 13
                elide: Text.ElideRight
            }
        }
    }
}

