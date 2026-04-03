import QtQuick 2.15
import QtQuick.Layouts 1.15

Item {
    property string icon:      ""
    property string label:     ""
    property real   minWidth:  100
    property bool   highlight: false

    Layout.preferredWidth: minWidth
    Layout.fillHeight: true

    Row {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 6
        spacing: 5

        Text {
            text: icon
            color: highlight ? "#4a9eff" : "#555555"
            font.pixelSize: 11
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            text: label
            color: highlight ? "#4a9eff" : "#aaaaaa"
            font.pixelSize: 11
            font.family: "Menlo"
        }
    }
}
