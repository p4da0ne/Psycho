pragma ComponentBehavior: Bound
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root

    property var appState
    property var panelManager
    property var panelState: ({})
    property real dragStartHeight: 0

    readonly property bool panelVisible: panelState && panelState.visible !== undefined ? panelState.visible : true
    readonly property bool collapsed: panelState && panelState.collapsed ? panelState.collapsed : false
    readonly property real expandedHeight: panelState && panelState.height ? panelState.height : 36
    readonly property real targetHeight: panelVisible ? (collapsed ? 28 : expandedHeight) : 0

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
        surfaceColor: "#141c24"
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
                        var nextHeight = Math.max(28, root.dragStartHeight - (mouse.y - startMouseY))
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
                    color: Qt.rgba(1, 1, 1, 0.76)
                    font.pixelSize: 13
                    elide: Text.ElideRight
                }

                Text {
                    width: 150
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Широта: " + Number(root.appState ? root.appState.cursorLat : 0).toFixed(5)
                    color: Qt.rgba(1, 1, 1, 0.76)
                    font.pixelSize: 13
                    elide: Text.ElideRight
                }

                Text {
                    width: 116
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Масштаб: " + root.approximateScale()
                    color: Qt.rgba(1, 1, 1, 0.56)
                    font.pixelSize: 13
                    elide: Text.ElideRight
                }
            }

            Text {
                id: statusText
                visible: !root.collapsed
                anchors.left: leftMetrics.right
                anchors.leftMargin: 12
                anchors.right: actionRow.left
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text: "Статус: " + (root.appState ? root.appState.statusMessage : "")
                color: Qt.rgba(1, 1, 1, 0.50)
                font.pixelSize: 13
                elide: Text.ElideRight
            }

            Row {
                id: actionRow
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                spacing: 6

                Button {
                    id: collapseButton
                    implicitWidth: 28
                    implicitHeight: 28
                    flat: true
                    background: Rectangle {
                        radius: 12
                        color: "#ffffff"
                        opacity: collapseButton.down ? 0.08 : collapseButton.hovered ? 0.05 : 0.03
                    }
                    contentItem: Text {
                        text: root.collapsed ? "▴" : "▾"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: "#dce4ed"
                        font.pixelSize: 14
                    }
                    onClicked: root.panelManager.toggleCollapsed("status-bar")
                }

                Button {
                    id: closeButton
                    implicitWidth: 28
                    implicitHeight: 28
                    flat: true
                    background: Rectangle {
                        radius: 12
                        color: "#ffffff"
                        opacity: closeButton.down ? 0.08 : closeButton.hovered ? 0.05 : 0.03
                    }
                    contentItem: Text {
                        text: "×"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: "#dce4ed"
                        font.pixelSize: 14
                    }
                    onClicked: root.panelManager.setPanelVisible("status-bar", false)
                }
            }
        }
    }
}

