import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/theme"

// Вкладка "Инспектор" (список объектов + слои + детали). host = RightSidebar (root).

Column {
    property var host

    spacing: 10

    SectionTitle {
        text: host.structureMode
            ? "Выбранный узел"
            : "Список объектов (" + (host.filteredObjects ? host.filteredObjects.length : 0) + ")"
    }

    Rectangle {
        width: parent.width
        implicitHeight: layersCardColumn.implicitHeight + 20
        radius: 18
        color: Theme.ink(0.018)
        border.width: 1
        border.color: Theme.ink(0.03)

        Column {
            id: layersCardColumn
            anchors.fill: parent
            anchors.margins: 10
            spacing: 8

            Text {
                text: "Карта и слои"
                color: Theme.ink(0.50)
                font.pixelSize: 11
                font.weight: Font.Medium
            }

            // Режим: точки vs тепловая карта (слой heat завязан на этот же переключатель).
            Row {
                spacing: 8
                anchors.left: parent.left
                anchors.right: parent.right

                Text {
                    text: "Режим"
                    color: Theme.ink(0.55)
                    font.pixelSize: 11
                    width: 72
                    anchors.verticalCenter: parent.verticalCenter
                }

                Row {
                    spacing: 6
                    anchors.verticalCenter: parent.verticalCenter

                    FilterChip {
                        text: "Точки"
                        checked: host.appState && host.appState.mapMode === "point"
                        onClicked: {
                            if (host.appState)
                                host.appState.mapMode = "point"
                        }
                    }

                    FilterChip {
                        text: "Тепловая карта"
                        checked: host.appState && host.appState.mapMode === "heatmap"
                        onClicked: {
                            if (host.appState)
                                host.appState.mapMode = "heatmap"
                        }
                    }
                }
            }

            Rectangle {
                width: parent.width
                height: 1
                color: Theme.ink(0.05)
            }

            Row {
                spacing: 8
                anchors.left: parent.left
                anchors.right: parent.right

                Text {
                    text: "Видимость"
                    color: Theme.ink(0.55)
                    font.pixelSize: 11
                    width: 72
                    anchors.verticalCenter: parent.verticalCenter
                }

                Row {
                    spacing: 6
                    anchors.verticalCenter: parent.verticalCenter

                    FilterChip {
                        text: "Линии"
                        checked: host.appState ? host.appState.showCoverageLine : true
                        onClicked: host.toggleBooleanState("showCoverageLine")
                    }

                    FilterChip {
                        text: "Подписи"
                        checked: host.appState ? host.appState.showLabels : true
                        onClicked: host.toggleBooleanState("showLabels")
                    }
                }
            }

            Rectangle {
                width: parent.width
                height: 1
                color: Theme.ink(0.05)
            }

            Row {
                spacing: 8
                anchors.left: parent.left
                anchors.right: parent.right

                Text {
                    text: "Стиль"
                    color: Theme.ink(0.55)
                    font.pixelSize: 11
                    width: 72
                    anchors.verticalCenter: parent.verticalCenter
                }

                Row {
                    spacing: 6
                    anchors.verticalCenter: parent.verticalCenter

                    FilterChip {
                        text: "MapTiler Basic"
                        checked: host.appState && host.appState.mapStyleName === "maptiler-basic"
                        onClicked: {
                            if (host.appState)
                                host.appState.mapStyleName = "maptiler-basic"
                        }
                    }

                    FilterChip {
                        text: "OSM Bright"
                        checked: host.appState && host.appState.mapStyleName === "osm-bright"
                        onClicked: {
                            if (host.appState)
                                host.appState.mapStyleName = "osm-bright"
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        width: parent.width
        height: Math.max(170, parent.height * 0.38)
        radius: 18
        color: Theme.ink(0.018)
        border.width: 1
        border.color: Theme.ink(0.03)

        ListView {
            anchors.fill: parent
            anchors.margins: 8
            clip: true
            spacing: 4
            model: host.structureMode
                ? (host.selectedStructureNode && host.selectedStructureNode.id ? [host.selectedStructureNode] : [])
                : host.filteredObjects
            boundsBehavior: Flickable.StopAtBounds

            delegate: Rectangle {
                id: objectRow

                required property var modelData

                readonly property bool selected: host.structureMode
                    ? true
                    : host.selectionAgent && host.selectionAgent.isSelected(objectRow.modelData)

                width: ListView.view.width
                height: 48
                radius: 14
                color: objectRow.selected ? Theme.ink(0.06) : Theme.ink(0.02)
                border.width: 1
                border.color: objectRow.selected ? Theme.ink(0.09) : Theme.ink(0.028)

                Row {
                    anchors.fill: parent
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    spacing: 8

                    Rectangle {
                        anchors.verticalCenter: parent.verticalCenter
                        width: 8
                        height: 8
                        radius: 4
                        color: host.structureMode ? Theme.structureDot : host.agentHub.symbolAgent.markerColor(objectRow.modelData)
                    }

                    Column {
                        anchors.verticalCenter: parent.verticalCenter
                        width: Math.max(0, parent.width - 26)
                        spacing: 2

                        Text {
                            width: parent.width
                            text: host.structureMode
                                ? (objectRow.modelData.label || "Узел структуры")
                                : objectRow.modelData.name
                            color: objectRow.selected ? Theme.ink(0.92) : Theme.ink(0.76)
                            font.pixelSize: 13
                            font.weight: objectRow.selected ? Font.Medium : Font.Normal
                            elide: Text.ElideRight
                        }

                        Text {
                            width: parent.width
                            text: host.structureMode
                                ? ("Путь: " + (host.selectedStructurePath || "—"))
                                : (host.objectTypeLabel(objectRow.modelData.objectType) || "Объект") + " · МППС " + objectRow.modelData.mpps
                            color: Theme.ink(0.44)
                            font.pixelSize: 11
                            elide: Text.ElideRight
                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        if (host.structureMode)
                            return
                        var item = objectRow.modelData
                        // Центрируем карту: appState.center* → MyMapView.map.center (one-way binding).
                        if (host.appState && item) {
                            var lat = Number(item.lat)
                            var lon = Number(item.lon)
                            if (isFinite(lat) && isFinite(lon)) {
                                host.appState.centerLat = lat
                                host.appState.centerLon = lon
                            }
                        }
                        // Выбор объекта (триггерит loadObjectDetailsForObject → таблица "Детали").
                        if (host.selectionAgent)
                            host.selectionAgent.selectObject(item)
                    }
                }
            }

            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded; width: 6 }
        }
    }

    SectionTitle { text: "Детали" }

    Rectangle {
        width: parent.width
        height: Math.max(0, parent.height - y)
        radius: 18
        color: Theme.ink(0.018)
        border.width: 1
        border.color: Theme.ink(0.03)

        Flickable {
            anchors.fill: parent
            anchors.margins: 10
            clip: true
            contentWidth: width
            contentHeight: objectDetails.implicitHeight
            boundsBehavior: Flickable.StopAtBounds

            Column {
                id: objectDetails

                width: parent.width
                spacing: 10

                Text {
                    width: parent.width
                    text: host.structureMode
                        ? (host.selectedStructureNode && host.selectedStructureNode.label ? host.selectedStructureNode.label : "Выберите узел структуры")
                        : (host.selectedObject && host.selectedObject.name ? host.selectedObject.name : "Выберите объект")
                    color: Theme.ink(0.92)
                    font.pixelSize: 16
                    font.weight: Font.Medium
                    elide: Text.ElideRight
                }

                Text {
                    width: parent.width
                    text: host.structureMode
                        ? "Узел структуры и связанные объекты"
                        : "Данные объекта из БД (табличный режим)"
                    color: Theme.ink(0.52)
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                }

                InfoField {
                    visible: host.structureMode
                    label: "Путь"
                    value: host.selectedStructurePath || "—"
                }
                InfoField {
                    visible: host.structureMode
                    label: "Тип"
                    value: host.structureKindLabel(host.selectedStructureNode)
                }
                InfoField {
                    visible: host.structureMode
                    label: "Дочерних узлов"
                    value: host.selectedStructureNode && host.selectedStructureNode.children
                        ? String(host.selectedStructureNode.children.length)
                        : "0"
                }

                Rectangle {
                    visible: !host.structureMode
                    width: parent.width
                    implicitHeight: tableHeader.implicitHeight + tableRows.implicitHeight + 12
                    radius: 14
                    color: Theme.ink(0.015)
                    border.width: 1
                    border.color: Theme.ink(0.03)

                    Column {
                        anchors.fill: parent
                        anchors.margins: 6
                        spacing: 0

                        Row {
                            id: tableHeader
                            width: parent.width
                            height: 32

                            Rectangle {
                                width: Math.max(140, parent.width * 0.38)
                                height: parent.height
                                color: Theme.ink(0.06)

                                Text {
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    anchors.leftMargin: 8
                                    text: "Поле"
                                    color: Theme.ink(0.80)
                                    font.pixelSize: 14
                                    font.weight: Font.Medium
                                }
                            }

                            Rectangle {
                                width: parent.width - (Math.max(140, parent.width * 0.38))
                                height: parent.height
                                color: Theme.ink(0.06)

                                Text {
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    anchors.leftMargin: 8
                                    text: "Значение"
                                    color: Theme.ink(0.80)
                                    font.pixelSize: 14
                                    font.weight: Font.Medium
                                }
                            }
                        }

                        Column {
                            id: tableRows
                            width: parent.width
                            spacing: 0

                            Repeater {
                                model: host.readableDetailsRows()

                                delegate: Row {
                                    required property int index
                                    required property var modelData
                                    width: tableRows.width
                                    height: Math.max(30, valueText.implicitHeight + 8)

                                    Rectangle {
                                        width: Math.max(140, parent.width * 0.38)
                                        height: parent.height
                                        color: (index % 2 === 0) ? Theme.ink(0.025) : Theme.ink(0.01)

                                        Text {
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.left: parent.left
                                            anchors.leftMargin: 8
                                            anchors.right: parent.right
                                            anchors.rightMargin: 6
                                            text: modelData.label
                                            color: Theme.ink(0.74)
                                            font.pixelSize: 13
                                            elide: Text.ElideRight
                                        }
                                    }

                                    Rectangle {
                                        width: parent.width - (Math.max(140, parent.width * 0.38))
                                        height: parent.height
                                        color: (index % 2 === 0) ? Theme.ink(0.025) : Theme.ink(0.01)

                                        Text {
                                            id: valueText
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.left: parent.left
                                            anchors.leftMargin: 8
                                            anchors.right: parent.right
                                            anchors.rightMargin: 6
                                            text: modelData.value
                                            color: Theme.ink(0.92)
                                            font.pixelSize: 13
                                            wrapMode: Text.WordWrap
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                Text {
                    width: parent.width
                    text: host.structureMode
                        ? "Выберите объект на карте или в структуре для перехода к детальным параметрам."
                        : (host.readableDetailsRows().length === 0 ? "Нет данных БД для выбранного объекта." : "")
                    visible: text.length > 0
                    color: Theme.ink(0.60)
                    font.pixelSize: 12
                    lineHeight: 1.24
                    wrapMode: Text.WordWrap
                }
            }

            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded; width: 6 }
        }
    }
}
