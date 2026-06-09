import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/theme"

// Вкладка "Символы" (библиотека символов). host = RightSidebar (root).

Column {
    property var host

    spacing: 10

    SectionTitle { text: "Библиотека символов" }

    Rectangle {
        width: parent.width
        height: Math.max(150, parent.height * 0.34)
        radius: 18
        color: Theme.ink(0.018)
        border.width: 1
        border.color: Theme.ink(0.03)

        Column {
            anchors.fill: parent
            anchors.margins: 8
            spacing: 8

            Text {
                width: parent.width
                text: host.symbolAgent ? host.symbolTypeLabel(host.symbolAgent.selectedTypePath) : "Тип не выбран"
                color: Theme.ink(0.56)
                font.pixelSize: 12
                elide: Text.ElideRight
            }

            ListView {
                width: parent.width
                height: parent.height - y
                clip: true
                spacing: 4
                model: host.symbolList

                delegate: Rectangle {
                    id: symbolRow

                    required property var modelData

                    readonly property bool selected: host.symbolAgent && host.symbolAgent.selectedSymbolId === symbolRow.modelData.id

                    width: ListView.view.width
                    height: 46
                    radius: 14
                    color: symbolRow.selected ? Theme.ink(0.06) : Theme.ink(0.02)
                    border.width: 1
                    border.color: symbolRow.selected ? Theme.ink(0.09) : Theme.ink(0.028)

                    Row {
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        spacing: 8

                        Rectangle {
                            anchors.verticalCenter: parent.verticalCenter
                            width: 18
                            height: 18
                            radius: 9
                            gradient: Gradient {
                                GradientStop { position: 0.0; color: symbolRow.modelData.topColor }
                                GradientStop { position: 1.0; color: symbolRow.modelData.fillColor }
                            }
                        }

                        Column {
                            anchors.verticalCenter: parent.verticalCenter
                            width: Math.max(0, parent.width - 26)
                            spacing: 2

                            Text {
                                width: parent.width
                                text: symbolRow.modelData.name
                                color: symbolRow.selected ? Theme.ink(0.92) : Theme.ink(0.76)
                                font.pixelSize: 13
                                font.weight: symbolRow.selected ? Font.Medium : Font.Normal
                                elide: Text.ElideRight
                            }

                            Text {
                                width: parent.width
                                text: symbolRow.modelData.glyph + " · " + symbolRow.modelData.fillColor
                                color: Theme.ink(0.44)
                                font.pixelSize: 11
                                elide: Text.ElideRight
                            }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: host.symbolAgent.selectedSymbolId = symbolRow.modelData.id
                    }
                }
            }
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
            contentHeight: symbolDetails.implicitHeight
            boundsBehavior: Flickable.StopAtBounds

            Column {
                id: symbolDetails

                width: parent.width
                spacing: 10

                Row {
                    width: parent.width
                    spacing: 10

                    Rectangle {
                        width: 62
                        height: 62
                        radius: 18
                        color: Theme.ink(0.025)
                        border.width: 1
                        border.color: Theme.ink(0.04)

                        Rectangle {
                            anchors.centerIn: parent
                            width: 24
                            height: 24
                            radius: 12
                            gradient: Gradient {
                                GradientStop { position: 0.0; color: host.symbolDraftTopColor || Theme.friendlyLight }
                                GradientStop { position: 1.0; color: host.symbolPreviewColor() }
                            }
                        }

                        Text {
                            anchors.centerIn: parent
                            text: host.symbolDraftGlyph || "●"
                            color: Theme.ink(0.86)
                            font.pixelSize: 14
                        }
                    }

                    Column {
                        width: parent.width - 72
                        spacing: 3

                        Text {
                            width: parent.width
                            text: host.symbolDraftName || "Выберите символ"
                            color: Theme.ink(0.92)
                            font.pixelSize: 15
                            font.weight: Font.Medium
                            elide: Text.ElideRight
                        }

                        Text {
                            width: parent.width
                            text: host.symbolAgent ? host.symbolTypeLabel(host.symbolAgent.selectedTypePath) : "—"
                            color: Theme.ink(0.52)
                            font.pixelSize: 12
                            elide: Text.ElideRight
                        }
                    }
                }

                EditField {
                    width: parent.width
                    label: "Название"
                    value: host.symbolDraftName
                    onValueEdited: function(nextValue) {
                        host.symbolDraftName = nextValue
                        host.commitSymbolDraft()
                    }
                }

                Row {
                    width: parent.width
                    spacing: 8

                    EditField {
                        width: parent.width * 0.28
                        label: "Знак"
                        value: host.symbolDraftGlyph
                        onValueEdited: function(nextValue) {
                            host.symbolDraftGlyph = nextValue.length > 0 ? nextValue[0] : "●"
                            host.commitSymbolDraft()
                        }
                    }

                    EditField {
                        width: parent.width * 0.34 - 4
                        label: "Заливка"
                        value: host.symbolDraftFillColor
                        onValueEdited: function(nextValue) {
                            host.symbolDraftFillColor = nextValue
                            host.commitSymbolDraft()
                        }
                    }

                    EditField {
                        width: parent.width * 0.38 - 4
                        label: "Верх"
                        value: host.symbolDraftTopColor
                        onValueEdited: function(nextValue) {
                            host.symbolDraftTopColor = nextValue
                            host.commitSymbolDraft()
                        }
                    }
                }

                Rectangle {
                    width: parent.width
                    implicitHeight: 50
                    radius: 16
                    color: Theme.ink(0.022)
                    border.width: 1
                    border.color: Theme.ink(0.03)

                    Column {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 4

                        Text {
                            text: "Внутренняя непрозрачность"
                            color: Theme.ink(0.48)
                            font.pixelSize: 11
                        }

                        Slider {
                            width: parent.width
                            from: 0.2
                            to: 1.0
                            value: host.symbolDraftInnerOpacity
                            onMoved: {
                                host.symbolDraftInnerOpacity = value
                                host.commitSymbolDraft()
                            }
                        }
                    }
                }
            }

            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded; width: 6 }
        }
    }
}
