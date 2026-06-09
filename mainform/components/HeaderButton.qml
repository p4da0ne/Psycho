import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/theme"

Button {
    id: headerButton

    property string label: ""
    property real rotationValue: 0
    implicitWidth: 28
    implicitHeight: 28
    flat: true
    background: Rectangle {
        radius: 14
        color: Theme.white
        opacity: headerButton.down ? 0.08 : headerButton.hovered ? 0.05 : 0.03
    }
    contentItem: Text {
        text: headerButton.label
        rotation: headerButton.rotationValue
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: Theme.textSoft3
        font.pixelSize: 15
    }
}
