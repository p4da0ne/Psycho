import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root

    property var appState
    property Item backdropSource

    implicitHeight: 58
    opacity: appState && appState.controlsVisible ? 1 : 0.10
    y: appState && appState.controlsVisible ? 0 : -12

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
        width: field.activeFocus || field.text.length > 0 ? Math.min(root.width, 560) : Math.min(root.width, 460)
        height: 48
        radius: 24
        padding: 0
        backdropSource: root.backdropSource
        surfaceColor: "#141c24"
        surfaceOpacity: field.activeFocus ? 0.58 : 0.52
        shadowOpacity: 0.08
        highlightOpacity: 0.05

        Behavior on width {
            SpringAnimation {
                spring: 3.8
                damping: 0.36
            }
        }

        Row {
            anchors.fill: parent
            anchors.leftMargin: 16
            anchors.rightMargin: 10
            spacing: 12

            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "⌕"
                color: "#d9e2ed"
                font.pixelSize: 14
                opacity: 0.80
            }

            TextField {
                id: field

                width: searchShell.width - (clearButton.visible ? 86 : 54)
                height: parent.height
                placeholderText: "Search"
                placeholderTextColor: "#95a2b1"
                text: root.appState ? root.appState.searchText : ""
                color: Qt.rgba(1, 1, 1, 0.9)
                font.pixelSize: 13
                font.weight: Font.Normal
                leftPadding: 0
                rightPadding: 0
                verticalAlignment: Text.AlignVCenter
                background: Item {}
                selectByMouse: true

                onTextChanged: if (root.appState) root.appState.searchText = text
            }

            Button {
                id: clearButton

                visible: field.text.length > 0
                anchors.verticalCenter: parent.verticalCenter
                implicitWidth: 28
                implicitHeight: 28
                flat: true
                background: Rectangle {
                    radius: 14
                    color: "#ffffff"
                    opacity: clearButton.down ? 0.09 : clearButton.hovered ? 0.06 : 0.04
                }
                contentItem: Text {
                    text: "×"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: "#d7e0ea"
                    font.pixelSize: 16
                }
                onClicked: {
                    field.text = ""
                    root.appState.searchText = ""
                }
            }
        }
    }
}
