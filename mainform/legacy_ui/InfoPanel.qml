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

    function sideCount(sideName) {
        var count = 0
        for (var i = 0; i < objects.length; ++i) {
            if (objects[i].side === sideName)
                count += 1
        }
        return count
    }

    ScrollView {
        anchors.fill: parent
        clip: true

        Column {
            width: root.width - 24
            spacing: 12
            padding: 12

            Rectangle {
                width: parent.width
                height: 100
                radius: 18
                color: "#fcfaf6"
                border.color: "#dfd8cb"
                border.width: 1

                Column {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 6

                    Text {
                        text: "Сводка сеанса"
                        color: "#2a2f35"
                        font.pixelSize: 14
                        font.bold: true
                    }

                    Text {
                        text: "Режим: " + (appState.mapMode === "point" ? "Точечный" : "Heatmap")
                        color: "#7d776f"
                        font.pixelSize: 11
                    }

                    Text {
                        text: "Центр: " + Number(appState.centerLon).toFixed(4) + ", " + Number(appState.centerLat).toFixed(4)
                        color: "#5f6d79"
                        font.family: "Menlo"
                        font.pixelSize: 11
                    }

                    Text {
                        text: appState.statusMessage
                        color: "#50555b"
                        wrapMode: Text.WordWrap
                        font.pixelSize: 11
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
                        text: "Объекты в сцене"
                        color: "#2a2f35"
                        font.pixelSize: 14
                        font.bold: true
                    }

                    Row {
                        spacing: 8

                        Rectangle {
                            width: 84
                            height: 30
                            radius: 10
                            color: "#f1efea"
                            Text {
                                anchors.centerIn: parent
                                text: "Свои " + root.sideCount("friendly")
                                color: "#1f2937"
                                font.pixelSize: 11
                            }
                        }

                        Rectangle {
                            width: 84
                            height: 30
                            radius: 10
                            color: "#f3ece9"
                            Text {
                                anchors.centerIn: parent
                                text: "Чужие " + root.sideCount("foreign")
                                color: "#7c4b45"
                                font.pixelSize: 11
                            }
                        }

                        Rectangle {
                            width: 104
                            height: 30
                            radius: 10
                            color: "#f2eee6"
                            Text {
                                anchors.centerIn: parent
                                text: "ЛБС " + root.sideCount("infrastructure")
                                color: "#8b7355"
                                font.pixelSize: 11
                            }
                        }
                    }

                    Text {
                        text: "Данные пока демонстрационные, но слой интерфейса уже готов под реальную БД и провайдер карты."
                        color: "#5d635f"
                        wrapMode: Text.WordWrap
                        font.pixelSize: 11
                    }
                }
            }

            Rectangle {
                width: parent.width
                height: appState.selectedObject ? 186 : 110
                radius: 18
                color: "#fcfaf6"
                border.color: "#dfd8cb"
                border.width: 1

                Column {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 6

                    Text {
                        text: "Текущее выделение"
                        color: "#2a2f35"
                        font.pixelSize: 14
                        font.bold: true
                    }

                    Text {
                        text: appState.selectedObject ? appState.selectedObject.name : "Объект не выбран"
                        color: appState.selectedObject ? "#171b20" : "#8a847c"
                        font.pixelSize: 12
                    }

                    Text {
                        visible: appState.selectedObject
                        text: "ID: " + appState.selectedObject.id
                        color: "#5f6d79"
                        font.family: "Menlo"
                        font.pixelSize: 11
                    }

                    Text {
                        visible: appState.selectedObject
                        text: "Тип: " + appState.selectedObject.kind + " | Сторона: " + appState.selectedObject.side
                        color: "#5d635f"
                        font.pixelSize: 11
                    }

                    Text {
                        visible: appState.selectedObject
                        text: "Координаты: "
                              + Number(appState.selectedObject.lon).toFixed(5)
                              + ", "
                              + Number(appState.selectedObject.lat).toFixed(5)
                        color: "#5d635f"
                        font.family: "Menlo"
                        font.pixelSize: 11
                    }

                    Text {
                        visible: appState.selectedObject
                        text: "МППС: " + appState.selectedObject.mpps + " | Скорость: " + appState.selectedObject.speed
                        color: "#5d635f"
                        font.pixelSize: 11
                    }

                    Text {
                        visible: appState.selectedObject
                        text: appState.selectedObject ? appState.selectedObject.notes : ""
                        color: "#454b51"
                        wrapMode: Text.WordWrap
                        font.pixelSize: 11
                    }
                }
            }

            Rectangle {
                width: parent.width
                height: 136
                radius: 18
                color: "#fcfaf6"
                border.color: "#dfd8cb"
                border.width: 1

                Column {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 8

                    Text {
                        text: "Heatmap легенда"
                        color: "#2a2f35"
                        font.pixelSize: 14
                        font.bold: true
                    }

                    Rectangle {
                        width: parent.width - 6
                        height: 16
                        radius: 8
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: "#22c55e" }
                            GradientStop { position: 0.45; color: "#eab308" }
                            GradientStop { position: 0.7; color: "#f59e0b" }
                            GradientStop { position: 1.0; color: "#ef4444" }
                        }
                    }

                    Row {
                        width: parent.width
                        spacing: 0

                        Text {
                            text: "Низкий"
                            color: "#8a847c"
                            font.pixelSize: 10
                        }

                        Item {
                            width: parent.width - 90
                            height: 1
                        }

                        Text {
                            text: "Высокий"
                            color: "#8a847c"
                            font.pixelSize: 10
                        }
                    }

                    Text {
                        text: "Идея для будущего: привязать heatmap к вычисляемому МППС по выборке из БД и тайм-срезу."
                        color: "#5d635f"
                        wrapMode: Text.WordWrap
                        font.pixelSize: 11
                    }
                }
            }
        }
    }
}
