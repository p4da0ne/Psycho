import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/theme"

Rectangle {
    id: editField

    property string label: ""
    property string value: ""
    signal valueEdited(string nextValue)

    implicitHeight: 48
    radius: 16
    color: Theme.ink(0.02)
    border.width: 1
    border.color: Theme.ink(0.03)

    Column {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 4

        Text {
            text: editField.label
            color: Theme.ink(0.46)
            font.pixelSize: 11
            font.weight: Font.Medium
        }

        TextField {
            width: parent.width
            text: editField.value
            color: Theme.ink(0.84)
            font.pixelSize: 12
            padding: 0
            background: Item {}
            onTextEdited: editField.valueEdited(text)
        }
    }
}
