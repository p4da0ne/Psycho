import QtQuick 2.15

Item {
    property string iconText: ""
    property string label:    ""
    property string shortcut: ""
    property bool   danger:   false
    property string hint:     ""

    implicitHeight: 28
    implicitWidth:  260

    Row {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 8
        spacing: 8

        Text {
            text: iconText
            color: danger ? "#ff5555" : "#666666"
            font.pixelSize: 12
            width: 16
            horizontalAlignment: Text.AlignHCenter
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            text: label
            color: danger ? "#ff5555" : "#dddddd"
            font.pixelSize: 12
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Text {
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 10
        text: hint !== "" ? hint : shortcut
        color: hint !== "" ? "#4a9eff" : "#555555"
        font.pixelSize: 10
        font.family: "Menlo"
    }
}
