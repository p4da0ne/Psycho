pragma ComponentBehavior: Bound
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root

    property var appState
    property var agentHub
    property var events: []
    property Item backdropSource

    readonly property var selectedEvent: appState ? appState.selectedEvent : ({})
    readonly property bool hasSelection: selectedEvent && selectedEvent.id !== undefined
    readonly property var statusOptions: [
        { "value": "all", "label": "All statuses", "icon": "◌" },
        { "value": "Actual", "label": "Actual", "icon": "●" },
        { "value": "Past", "label": "Past", "icon": "◔" },
        { "value": "Planned", "label": "Planned", "icon": "◷" },
        { "value": "Cancelled", "label": "Cancelled", "icon": "×" }
    ]
    readonly property var typeOptions: [
        { "value": "all", "label": "All types", "icon": "⌘" },
        { "value": "Operations", "label": "Operations", "icon": "◎" },
        { "value": "Communications", "label": "Communications", "icon": "≈" },
        { "value": "Monitoring", "label": "Monitoring", "icon": "◈" },
        { "value": "Logistics", "label": "Logistics", "icon": "⇄" },
        { "value": "Infrastructure", "label": "Infrastructure", "icon": "▣" }
    ]
    readonly property var filteredEvents: {
        if (!appState || !agentHub || !agentHub.filterAgent)
            return []
        appState.searchText
        appState.eventStatusFilter
        appState.eventTypeFilter
        return agentHub.filterAgent.visibleEvents(events || [])
    }
    readonly property int eventCount: filteredEvents ? filteredEvents.length : 0

    function statusColor(statusValue) {
        switch (statusValue) {
        case "Actual":
            return Qt.rgba(0.50, 0.72, 1.0, 1.0)
        case "Planned":
            return Qt.rgba(0.58, 0.66, 1.0, 1.0)
        case "Past":
            return Qt.rgba(0.58, 0.63, 0.68, 1.0)
        case "Cancelled":
            return Qt.rgba(0.71, 0.56, 0.58, 1.0)
        default:
            return Qt.rgba(0.56, 0.64, 0.73, 1.0)
        }
    }

    function typeIcon(typeValue) {
        switch (typeValue) {
        case "Operations":
            return "◎"
        case "Communications":
            return "≈"
        case "Monitoring":
            return "◈"
        case "Logistics":
            return "⇄"
        case "Infrastructure":
            return "▣"
        default:
            return "⌘"
        }
    }

    GlassPanel {
        anchors.fill: parent
        radius: 24
        padding: 0
        backdropSource: root.backdropSource
        surfaceColor: "#141c24"
        surfaceOpacity: 0.55
        shadowOpacity: 0.09
        highlightOpacity: 0.05
        edgeOpacity: 0.06

        Column {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 12

            Row {
                width: parent.width
                height: 32
                spacing: 10

                Column {
                    width: Math.max(0, parent.width - 44)
                    spacing: 2

                    Text {
                        width: parent.width
                        text: "Events"
                        color: Qt.rgba(1, 1, 1, 0.92)
                        font.pixelSize: 15
                        font.weight: Font.Medium
                        elide: Text.ElideRight
                    }

                    Text {
                        width: parent.width
                        text: root.eventCount + " visible · filter-driven list"
                        color: Qt.rgba(1, 1, 1, 0.58)
                        font.pixelSize: 10
                        elide: Text.ElideRight
                    }
                }

                Button {
                    id: clearButton

                    anchors.verticalCenter: parent.verticalCenter
                    implicitWidth: 28
                    implicitHeight: 28
                    flat: true
                    visible: root.hasSelection
                    background: Rectangle {
                        radius: 14
                        color: "#ffffff"
                        opacity: clearButton.down ? 0.08 : clearButton.hovered ? 0.05 : 0.03
                    }
                    contentItem: Text {
                        text: "×"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: "#dce4ed"
                        font.pixelSize: 14
                    }
                    onClicked: root.agentHub.selectionAgent.clearEventSelection()
                }
            }

            Rectangle {
                width: parent.width
                height: 1
                color: Qt.rgba(1, 1, 1, 0.05)
            }

            Column {
                width: parent.width
                spacing: 8

                Flow {
                    width: parent.width
                    spacing: 6

                    Repeater {
                        model: root.statusOptions

                        delegate: Rectangle {
                            id: statusChip

                            required property var modelData

                            height: 28
                            radius: 14
                            color: root.appState.eventStatusFilter === statusChip.modelData.value ? Qt.rgba(1, 1, 1, 0.10) : Qt.rgba(1, 1, 1, 0.03)
                            border.width: 1
                            border.color: root.appState.eventStatusFilter === statusChip.modelData.value
                                ? Qt.rgba(1, 1, 1, 0.12)
                                : Qt.rgba(1, 1, 1, 0.04)
                            opacity: 1
                            width: statusRow.implicitWidth + 18

                            Row {
                                id: statusRow

                                anchors.centerIn: parent
                                spacing: 6

                                Text {
                                    text: statusChip.modelData.icon
                                    color: root.appState.eventStatusFilter === statusChip.modelData.value
                                        ? root.statusColor(statusChip.modelData.value)
                                        : Qt.rgba(1, 1, 1, 0.54)
                                    font.pixelSize: 10
                                }

                                Text {
                                    text: statusChip.modelData.label
                                    color: root.appState.eventStatusFilter === statusChip.modelData.value
                                        ? Qt.rgba(1, 1, 1, 0.92)
                                        : Qt.rgba(1, 1, 1, 0.62)
                                    font.pixelSize: 10
                                    font.weight: root.appState.eventStatusFilter === statusChip.modelData.value ? Font.Medium : Font.Normal
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: root.appState.eventStatusFilter = statusChip.modelData.value
                            }
                        }
                    }
                }

                Flow {
                    width: parent.width
                    spacing: 6

                    Repeater {
                        model: root.typeOptions

                        delegate: Rectangle {
                            id: typeChip

                            required property var modelData

                            height: 28
                            radius: 14
                            color: root.appState.eventTypeFilter === typeChip.modelData.value ? Qt.rgba(1, 1, 1, 0.10) : Qt.rgba(1, 1, 1, 0.03)
                            border.width: 1
                            border.color: root.appState.eventTypeFilter === typeChip.modelData.value
                                ? Qt.rgba(1, 1, 1, 0.12)
                                : Qt.rgba(1, 1, 1, 0.04)
                            width: typeRow.implicitWidth + 18

                            Row {
                                id: typeRow

                                anchors.centerIn: parent
                                spacing: 6

                                Text {
                                    text: typeChip.modelData.icon
                                    color: root.appState.eventTypeFilter === typeChip.modelData.value
                                        ? Qt.rgba(1, 1, 1, 0.80)
                                        : Qt.rgba(1, 1, 1, 0.50)
                                    font.pixelSize: 10
                                }

                                Text {
                                    text: typeChip.modelData.label
                                    color: root.appState.eventTypeFilter === typeChip.modelData.value
                                        ? Qt.rgba(1, 1, 1, 0.88)
                                        : Qt.rgba(1, 1, 1, 0.60)
                                    font.pixelSize: 10
                                    font.weight: root.appState.eventTypeFilter === typeChip.modelData.value ? Font.Medium : Font.Normal
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: root.appState.eventTypeFilter = typeChip.modelData.value
                            }
                        }
                    }
                }
            }

            Row {
                width: parent.width
                height: Math.max(0, parent.height - y)
                spacing: 14

                ListView {
                    id: eventList

                    width: Math.min(324, parent.width * 0.46)
                    height: parent.height
                    clip: true
                    spacing: 4
                    model: root.filteredEvents
                    boundsBehavior: Flickable.StopAtBounds

                    delegate: Rectangle {
                        id: eventRow

                        required property var modelData
                        required property int index

                        readonly property bool selected: root.agentHub.selectionAgent.isEventSelected(eventRow.modelData)
                        readonly property color accentColor: root.statusColor(eventRow.modelData.status)

                        width: eventList.width
                        height: 76
                        radius: 18
                        color: selected ? Qt.rgba(1, 1, 1, 0.07) : Qt.rgba(1, 1, 1, hovered ? 0.042 : 0.022)
                        border.width: 1
                        border.color: selected ? Qt.rgba(1, 1, 1, 0.11) : Qt.rgba(1, 1, 1, 0.03)
                        opacity: selected ? 1 : 0.94

                        property bool hovered: false

                        Rectangle {
                            width: 2
                            height: parent.height - 20
                            anchors.left: parent.left
                            anchors.leftMargin: 8
                            anchors.verticalCenter: parent.verticalCenter
                            radius: 1
                            color: eventRow.accentColor
                            opacity: eventRow.selected ? 0.60 : 0.28
                        }

                        Column {
                            anchors.fill: parent
                            anchors.leftMargin: 18
                            anchors.rightMargin: 14
                            anchors.topMargin: 10
                            anchors.bottomMargin: 10
                            spacing: 5

                            Row {
                                width: parent.width
                                spacing: 8

                                Text {
                                    width: Math.max(0, parent.width - statusBadge.width - 8)
                                    text: eventRow.modelData.name
                                    color: eventRow.selected ? Qt.rgba(1, 1, 1, 0.94) : Qt.rgba(1, 1, 1, 0.84)
                                    font.pixelSize: 12
                                    font.weight: eventRow.selected ? Font.Medium : Font.Normal
                                    elide: Text.ElideRight
                                }

                                Rectangle {
                                    id: statusBadge

                                    height: 20
                                    radius: 10
                                    color: Qt.rgba(eventRow.accentColor.r, eventRow.accentColor.g, eventRow.accentColor.b, 0.10)
                                    border.width: 1
                                    border.color: Qt.rgba(eventRow.accentColor.r, eventRow.accentColor.g, eventRow.accentColor.b, 0.20)
                                    width: badgeRow.implicitWidth + 12

                                    Row {
                                        id: badgeRow

                                        anchors.centerIn: parent
                                        spacing: 5

                                        Text {
                                            text: "●"
                                            color: eventRow.accentColor
                                            font.pixelSize: 8
                                        }

                                        Text {
                                            text: eventRow.modelData.status
                                            color: Qt.rgba(1, 1, 1, 0.70)
                                            font.pixelSize: 9
                                        }
                                    }
                                }
                            }

                            Row {
                                width: parent.width
                                spacing: 8

                                Text {
                                    text: root.typeIcon(eventRow.modelData.type) + " " + eventRow.modelData.type
                                    color: Qt.rgba(1, 1, 1, eventRow.selected ? 0.70 : 0.58)
                                    font.pixelSize: 10
                                    elide: Text.ElideRight
                                }

                                Text {
                                    text: eventRow.modelData.startTimestamp
                                    color: Qt.rgba(1, 1, 1, 0.42)
                                    font.pixelSize: 10
                                    elide: Text.ElideRight
                                }
                            }

                            Text {
                                width: parent.width
                                text: eventRow.modelData.description
                                color: Qt.rgba(1, 1, 1, 0.50)
                                font.pixelSize: 10
                                lineHeight: 1.15
                                elide: Text.ElideRight
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            onEntered: parent.hovered = true
                            onExited: parent.hovered = false
                            onClicked: root.agentHub.selectionAgent.selectEvent(eventRow.modelData)
                        }
                    }

                    ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AsNeeded
                        width: 6
                    }
                }

                Rectangle {
                    width: 1
                    height: parent.height
                    color: Qt.rgba(1, 1, 1, 0.05)
                }

                Item {
                    width: parent.width - eventList.width - 15
                    height: parent.height

                    Column {
                        anchors.fill: parent
                        spacing: 10
                        visible: !root.hasSelection

                        Item { width: 1; height: 8 }

                        Text {
                            width: parent.width
                            text: "Select an event"
                            color: Qt.rgba(1, 1, 1, 0.88)
                            font.pixelSize: 16
                            font.weight: Font.Medium
                            elide: Text.ElideRight
                        }

                        Text {
                            width: parent.width
                            text: "Choose a row on the left to inspect status, type, timestamps, and contextual description."
                            color: Qt.rgba(1, 1, 1, 0.54)
                            font.pixelSize: 11
                            lineHeight: 1.28
                            wrapMode: Text.WordWrap
                        }
                    }

                    Flickable {
                        anchors.fill: parent
                        visible: root.hasSelection
                        clip: true
                        contentWidth: width
                        contentHeight: inspectorColumn.implicitHeight
                        boundsBehavior: Flickable.StopAtBounds

                        Column {
                            id: inspectorColumn

                            width: parent.width
                            spacing: 12

                            Text {
                                width: parent.width
                                text: root.selectedEvent.name || "Event"
                                color: Qt.rgba(1, 1, 1, 0.94)
                                font.pixelSize: 16
                                font.weight: Font.Medium
                                elide: Text.ElideRight
                            }

                            Row {
                                width: parent.width
                                spacing: 8

                                Rectangle {
                                    height: 24
                                    radius: 12
                                    color: Qt.rgba(root.statusColor(root.selectedEvent.status).r,
                                                   root.statusColor(root.selectedEvent.status).g,
                                                   root.statusColor(root.selectedEvent.status).b, 0.11)
                                    border.width: 1
                                    border.color: Qt.rgba(root.statusColor(root.selectedEvent.status).r,
                                                          root.statusColor(root.selectedEvent.status).g,
                                                          root.statusColor(root.selectedEvent.status).b, 0.20)
                                    width: statusInspectorRow.implicitWidth + 14

                                    Row {
                                        id: statusInspectorRow

                                        anchors.centerIn: parent
                                        spacing: 5

                                        Text {
                                            text: "●"
                                            color: root.statusColor(root.selectedEvent.status)
                                            font.pixelSize: 8
                                        }

                                        Text {
                                            text: root.selectedEvent.status
                                            color: Qt.rgba(1, 1, 1, 0.80)
                                            font.pixelSize: 10
                                        }
                                    }
                                }

                                Rectangle {
                                    height: 24
                                    radius: 12
                                    color: Qt.rgba(1, 1, 1, 0.05)
                                    border.width: 1
                                    border.color: Qt.rgba(1, 1, 1, 0.06)
                                    width: typeInspectorRow.implicitWidth + 14

                                    Row {
                                        id: typeInspectorRow

                                        anchors.centerIn: parent
                                        spacing: 5

                                        Text {
                                            text: root.typeIcon(root.selectedEvent.type)
                                            color: Qt.rgba(1, 1, 1, 0.62)
                                            font.pixelSize: 10
                                        }

                                        Text {
                                            text: root.selectedEvent.type
                                            color: Qt.rgba(1, 1, 1, 0.74)
                                            font.pixelSize: 10
                                        }
                                    }
                                }
                            }

                            Column {
                                width: parent.width
                                spacing: 8

                                Text {
                                    text: "Timestamps"
                                    color: Qt.rgba(1, 1, 1, 0.46)
                                    font.pixelSize: 9
                                    font.weight: Font.Medium
                                    font.letterSpacing: 0.4
                                }

                                EventField {
                                    width: parent.width
                                    label: "Start"
                                    value: root.selectedEvent.startTimestamp || "—"
                                }

                                EventField {
                                    width: parent.width
                                    label: "End"
                                    value: root.selectedEvent.endTimestamp || "—"
                                }

                                EventField {
                                    width: parent.width
                                    label: "Updated"
                                    value: root.selectedEvent.updatedTimestamp || "—"
                                }
                            }

                            Column {
                                width: parent.width
                                spacing: 8

                                Text {
                                    text: "Description"
                                    color: Qt.rgba(1, 1, 1, 0.46)
                                    font.pixelSize: 9
                                    font.weight: Font.Medium
                                    font.letterSpacing: 0.4
                                }

                                Rectangle {
                                    width: parent.width
                                    radius: 16
                                    color: Qt.rgba(1, 1, 1, 0.028)
                                    border.width: 1
                                    border.color: Qt.rgba(1, 1, 1, 0.04)
                                    implicitHeight: descriptionText.implicitHeight + 20

                                    Text {
                                        id: descriptionText

                                        anchors.fill: parent
                                        anchors.margins: 10
                                        text: root.selectedEvent.description || "No description"
                                        color: Qt.rgba(1, 1, 1, 0.68)
                                        font.pixelSize: 11
                                        lineHeight: 1.28
                                        wrapMode: Text.WordWrap
                                    }
                                }
                            }
                        }

                        ScrollBar.vertical: ScrollBar {
                            policy: ScrollBar.AsNeeded
                            width: 6
                        }
                    }
                }
            }
        }
    }

    component EventField: Rectangle {
        id: fieldRoot

        property string label: ""
        property string value: ""

        implicitHeight: 36
        radius: 14
        color: Qt.rgba(1, 1, 1, 0.03)
        border.width: 1
        border.color: Qt.rgba(1, 1, 1, 0.04)

        Row {
            anchors.fill: parent
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            spacing: 10

            Text {
                width: 72
                anchors.verticalCenter: parent.verticalCenter
                text: fieldRoot.label
                color: Qt.rgba(1, 1, 1, 0.48)
                font.pixelSize: 10
                elide: Text.ElideRight
            }

            Text {
                width: Math.max(0, parent.width - 82)
                anchors.verticalCenter: parent.verticalCenter
                text: fieldRoot.value
                color: Qt.rgba(1, 1, 1, 0.84)
                font.pixelSize: 11
                elide: Text.ElideRight
            }
        }
    }
}
