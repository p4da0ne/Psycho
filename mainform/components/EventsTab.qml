import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/theme"

// Вкладка "События" правой панели. host = RightSidebar (root).

Column {
    property var host

    spacing: 10

    SectionTitle { text: "Список событий" }

    Rectangle {
        width: parent.width
        height: 98
        radius: 18
        color: Theme.ink(0.018)
        border.width: 1
        border.color: Theme.ink(0.03)

        Column {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 6

            Flow {
                width: parent.width
                spacing: 6

                Repeater {
                    model: host.eventStatusOptions

                    delegate: FilterChip {
                        required property var modelData
                        text: modelData.label
                        checked: host.appState && host.appState.eventStatusFilter === modelData.value
                        onClicked: host.appState.eventStatusFilter = modelData.value
                    }
                }
            }

            Flow {
                width: parent.width
                spacing: 6

                Repeater {
                    model: host.eventTypeOptions

                    delegate: FilterChip {
                        required property var modelData
                        text: modelData.label || ("Тип #" + modelData.idTypeEvent)
                        checked: host.appState && Number(host.appState.eventTypeFilterId || 0) === Number(modelData.idTypeEvent || 0)
                        onClicked: host.appState.eventTypeFilterId = Number(modelData.idTypeEvent || 0)
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
            model: host.filteredEvents
            boundsBehavior: Flickable.StopAtBounds

            delegate: Rectangle {
                id: eventRow

                required property var modelData

                readonly property bool selected: host.selectionAgent && host.selectionAgent.isEventSelected(eventRow.modelData)

                width: ListView.view.width
                height: 54
                radius: 14
                color: eventRow.selected ? Theme.ink(0.06) : Theme.ink(0.02)
                border.width: 1
                border.color: eventRow.selected ? Theme.ink(0.09) : Theme.ink(0.028)

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
                        color: host.statusColor(eventRow.modelData.status)
                    }

                    Column {
                        anchors.verticalCenter: parent.verticalCenter
                        width: Math.max(0, parent.width - 26)
                        spacing: 2

                        Text {
                            width: parent.width
                            text: eventRow.modelData.name
                            color: eventRow.selected ? Theme.ink(0.92) : Theme.ink(0.76)
                            font.pixelSize: 13
                            font.weight: eventRow.selected ? Font.Medium : Font.Normal
                            elide: Text.ElideRight
                        }

                        Text {
                            width: parent.width
                            text: host.typeIcon(eventRow.modelData.type) + " " + host.typeLabel(eventRow.modelData.type) + " · " + host.statusLabel(eventRow.modelData.status)
                            color: Theme.ink(0.44)
                            font.pixelSize: 11
                            elide: Text.ElideRight
                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: host.selectionAgent.selectEvent(eventRow.modelData)
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
            contentHeight: eventDetails.implicitHeight
            boundsBehavior: Flickable.StopAtBounds

            Column {
                id: eventDetails

                width: parent.width
                spacing: 10

                Text {
                    width: parent.width
                    text: host.selectedEvent && host.selectedEvent.name ? host.selectedEvent.name : "Выберите событие"
                    color: Theme.ink(0.92)
                    font.pixelSize: 16
                    font.weight: Font.Medium
                    elide: Text.ElideRight
                }

                Text {
                    width: parent.width
                    text: host.selectedEvent && host.selectedEvent.type
                        ? host.typeIcon(host.selectedEvent.type) + " " + host.typeLabel(host.selectedEvent.type) + " · " + host.statusLabel(host.selectedEvent.status)
                        : "Выберите событие выше для просмотра деталей"
                    color: Theme.ink(0.52)
                    font.pixelSize: 12
                    wrapMode: Text.WordWrap
                }

                Rectangle {
                    visible: host.selectedEvent && host.eventLegacyRows().length > 0
                    width: parent.width
                    implicitHeight: eventLegacyTitle.implicitHeight + eventLegacyRows.implicitHeight + 14
                    radius: 14
                    color: Theme.ink(0.018)
                    border.width: 1
                    border.color: Theme.ink(0.03)

                    Column {
                        anchors.fill: parent
                        anchors.margins: 7
                        spacing: 6

                        Text {
                            id: eventLegacyTitle
                            width: parent.width
                            text: "Ключевые поля события"
                            color: Theme.ink(0.86)
                            font.pixelSize: 12
                            font.weight: Font.Medium
                        }

                        Column {
                            id: eventLegacyRows
                            width: parent.width
                            spacing: 4

                            Repeater {
                                model: host.eventLegacyRows()

                                delegate: InfoField {
                                    width: eventLegacyRows.width
                                    label: modelData.label
                                    value: modelData.value
                                }
                            }
                        }
                    }
                }

                InfoField { label: "Начало"; value: host.selectedEvent && host.selectedEvent.startTimestamp ? host.selectedEvent.startTimestamp : "—" }
                InfoField { label: "Окончание"; value: host.selectedEvent && host.selectedEvent.endTimestamp ? host.selectedEvent.endTimestamp : "—" }
                InfoField { label: "Обновлено"; value: host.selectedEvent && host.selectedEvent.updatedTimestamp ? host.selectedEvent.updatedTimestamp : "—" }
                InfoField { label: "Связи"; value: host.selectedEvent && host.selectedEvent.objectIds ? String(host.selectedEvent.objectIds.length) + " объектов" : "—" }
                InfoField { label: "ID типа"; value: host.selectedEvent && host.selectedEvent.idTypeEvent ? String(host.selectedEvent.idTypeEvent) : "—" }
                InfoField { label: "ID статуса"; value: host.selectedEvent && host.selectedEvent.idEventStatus ? String(host.selectedEvent.idEventStatus) : "—" }
                InfoField { label: "ID знака"; value: host.selectedEvent && host.selectedEvent.idSign ? String(host.selectedEvent.idSign) : "—" }
                InfoField { label: "Код знака"; value: host.selectedEvent && host.selectedEvent.signKey ? String(host.selectedEvent.signKey) : "—" }

                Text {
                    width: parent.width
                    text: host.selectedEvent && host.selectedEvent.description ? host.selectedEvent.description : ""
                    visible: text.length > 0
                    color: Theme.ink(0.60)
                    font.pixelSize: 12
                    lineHeight: 1.24
                    wrapMode: Text.WordWrap
                }

                Rectangle {
                    width: parent.width
                    implicitHeight: Math.max(140, Math.min(320, eventDetailsList.contentHeight + 14))
                    radius: 14
                    color: Theme.ink(0.018)
                    border.width: 1
                    border.color: Theme.ink(0.03)
                    visible: host.selectedEvent && host.selectedEvent.id !== undefined

                    ListView {
                        id: eventDetailsList
                        anchors.fill: parent
                        anchors.margins: 6
                        clip: true
                        spacing: 4
                        model: host.eventDetailsRows()

                        delegate: Rectangle {
                            id: eventDetailRow
                            required property var modelData

                            width: ListView.view.width
                            implicitHeight: Math.max(32, detailKey.implicitHeight + detailValue.implicitHeight + 10)
                            radius: 10
                            color: Theme.ink(0.02)
                            border.width: 1
                            border.color: Theme.ink(0.025)

                            Column {
                                anchors.fill: parent
                                anchors.margins: 6
                                spacing: 2

                                Text {
                                    id: detailKey
                                    width: parent.width
                                    text: eventDetailRow.modelData.label
                                    color: Theme.ink(0.46)
                                    font.pixelSize: 11
                                    wrapMode: Text.WrapAnywhere
                                }

                                Text {
                                    id: detailValue
                                    width: parent.width
                                    text: eventDetailRow.modelData.value
                                    color: Theme.ink(0.80)
                                    font.pixelSize: 12
                                    wrapMode: Text.WrapAnywhere
                                }
                            }
                        }

                        ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded; width: 6 }
                    }
                }
            }

            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded; width: 6 }
        }
    }
}
