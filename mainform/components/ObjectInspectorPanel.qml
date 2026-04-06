import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root

    property var appState
    property var agentHub
    property Item backdropSource
    property bool additionalExpanded: false

    readonly property int selectionCount: appState && appState.selectedObjects ? appState.selectedObjects.length : 0
    readonly property bool hasSelection: selectionCount > 0
    readonly property bool multiSelection: selectionCount > 1
    readonly property var primaryObject: hasSelection ? appState.selectedObject : ({})
    readonly property var selectedObjects: appState && appState.selectedObjects ? appState.selectedObjects : []
    readonly property string typeLabel: multiSelection
        ? "Multiple objects"
        : hasSelection
            ? primaryObject.kind + " · " + primaryObject.side
            : ""
    readonly property string titleLabel: multiSelection
        ? selectionCount + " objects selected"
        : hasSelection
            ? primaryObject.name
            : "Inspector"

    function averageMpps() {
        if (!selectedObjects || selectedObjects.length === 0)
            return 0

        var total = 0
        for (var i = 0; i < selectedObjects.length; ++i)
            total += selectedObjects[i].mpps
        return Math.round(total / selectedObjects.length)
    }

    function selectedNames() {
        if (!selectedObjects || selectedObjects.length === 0)
            return ""

        var names = []
        for (var i = 0; i < selectedObjects.length; ++i)
            names.push(selectedObjects[i].name)
        return names.join(", ")
    }

    visible: hasSelection
    implicitWidth: 336
    implicitHeight: inspectorShell.implicitHeight
    opacity: hasSelection ? 1 : 0
    scale: hasSelection ? 1 : 0.97

    Behavior on opacity {
        NumberAnimation { duration: 220 }
    }

    Behavior on scale {
        NumberAnimation { duration: 220 }
    }

    GlassPanel {
        id: inspectorShell

        anchors.fill: parent
        radius: 24
        padding: 16
        backdropSource: root.backdropSource
        surfaceColor: "#141c24"
        surfaceOpacity: 0.56
        shadowOpacity: 0.09
        highlightOpacity: 0.05
        edgeOpacity: 0.06

        Column {
            width: parent.width
            spacing: 12

            Row {
                width: parent.width
                spacing: 10

                Column {
                    width: Math.max(0, parent.width - 40)
                    spacing: 3

                    Text {
                        width: parent.width
                        text: root.titleLabel
                        color: Qt.rgba(1, 1, 1, 0.94)
                        font.pixelSize: 15
                        font.weight: Font.Medium
                        lineHeight: 1.14
                        elide: Text.ElideRight
                    }

                    Text {
                        width: parent.width
                        text: root.typeLabel
                        color: Qt.rgba(1, 1, 1, 0.62)
                        font.pixelSize: 10
                        lineHeight: 1.18
                        elide: Text.ElideRight
                    }
                }

                Button {
                    id: clearButton

                    anchors.verticalCenter: parent.verticalCenter
                    implicitWidth: 28
                    implicitHeight: 28
                    flat: true
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
                    onClicked: root.agentHub.selectionAgent.clearSelection()
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

                Text {
                    text: "Title"
                    color: Qt.rgba(1, 1, 1, 0.46)
                    font.pixelSize: 9
                    font.weight: Font.Medium
                    font.letterSpacing: 0.4
                }

                InspectorField {
                    width: parent.width
                    label: "Name"
                    value: root.multiSelection ? root.selectedNames() : root.primaryObject.name
                }
            }

            Column {
                width: parent.width
                spacing: 8

                Text {
                    text: "Type"
                    color: Qt.rgba(1, 1, 1, 0.46)
                    font.pixelSize: 9
                    font.weight: Font.Medium
                    font.letterSpacing: 0.4
                }

                InspectorField {
                    width: parent.width
                    label: "Category"
                    value: root.multiSelection ? "Selection" : root.primaryObject.kind
                }

                InspectorField {
                    width: parent.width
                    label: "Side"
                    value: root.multiSelection ? "Mixed / grouped" : root.primaryObject.side
                }
            }

            Column {
                width: parent.width
                spacing: 8

                Text {
                    text: "Attributes"
                    color: Qt.rgba(1, 1, 1, 0.46)
                    font.pixelSize: 9
                    font.weight: Font.Medium
                    font.letterSpacing: 0.4
                }

                InspectorField {
                    width: parent.width
                    label: root.multiSelection ? "Objects" : "MPPS"
                    value: root.multiSelection ? String(root.selectionCount) : String(root.primaryObject.mpps)
                    accent: true
                }

                InspectorField {
                    width: parent.width
                    label: root.multiSelection ? "Average MPPS" : "Coordinates"
                    value: root.multiSelection
                        ? String(root.averageMpps())
                        : Number(root.primaryObject.lon).toFixed(5) + ", " + Number(root.primaryObject.lat).toFixed(5)
                    mono: !root.multiSelection
                }

                InspectorField {
                    width: parent.width
                    label: root.multiSelection ? "Primary" : "Speed"
                    value: root.multiSelection ? root.primaryObject.name : String(root.primaryObject.speed)
                }

                InspectorField {
                    width: parent.width
                    label: root.multiSelection ? "Selection mode" : "Course"
                    value: root.multiSelection ? "Multi-select" : String(root.primaryObject.course)
                }
            }

            Column {
                width: parent.width
                spacing: 8

                Button {
                    id: additionalButton

                    width: parent.width
                    implicitHeight: 28
                    flat: true
                    background: Rectangle {
                        radius: 14
                        color: "#ffffff"
                        opacity: additionalButton.down ? 0.05 : additionalButton.hovered ? 0.035 : 0.02
                    }
                    contentItem: Row {
                        spacing: 8

                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            text: root.additionalExpanded ? "▾" : "▸"
                            color: Qt.rgba(1, 1, 1, 0.64)
                            font.pixelSize: 10
                        }

                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            text: "Additional data"
                            color: Qt.rgba(1, 1, 1, 0.76)
                            font.pixelSize: 10
                            font.weight: Font.Medium
                        }
                    }
                    onClicked: root.additionalExpanded = !root.additionalExpanded
                }

                Item {
                    width: parent.width
                    height: root.additionalExpanded ? detailsColumn.implicitHeight : 0
                    clip: true

                    Behavior on height {
                        NumberAnimation { duration: 180 }
                    }

                    Column {
                        id: detailsColumn

                        width: parent.width
                        spacing: 6
                        opacity: root.additionalExpanded ? 1 : 0

                        Behavior on opacity {
                            NumberAnimation { duration: 140 }
                        }

                        InspectorField {
                            width: parent.width
                            label: "Source"
                            value: root.multiSelection ? "Grouped demo dataset" : root.primaryObject.source
                        }

                        InspectorField {
                            width: parent.width
                            label: root.multiSelection ? "Primary ID" : "Identifier"
                            value: root.multiSelection ? root.primaryObject.id : root.primaryObject.id
                            mono: true
                        }

                        InspectorField {
                            width: parent.width
                            label: "Notes"
                            value: root.multiSelection
                                ? "Selection contains " + root.selectionCount + " objects. Use grouped actions from context."
                                : root.primaryObject.notes
                            wrap: true
                        }
                    }
                }
            }
        }
    }

    component InspectorField: Item {
        id: fieldRoot

        property string label: ""
        property string value: ""
        property bool mono: false
        property bool accent: false
        property bool wrap: false

        implicitWidth: 300
        implicitHeight: valueText.paintedHeight + labelText.paintedHeight + 6

        Column {
            width: parent.width
            spacing: 2

            Text {
                id: labelText
                width: parent.width
                text: fieldRoot.label
                color: Qt.rgba(1, 1, 1, 0.46)
                font.pixelSize: 9
                lineHeight: 1.16
                elide: Text.ElideRight
            }

            Text {
                id: valueText
                width: parent.width
                text: fieldRoot.value
                color: fieldRoot.accent ? "#A9C4FF" : Qt.rgba(1, 1, 1, 0.86)
                font.pixelSize: 10
                font.weight: fieldRoot.accent ? Font.Medium : Font.Normal
                font.family: fieldRoot.mono ? "Menlo" : ""
                lineHeight: 1.2
                wrapMode: fieldRoot.wrap ? Text.Wrap : Text.NoWrap
                maximumLineCount: fieldRoot.wrap ? 5 : 1
                elide: fieldRoot.wrap ? Text.ElideNone : Text.ElideRight
            }
        }
    }
}
