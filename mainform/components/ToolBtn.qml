import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    property string icon:    ""
    property string tooltip: ""
    property bool   active:  false

    signal clicked()

    implicitWidth:  32
    implicitHeight: 32
    radius: 4
    color: active             ? "#4a9eff22"
         : hoverArea.containsMouse ? "#2e2e2e"
         : "transparent"
    border.color: active ? "#4a9eff" : "transparent"
    border.width: 1

    Text {
        anchors.centerIn: parent
        text: icon
        font.pixelSize: 14
        color: active ? "#4a9eff" : "#cccccc"
    }

    ToolTip.visible: hoverArea.containsMouse
    ToolTip.text:    tooltip
    ToolTip.delay:   500

    MouseArea {
        id: hoverArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: parent.clicked()
    }
}
