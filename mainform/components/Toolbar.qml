import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root

    property var appState
    property var agentHub
    property var panelManager
    property Item backdropSource

    implicitHeight: 50
    opacity: appState && appState.controlsVisible ? 1 : 0.88
    y: 0

    Behavior on opacity {
        NumberAnimation {
            duration: 240
        }
    }

    Behavior on y {
        NumberAnimation {
            duration: 260
        }
    }

    GlassPanel {
        id: searchShell

        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: field.activeFocus || field.text.length > 0 ? Math.min(root.width, 540) : Math.min(root.width, 490)
        height: 42
        radius: 21
        padding: 0
        backdropSource: root.backdropSource
        surfaceColor: "#141c24"
        surfaceOpacity: field.activeFocus ? 0.52 : 0.46
        shadowOpacity: 0.05
        highlightOpacity: 0.04

        Behavior on width {
            SpringAnimation {
                spring: 3.8
                damping: 0.36
            }
        }

        Row {
            anchors.fill: parent
            anchors.leftMargin: 8
            anchors.rightMargin: 8
            spacing: 8

            ToolButton {
                id: leftToggle

                anchors.verticalCenter: parent.verticalCenter
                implicitWidth: 86
                implicitHeight: 26
                text: "Навигация"
                font.pixelSize: 10
                hoverEnabled: true
                background: Rectangle {
                    radius: 13
                    color: "#ffffff"
                    opacity: leftToggle.down ? 0.09 : leftToggle.hovered ? 0.06 : 0.04
                }
                contentItem: Text {
                    text: leftToggle.text
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: "#d7e0ea"
                    font.pixelSize: 10
                    elide: Text.ElideRight
                }
                ToolTip.visible: hovered
                ToolTip.text: "Показать или скрыть навигацию"
                onClicked: {
                    if (root.agentHub && root.agentHub.uiStateAgent)
                        root.agentHub.uiStateAgent.toggleNavigationVisible()
                }
            }

            Text {
                id: searchIcon
                anchors.verticalCenter: parent.verticalCenter
                text: "⌕"
                color: "#d9e2ed"
                font.pixelSize: 13
                opacity: 0.80
            }

            TextField {
                id: field

                width: Math.max(96,
                                searchShell.width
                                - leftToggle.width
                                - rightToggle.width
                                - statusToggle.width
                                - searchIcon.width
                                - (clearButton.visible ? 36 : 10)
                                - 34)
                height: parent.height
                placeholderText: "Поиск"
                placeholderTextColor: "#91a0b0"
                text: root.appState ? root.appState.searchText : ""
                color: Qt.rgba(1, 1, 1, 0.88)
                font.pixelSize: 12
                font.weight: Font.Normal
                leftPadding: 0
                rightPadding: 0
                verticalAlignment: Text.AlignVCenter
                background: Item {}
                selectByMouse: true

                onTextChanged: {
                    if (root.agentHub && root.agentHub.uiStateAgent)
                        root.agentHub.uiStateAgent.setSearchText(text)
                }
                onAccepted: {
                    if (root.agentHub && root.agentHub.uiStateAgent)
                        root.agentHub.uiStateAgent.focusSearchResult()
                }
            }

            Button {
                id: clearButton

                visible: field.text.length > 0
                anchors.verticalCenter: parent.verticalCenter
                implicitWidth: 24
                implicitHeight: 24
                flat: true
                background: Rectangle {
                    radius: 12
                    color: "#ffffff"
                    opacity: clearButton.down ? 0.09 : clearButton.hovered ? 0.06 : 0.04
                }
                contentItem: Text {
                    text: "×"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: "#d7e0ea"
                    font.pixelSize: 14
                }
                onClicked: {
                    field.text = ""
                    if (root.agentHub && root.agentHub.uiStateAgent)
                        root.agentHub.uiStateAgent.setSearchText("")
                }
            }

            ToolButton {
                id: rightToggle

                anchors.verticalCenter: parent.verticalCenter
                implicitWidth: 82
                implicitHeight: 26
                text: "Инспектор"
                font.pixelSize: 10
                hoverEnabled: true
                background: Rectangle {
                    radius: 13
                    color: "#ffffff"
                    opacity: rightToggle.down ? 0.09 : rightToggle.hovered ? 0.06 : 0.04
                }
                contentItem: Text {
                    text: rightToggle.text
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: "#d7e0ea"
                    font.pixelSize: 10
                    elide: Text.ElideRight
                }
                ToolTip.visible: hovered
                ToolTip.text: "Показать или скрыть инспектор"
                onClicked: {
                    if (root.agentHub && root.agentHub.uiStateAgent)
                        root.agentHub.uiStateAgent.toggleInspectorVisible()
                }
            }

            ToolButton {
                id: statusToggle

                anchors.verticalCenter: parent.verticalCenter
                implicitWidth: 64
                implicitHeight: 26
                text: "Статус"
                font.pixelSize: 10
                hoverEnabled: true
                background: Rectangle {
                    radius: 13
                    color: "#ffffff"
                    opacity: statusToggle.down ? 0.09 : statusToggle.hovered ? 0.06 : 0.04
                }
                contentItem: Text {
                    text: statusToggle.text
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: "#d7e0ea"
                    font.pixelSize: 10
                    elide: Text.ElideRight
                }
                ToolTip.visible: hovered
                ToolTip.text: "Показать или скрыть строку состояния"
                onClicked: {
                    if (!root.panelManager)
                        return
                    var panel = root.panelManager.panelById("status-bar")
                    root.panelManager.setPanelVisible("status-bar", !(panel.visible === true))
                }
            }
        }
    }
}
