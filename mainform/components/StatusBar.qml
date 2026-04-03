import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    radius: 18
    color: "#f7f4ee"
    border.color: "#d5cfc4"
    border.width: 1

    property var appState
    property int visibleCount: 0
    property int averageMppsValue: 0

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 12
        anchors.rightMargin: 12
        spacing: 16

        Text {
            text: "Lon " + Number(appState.cursorLon).toFixed(5) + "  Lat " + Number(appState.cursorLat).toFixed(5)
            color: "#40464d"
            font.family: "Menlo"
            font.pixelSize: 11
        }

        Text {
            text: "Zoom " + Number(appState.zoomLevel).toFixed(1)
            color: "#5f6d79"
            font.family: "Menlo"
            font.pixelSize: 11
        }

        Text {
            text: "Mode " + (appState.mapMode === "point" ? "point" : "heatmap")
            color: "#5d635f"
            font.pixelSize: 11
        }

        Text {
            text: "Visible " + visibleCount
            color: "#5d635f"
            font.pixelSize: 11
        }

        Text {
            text: "Avg MPPS " + averageMppsValue
            color: "#8b7355"
            font.pixelSize: 11
        }

        Item {
            Layout.fillWidth: true
        }

        Text {
            text: appState.selectedObject
                ? "Selected: " + appState.selectedObject.name
                : "Selected: none"
            color: appState.selectedObject ? "#23282e" : "#8a847c"
            font.pixelSize: 11
        }
    }
}
