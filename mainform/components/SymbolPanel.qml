pragma ComponentBehavior: Bound
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root

    property var agentHub
    property Item backdropSource

    readonly property var symbolAgent: agentHub ? agentHub.symbolAgent : null
    readonly property var treeData: symbolAgent ? symbolAgent.typeTree : []
    readonly property var symbols: symbolAgent ? symbolAgent.symbolsForSelectedType : []
    readonly property var selectedSymbol: symbolAgent ? symbolAgent.selectedSymbol : ({})
    readonly property string selectedTypePath: symbolAgent ? symbolAgent.selectedTypePath : ""
    readonly property int symbolCount: symbols ? symbols.length : 0
    property var expandedMap: ({
        "units": true,
        "relays": true,
        "infrastructure": true
    })
    property string draftName: ""
    property string draftGlyph: ""
    property string draftFillColor: ""
    property string draftTopColor: ""
    property real draftInnerOpacity: 0.8

    function isExpanded(path) {
        return !!expandedMap[path]
    }

    function toggleNode(path) {
        var nextMap = Object.assign({}, expandedMap)
        nextMap[path] = !nextMap[path]
        expandedMap = nextMap
    }

    function visibleNodes() {
        var output = []

        function walk(nodes, depth, prefix) {
            for (var i = 0; i < nodes.length; ++i) {
                var current = nodes[i]
                var path = prefix === "" ? current.id : prefix + "/" + current.id
                var children = current.children ? current.children : []
                var hasChildrenValue = children.length > 0
                output.push({
                    "id": current.id,
                    "label": current.label,
                    "path": path,
                    "depth": depth,
                    "hasChildren": hasChildrenValue,
                    "expanded": hasChildrenValue && root.isExpanded(path)
                })

                if (hasChildrenValue && root.isExpanded(path))
                    walk(children, depth + 1, path)
            }
        }

        walk(root.treeData || [], 0, "")
        return output
    }

    function syncDraft() {
        root.draftName = root.selectedSymbol && root.selectedSymbol.name ? root.selectedSymbol.name : ""
        root.draftGlyph = root.selectedSymbol && root.selectedSymbol.glyph ? root.selectedSymbol.glyph : "●"
        root.draftFillColor = root.selectedSymbol && root.selectedSymbol.fillColor ? root.selectedSymbol.fillColor : "#7298f6"
        root.draftTopColor = root.selectedSymbol && root.selectedSymbol.topColor ? root.selectedSymbol.topColor : "#8DB7FF"
        root.draftInnerOpacity = root.selectedSymbol && root.selectedSymbol.innerOpacity !== undefined
            ? root.selectedSymbol.innerOpacity
            : 0.82
    }

    function commitDraft() {
        if (!root.symbolAgent || !root.selectedSymbol || !root.selectedSymbol.id)
            return
        root.symbolAgent.updateSelectedSymbol(root.draftName,
                                              root.draftGlyph,
                                              root.draftFillColor,
                                              root.draftTopColor,
                                              root.draftInnerOpacity)
    }

    Component.onCompleted: syncDraft()

    Connections {
        target: root.symbolAgent

        function onSelectedSymbolChanged() {
            root.syncDraft()
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
                spacing: 8

                Column {
                    width: Math.max(0, parent.width - 72)
                    spacing: 2

                    Text {
                        width: parent.width
                        text: "Symbols"
                        color: Qt.rgba(1, 1, 1, 0.92)
                        font.pixelSize: 15
                        font.weight: Font.Medium
                        elide: Text.ElideRight
                    }

                    Text {
                        width: parent.width
                        text: root.symbolCount + " variants · assigned by object type"
                        color: Qt.rgba(1, 1, 1, 0.58)
                        font.pixelSize: 10
                        elide: Text.ElideRight
                    }
                }

                Button {
                    id: addButton

                    anchors.verticalCenter: parent.verticalCenter
                    implicitWidth: 28
                    implicitHeight: 28
                    flat: true
                    background: Rectangle {
                        radius: 14
                        color: "#ffffff"
                        opacity: addButton.down ? 0.08 : addButton.hovered ? 0.05 : 0.03
                    }
                    contentItem: Text {
                        text: "+"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: "#dce4ed"
                        font.pixelSize: 14
                    }
                    onClicked: root.symbolAgent.addSymbol()
                }

                Button {
                    id: deleteButton

                    anchors.verticalCenter: parent.verticalCenter
                    implicitWidth: 28
                    implicitHeight: 28
                    flat: true
                    enabled: root.symbolCount > 1
                    background: Rectangle {
                        radius: 14
                        color: "#ffffff"
                        opacity: !deleteButton.enabled ? 0.015 : deleteButton.down ? 0.08 : deleteButton.hovered ? 0.05 : 0.03
                    }
                    contentItem: Text {
                        text: "−"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: deleteButton.enabled ? "#dce4ed" : Qt.rgba(1, 1, 1, 0.24)
                        font.pixelSize: 16
                    }
                    onClicked: root.symbolAgent.deleteSelectedSymbol()
                }
            }

            Rectangle {
                width: parent.width
                height: 1
                color: Qt.rgba(1, 1, 1, 0.05)
            }

            Row {
                width: parent.width
                height: Math.max(0, parent.height - y)
                spacing: 14

                Column {
                    width: 172
                    height: parent.height
                    spacing: 8

                    Text {
                        text: "Object types"
                        color: Qt.rgba(1, 1, 1, 0.48)
                        font.pixelSize: 9
                        font.weight: Font.Medium
                    }

                    ListView {
                        width: parent.width
                        height: parent.height - y
                        clip: true
                        spacing: 2
                        model: root.visibleNodes()
                        boundsBehavior: Flickable.StopAtBounds

                        delegate: Rectangle {
                            id: typeRow

                            required property var modelData

                            readonly property bool selected: root.selectedTypePath === typeRow.modelData.path

                            width: ListView.view.width
                            height: 30
                            radius: 15
                            color: selected ? Qt.rgba(1, 1, 1, 0.07) : Qt.rgba(1, 1, 1, 0.022)
                            border.width: 1
                            border.color: selected ? Qt.rgba(1, 1, 1, 0.10) : Qt.rgba(1, 1, 1, 0.03)

                            Row {
                                anchors.fill: parent
                                anchors.leftMargin: 10 + typeRow.modelData.depth * 14
                                anchors.rightMargin: 10
                                spacing: 8

                                Text {
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: 10
                                    text: typeRow.modelData.hasChildren ? (typeRow.modelData.expanded ? "▾" : "▸") : ""
                                    color: Qt.rgba(1, 1, 1, 0.48)
                                    font.pixelSize: 10
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: Math.max(0, parent.width - 18)
                                    text: typeRow.modelData.label
                                    color: typeRow.selected ? Qt.rgba(1, 1, 1, 0.90) : Qt.rgba(1, 1, 1, 0.66)
                                    font.pixelSize: 10
                                    font.weight: typeRow.selected ? Font.Medium : Font.Normal
                                    elide: Text.ElideRight
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: function(mouse) {
                                    if (typeRow.modelData.hasChildren && mouse.x < 26 + typeRow.modelData.depth * 14)
                                        root.toggleNode(typeRow.modelData.path)
                                    root.symbolAgent.selectedTypePath = typeRow.modelData.path
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    width: 1
                    height: parent.height
                    color: Qt.rgba(1, 1, 1, 0.05)
                }

                Column {
                    width: parent.width - 187
                    height: parent.height
                    spacing: 10

                    Row {
                        width: parent.width
                        spacing: 12

                        Rectangle {
                            width: 96
                            height: 96
                            radius: 24
                            color: Qt.rgba(1, 1, 1, 0.03)
                            border.width: 1
                            border.color: Qt.rgba(1, 1, 1, 0.05)

                            Rectangle {
                                anchors.centerIn: parent
                                width: 42
                                height: 42
                                radius: 21
                                gradient: Gradient {
                                    GradientStop { position: 0.0; color: root.draftTopColor || "#8DB7FF" }
                                    GradientStop { position: 1.0; color: root.draftFillColor || "#7298f6" }
                                }
                            }

                            Rectangle {
                                anchors.centerIn: parent
                                width: 18
                                height: 18
                                radius: 9
                                color: "#f6f9fc"
                                opacity: root.draftInnerOpacity
                            }

                            Text {
                                anchors.centerIn: parent
                                text: root.draftGlyph || "●"
                                color: Qt.rgba(1, 1, 1, 0.88)
                                font.pixelSize: 16
                            }
                        }

                        Column {
                            width: parent.width - 108
                            spacing: 6

                            Text {
                                width: parent.width
                                text: root.draftName || "Symbol"
                                color: Qt.rgba(1, 1, 1, 0.92)
                                font.pixelSize: 14
                                font.weight: Font.Medium
                                elide: Text.ElideRight
                            }

                            Text {
                                width: parent.width
                                text: root.selectedTypePath || "No type selected"
                                color: Qt.rgba(1, 1, 1, 0.56)
                                font.pixelSize: 10
                                elide: Text.ElideRight
                            }

                            Text {
                                width: parent.width
                                text: "Library variants"
                                color: Qt.rgba(1, 1, 1, 0.44)
                                font.pixelSize: 9
                                font.weight: Font.Medium
                            }

                            ListView {
                                width: parent.width
                                height: 64
                                orientation: ListView.Horizontal
                                spacing: 8
                                clip: true
                                model: root.symbols

                                delegate: Rectangle {
                                    id: symbolChip

                                    required property var modelData

                                    readonly property bool selected: root.symbolAgent.selectedSymbolId === symbolChip.modelData.id

                                    width: 70
                                    height: 64
                                    radius: 18
                                    color: selected ? Qt.rgba(1, 1, 1, 0.08) : Qt.rgba(1, 1, 1, 0.03)
                                    border.width: 1
                                    border.color: selected ? Qt.rgba(1, 1, 1, 0.10) : Qt.rgba(1, 1, 1, 0.04)

                                    Column {
                                        anchors.centerIn: parent
                                        spacing: 5

                                        Rectangle {
                                            width: 24
                                            height: 24
                                            radius: 12
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            gradient: Gradient {
                                                GradientStop { position: 0.0; color: symbolChip.modelData.topColor }
                                                GradientStop { position: 1.0; color: symbolChip.modelData.fillColor }
                                            }
                                        }

                                        Text {
                                            width: 58
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            text: symbolChip.modelData.glyph
                                            color: Qt.rgba(1, 1, 1, 0.82)
                                            font.pixelSize: 11
                                            horizontalAlignment: Text.AlignHCenter
                                        }
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: root.symbolAgent.selectedSymbolId = symbolChip.modelData.id
                                    }
                                }
                            }
                        }
                    }

                    Column {
                        width: parent.width
                        spacing: 8

                        SymbolField {
                            width: parent.width
                            label: "Name"
                            value: root.draftName
                            onValueEdited: function(nextValue) {
                                root.draftName = nextValue
                                root.commitDraft()
                            }
                        }

                        Row {
                            width: parent.width
                            spacing: 8

                            SymbolField {
                                width: parent.width * 0.28
                                label: "Glyph"
                                value: root.draftGlyph
                                onValueEdited: function(nextValue) {
                                    root.draftGlyph = nextValue.length > 0 ? nextValue[0] : "●"
                                    root.commitDraft()
                                }
                            }

                            SymbolField {
                                width: parent.width * 0.34 - 4
                                label: "Fill"
                                value: root.draftFillColor
                                onValueEdited: function(nextValue) {
                                    root.draftFillColor = nextValue
                                    root.commitDraft()
                                }
                            }

                            SymbolField {
                                width: parent.width * 0.38 - 4
                                label: "Top"
                                value: root.draftTopColor
                                onValueEdited: function(nextValue) {
                                    root.draftTopColor = nextValue
                                    root.commitDraft()
                                }
                            }
                        }

                        Rectangle {
                            width: parent.width
                            implicitHeight: 54
                            radius: 16
                            color: Qt.rgba(1, 1, 1, 0.03)
                            border.width: 1
                            border.color: Qt.rgba(1, 1, 1, 0.04)

                            Column {
                                anchors.fill: parent
                                anchors.margins: 10
                                spacing: 6

                                Text {
                                    text: "Inner opacity"
                                    color: Qt.rgba(1, 1, 1, 0.48)
                                    font.pixelSize: 10
                                }

                                Slider {
                                    width: parent.width
                                    from: 0.2
                                    to: 1.0
                                    value: root.draftInnerOpacity
                                    onMoved: {
                                        root.draftInnerOpacity = value
                                        root.commitDraft()
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    component SymbolField: Rectangle {
        id: fieldRoot

        property string label: ""
        property string value: ""
        signal valueEdited(string nextValue)

        implicitHeight: 50
        radius: 16
        color: Qt.rgba(1, 1, 1, 0.03)
        border.width: 1
        border.color: Qt.rgba(1, 1, 1, 0.04)

        Column {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 4

            Text {
                text: fieldRoot.label
                color: Qt.rgba(1, 1, 1, 0.48)
                font.pixelSize: 9
                font.weight: Font.Medium
            }

            TextField {
                width: parent.width
                text: fieldRoot.value
                color: Qt.rgba(1, 1, 1, 0.88)
                font.pixelSize: 11
                selectByMouse: true
                background: Item {}
                padding: 0
                onTextEdited: fieldRoot.valueEdited(text)
            }
        }
    }
}
