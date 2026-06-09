import QtQuick 2.15
import "qrc:/theme"

Rectangle {
    id: infoField

    property string label: ""
    property string value: ""
    property bool accent: false

    implicitHeight: 34
    radius: 14
    color: Theme.ink(0.02)
    border.width: 1
    border.color: Theme.ink(0.03)

    Row {
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        spacing: 10

        Text {
            width: 82
            anchors.verticalCenter: parent.verticalCenter
            text: infoField.label
            color: Theme.ink(0.46)
            font.pixelSize: 11
            elide: Text.ElideRight
        }

        Text {
            width: Math.max(0, parent.width - 92)
            anchors.verticalCenter: parent.verticalCenter
            text: infoField.value
            color: infoField.accent ? Theme.ink(0.90) : Theme.ink(0.76)
            font.pixelSize: 12
            font.weight: infoField.accent ? Font.Medium : Font.Normal
            elide: Text.ElideRight
        }
    }
}
