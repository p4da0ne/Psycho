import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: root

    radius: 24
    color: "#f7f4ee"
    border.color: "#d5cfc4"
    border.width: 1

    property var appState
    property var objects: []
    property int visibleCount: 0
    property int averageMppsValue: 0

    ScrollView {
        anchors.fill: parent
        clip: true

        Column {
            width: root.width - 24
            spacing: 12
            padding: 12

            Text {
                text: "Слои и режимы"
                color: "#191d22"
                font.pixelSize: 16
                font.bold: true
            }

            Rectangle {
                width: parent.width
                height: 164
                radius: 18
                color: "#fcfaf6"
                border.color: "#dfd8cb"
                border.width: 1

                Column {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 6

                    Text {
                        text: "Отображение"
                        color: "#2a2f35"
                        font.pixelSize: 13
                        font.bold: true
                    }

                    Switch {
                        text: "Свои объекты"
                        checked: appState && appState.showFriendly
                        onToggled: appState.showFriendly = checked
                    }

                    Switch {
                        text: "Чужие объекты"
                        checked: appState && appState.showForeign
                        onToggled: appState.showForeign = checked
                    }

                    Switch {
                        text: "Линия ЛБС"
                        checked: appState && appState.showCoverageLine
                        onToggled: appState.showCoverageLine = checked
                    }

                    Switch {
                        text: "Базовые станции"
                        checked: appState && appState.showBaseStations
                        onToggled: appState.showBaseStations = checked
                    }

                    Switch {
                        text: "Подписи"
                        checked: appState && appState.showLabels
                        onToggled: appState.showLabels = checked
                    }
                }
            }

            Rectangle {
                width: parent.width
                height: 122
                radius: 18
                color: "#fcfaf6"
                border.color: "#dfd8cb"
                border.width: 1

                Column {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 8

                    Text {
                        text: "Режим карты"
                        color: "#2a2f35"
                        font.pixelSize: 13
                        font.bold: true
                    }

                    ButtonGroup {
                        id: modeGroup
                    }

                    RadioButton {
                        text: "Точечный режим"
                        ButtonGroup.group: modeGroup
                        checked: appState && appState.mapMode === "point"
                        onClicked: appState.mapMode = "point"
                    }

                    RadioButton {
                        text: "Heatmap режим"
                        ButtonGroup.group: modeGroup
                        checked: appState && appState.mapMode === "heatmap"
                        onClicked: appState.mapMode = "heatmap"
                    }

                    CheckBox {
                        text: "Показывать heatmap слой"
                        checked: appState && appState.showHeatmapLayer
                        onToggled: appState.showHeatmapLayer = checked
                    }
                }
            }

            Rectangle {
                width: parent.width
                height: 112
                radius: 18
                color: "#fcfaf6"
                border.color: "#dfd8cb"
                border.width: 1

                Column {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 8

                    Text {
                        text: "Базовая сводка"
                        color: "#2a2f35"
                        font.pixelSize: 13
                        font.bold: true
                    }

                    Text {
                        text: "Видимых объектов: " + visibleCount
                        color: "#50555b"
                        font.pixelSize: 11
                    }

                    Text {
                        text: "Средний МППС: " + averageMppsValue
                        color: "#50555b"
                        font.pixelSize: 11
                    }

                    Text {
                        text: "После рефакторинга БД сюда можно вынести реальную структуру слоев."
                        color: "#868078"
                        wrapMode: Text.WordWrap
                        font.pixelSize: 11
                    }
                }
            }
        }
    }
}
