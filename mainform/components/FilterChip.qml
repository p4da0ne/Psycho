import QtQuick 2.15
import "qrc:/theme"

Rectangle {
    id: filterChip
    property string text: ""
    property bool checked: false
    signal clicked()

    width: chipText.implicitWidth + 16
    height: 28
    radius: 14
    color: checked ? Theme.ink(0.10) : Theme.ink(0.03)
    border.width: 1
    border.color: checked ? Theme.ink(0.12) : Theme.ink(0.04)

    Text {
        id: chipText
        anchors.centerIn: parent
        text: filterChip.text
        color: filterChip.checked ? Theme.ink(0.90) : Theme.ink(0.62)
        font.pixelSize: 12
        font.weight: filterChip.checked ? Font.Medium : Font.Normal
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: filterChip.clicked()
    }
}
