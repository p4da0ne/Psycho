import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: root

    property var appState
    property var agentHub
    property var panelManager
    property Item backdropSource
    property var objectTypeFilters: [
        { "value": 0, "label": "Все типы" },
        { "value": 1, "label": "Формирования" },
        { "value": 2, "label": "Особые условия" },
        { "value": 3, "label": "Средства СМИ" },
        { "value": 4, "label": "Средства формирований" },
        { "value": 5, "label": "Средства групп" },
        { "value": 6, "label": "Регионы" },
        { "value": 7, "label": "Персоналии" },
        { "value": 8, "label": "События" },
        { "value": 9, "label": "Группы" },
        { "value": 10, "label": "СМИ" }
    ]
    property var ownershipFilters: [
        { "value": "all", "label": "Все" },
        { "value": "friendly", "label": "Свои" },
        { "value": "enemy", "label": "Противник" }
    ]
    property var regionFilters: [
        { "value": "all", "label": "Все регионы" },
        { "value": "russia", "label": "Внутри РФ" },
        { "value": "foreign", "label": "Другие страны" }
    ]

    function indexByValue(listModel, value, fallbackIndex) {
        for (var i = 0; i < listModel.length; ++i) {
            if (String(listModel[i].value) === String(value))
                return i
        }
        return fallbackIndex
    }

    implicitHeight: 50
    opacity: appState && appState.controlsVisible ? 1 : 0.88
    y: 0

    Behavior on opacity {
        NumberAnimation {
            duration: 240
        }
    }

    Behavior on y {
        NumberAnimation {
            duration: 260
        }
    }

    GlassPanel {
        id: searchShell

        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: field.activeFocus || field.text.length > 0 ? Math.min(root.width, 540) : Math.min(root.width, 490)
        height: 42
        radius: 21
        padding: 0
        backdropSource: root.backdropSource
        surfaceColor: "#141c24"
        surfaceOpacity: field.activeFocus ? 0.52 : 0.46
        shadowOpacity: 0.05
        highlightOpacity: 0.04

        Behavior on width {
            SpringAnimation {
                spring: 3.8
                damping: 0.36
            }
        }

        Row {
            anchors.fill: parent
            anchors.leftMargin: 8
            anchors.rightMargin: 8
            spacing: 8

            Text {
                id: searchIcon
                anchors.verticalCenter: parent.verticalCenter
                text: "⌕"
                color: "#d9e2ed"
                font.pixelSize: 17
                opacity: 0.80
            }

            TextField {
                id: field

                width: Math.max(96,
                                searchShell.width
                                - searchIcon.width
                                - (clearButton.visible ? 36 : 10)
                                - 34)
                height: parent.height
                placeholderText: "Поиск"
                placeholderTextColor: "#91a0b0"
                text: root.appState ? root.appState.searchText : ""
                color: Qt.rgba(1, 1, 1, 0.88)
                font.pixelSize: 16
                font.weight: Font.Normal
                leftPadding: 0
                rightPadding: 0
                verticalAlignment: Text.AlignVCenter
                background: Item {}
                selectByMouse: true

                onTextChanged: {
                    if (root.agentHub && root.agentHub.uiStateAgent)
                        root.agentHub.uiStateAgent.setSearchText(text)
                }
                onAccepted: {
                    if (root.agentHub && root.agentHub.uiStateAgent)
                        root.agentHub.uiStateAgent.focusSearchResult()
                }
                onActiveFocusChanged: {
                    if (activeFocus)
                        filtersPopup.open()
                }
            }

            Button {
                id: clearButton

                visible: field.text.length > 0
                anchors.verticalCenter: parent.verticalCenter
                implicitWidth: 24
                implicitHeight: 24
                flat: true
                background: Rectangle {
                    radius: 12
                    color: "#ffffff"
                    opacity: clearButton.down ? 0.09 : clearButton.hovered ? 0.06 : 0.04
                }
                contentItem: Text {
                    text: "×"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: "#d7e0ea"
                    font.pixelSize: 18
                }
                onClicked: {
                    field.text = ""
                    if (root.agentHub && root.agentHub.uiStateAgent)
                        root.agentHub.uiStateAgent.setSearchText("")
                }
            }

        }

        TapHandler {
            onTapped: {
                if (!filtersPopup.opened)
                    filtersPopup.open()
                field.forceActiveFocus()
            }
        }
    }

    Popup {
        id: filtersPopup
        parent: root
        x: searchShell.x
        y: searchShell.y + searchShell.height + 6
        width: searchShell.width
        modal: false
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
        padding: 0

        background: Rectangle {
            radius: 14
            color: "#1a242f"
            border.width: 1
            border.color: Qt.rgba(1, 1, 1, 0.09)
        }

        contentItem: ColumnLayout {
            spacing: 10
            anchors.fill: parent
            anchors.margins: 12

            Text {
                text: "Фильтры"
                color: "#e7eef7"
                font.pixelSize: 16
                font.weight: Font.DemiBold
                Layout.fillWidth: true
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Text {
                    text: "Тип объекта"
                    color: "#c8d4e2"
                    font.pixelSize: 13
                    Layout.preferredWidth: 146
                }

                ComboBox {
                    id: objectTypeCombo
                    Layout.fillWidth: true
                    model: root.objectTypeFilters
                    textRole: "label"
                    currentIndex: root.indexByValue(root.objectTypeFilters,
                                                   root.appState ? root.appState.quickFilterObjectType : 0,
                                                   0)
                    onActivated: {
                        var value = model[currentIndex].value
                        if (root.agentHub && root.agentHub.uiStateAgent)
                            root.agentHub.uiStateAgent.setQuickFilterObjectType(value)
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Text {
                    text: "Принадлежность"
                    color: "#c8d4e2"
                    font.pixelSize: 13
                    Layout.preferredWidth: 146
                }

                ComboBox {
                    id: ownershipCombo
                    Layout.fillWidth: true
                    model: root.ownershipFilters
                    textRole: "label"
                    currentIndex: root.indexByValue(root.ownershipFilters,
                                                   root.appState ? root.appState.quickFilterOwnership : "all",
                                                   0)
                    onActivated: {
                        var value = model[currentIndex].value
                        if (root.agentHub && root.agentHub.uiStateAgent)
                            root.agentHub.uiStateAgent.setQuickFilterOwnership(value)
                    }
                }
            }

            RowLayout {
                visible: root.appState && Number(root.appState.quickFilterObjectType || 0) === 8
                Layout.fillWidth: true
                spacing: 8

                Text {
                    text: "Временные рамки"
                    color: "#c8d4e2"
                    font.pixelSize: 13
                    Layout.preferredWidth: 146
                }

                TextField {
                    id: fromDateField
                    Layout.fillWidth: true
                    placeholderText: "от YYYY-MM-DD"
                    text: root.appState ? root.appState.quickFilterEventFrom : ""
                    selectByMouse: true
                    onEditingFinished: {
                        if (root.agentHub && root.agentHub.uiStateAgent)
                            root.agentHub.uiStateAgent.setQuickFilterEventRange(text, toDateField.text)
                    }
                }

                TextField {
                    id: toDateField
                    Layout.fillWidth: true
                    placeholderText: "до YYYY-MM-DD"
                    text: root.appState ? root.appState.quickFilterEventTo : ""
                    selectByMouse: true
                    onEditingFinished: {
                        if (root.agentHub && root.agentHub.uiStateAgent)
                            root.agentHub.uiStateAgent.setQuickFilterEventRange(fromDateField.text, text)
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Text {
                    text: "Регионы"
                    color: "#c8d4e2"
                    font.pixelSize: 13
                    Layout.preferredWidth: 146
                }

                ComboBox {
                    id: regionCombo
                    Layout.fillWidth: true
                    model: root.regionFilters
                    textRole: "label"
                    currentIndex: root.indexByValue(root.regionFilters,
                                                   root.appState ? root.appState.quickFilterRegionScope : "all",
                                                   0)
                    onActivated: {
                        var value = model[currentIndex].value
                        if (root.agentHub && root.agentHub.uiStateAgent)
                            root.agentHub.uiStateAgent.setQuickFilterRegionScope(value)
                    }
                }
            }
        }
    }
}

