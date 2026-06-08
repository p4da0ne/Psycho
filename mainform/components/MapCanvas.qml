pragma ComponentBehavior: Bound
import QtQuick 2.15
import mapview 1.0
import Saturn.Backend 1.0

Rectangle {
    id: root

    color: "#0B0F14"
    clip: true

    property var appState
    property var agentHub
    property var objects: []
    property var lines: []
    property var polygons: []
    property var labels: []
    property var editRoleOptions: [
        { "roleId": "position", "label": "Позиция" },
        { "roleId": "route", "label": "Маршрут" },
        { "roleId": "coverage", "label": "Покрытие" }
    ]
    property alias sceneContent: mapHost

    signal interactionActivity()

    function markerVisible(objectData) {
        if (!objectData || !appState)
            return false
        // Единый фильтр видимости — из filterAgent, чтобы карта и список совпадали.
        if (agentHub && agentHub.filterAgent)
            return agentHub.filterAgent.objectVisible(objectData)
        // Fallback, если agentHub ещё не передан.
        if (objectData.side === "friendly" && !appState.showFriendly)
            return false
        if (objectData.side === "foreign" && !appState.showForeign)
            return false
        if (objectData.side === "infrastructure" && !appState.showBaseStations)
            return false
        if (appState.searchText !== "") {
            var needle = appState.searchText.toLowerCase()
            var haystack = (objectData.name + " " + objectData.id + " " + objectData.kind).toLowerCase()
            if (haystack.indexOf(needle) === -1)
                return false
        }
        return true
    }

    function filteredObjects() {
        return (objects || []).filter(function(objectData) {
            return markerVisible(objectData)
        })
    }

    function filteredLines() {
        return (lines || []).filter(function(lineData) {
            return markerVisible(lineData) && (appState ? appState.showCoverageLine : true)
        })
    }

    function filteredPolygons() {
        return (polygons || []).filter(function(polygonData) {
            return markerVisible(polygonData) && (appState ? appState.showCoverageLine : true)
        })
    }

    function updateEditPoint(index, longitude, latitude) {
        if (!appState || !appState.geometryEditActive)
            return
        var current = appState.geometryEditPoints ? appState.geometryEditPoints.slice(0) : []
        if (index < 0 || index >= current.length)
            return
        current[index] = {
            "longitude": Number(longitude),
            "latitude": Number(latitude)
        }
        appState.geometryEditPoints = current
        appState.geometryEditDirty = true
    }

    function removeEditPoint(index) {
        if (!appState || !appState.geometryEditActive)
            return
        var current = appState.geometryEditPoints ? appState.geometryEditPoints.slice(0) : []
        if (index < 0 || index >= current.length)
            return
        current.splice(index, 1)
        appState.geometryEditPoints = current
        appState.geometryEditDirty = true
    }

    function appendEditPoint(longitude, latitude) {
        if (!appState || !appState.geometryEditActive)
            return
        var current = appState.geometryEditPoints ? appState.geometryEditPoints.slice(0) : []
        if (appState.geometryEditRole === "position") {
            current = [{ "longitude": Number(longitude), "latitude": Number(latitude) }]
        } else {
            current.push({ "longitude": Number(longitude), "latitude": Number(latitude) })
        }
        appState.geometryEditPoints = current
        appState.geometryEditDirty = true
    }

    function insertEditPoint(index, longitude, latitude) {
        if (!appState || !appState.geometryEditActive)
            return
        var current = appState.geometryEditPoints ? appState.geometryEditPoints.slice(0) : []
        var insertIndex = Math.max(0, Math.min(Number(index), current.length))
        if (appState.geometryEditRole === "position") {
            current = [{ "longitude": Number(longitude), "latitude": Number(latitude) }]
        } else {
            current.splice(insertIndex, 0, { "longitude": Number(longitude), "latitude": Number(latitude) })
        }
        appState.geometryEditPoints = current
        appState.geometryEditDirty = true
    }

    Item {
        id: mapHost
        anchors.fill: parent

        MyMapView {
            id: mapLibreSurface
            anchors.fill: parent
            appState: root.appState
            objects: root.filteredObjects()
            lines: root.filteredLines()
            polygons: root.filteredPolygons()
            labels: root.labels
            editActive: root.appState ? root.appState.geometryEditActive : false
            editRole: root.appState ? root.appState.geometryEditRole : ""
            editGeometryType: root.appState ? root.appState.geometryEditType : ""
            editPoints: root.appState ? root.appState.geometryEditPoints : []
            editClosed: root.appState ? root.appState.geometryEditClosed : false

            onInteractionActivity: root.interactionActivity()

            onRequestMapMenu: function(lon, lat, screenX, screenY) {
                root.appState.lastContextLon = lon
                root.appState.lastContextLat = lat
                mapMenu.popup(screenX, screenY)
            }

            onRequestObjectMenu: function(objectData, screenX, screenY) {
                objectMenu.objectData = objectData
                objectMenu.popup(screenX, screenY)
            }

            onRequestSelectObject: function(objectData) {
                if (root.agentHub && root.agentHub.selectionAgent)
                    root.agentHub.selectionAgent.selectObject(objectData)
                else
                    root.appState.selectObject(objectData)
            }

            onRequestClearSelection: {
                if (root.agentHub && root.agentHub.selectionAgent) {
                    root.agentHub.selectionAgent.clearSelection()
                    root.agentHub.selectionAgent.clearEventSelection()
                } else {
                    root.appState.clearSelection()
                }
            }

            onHoverObject: function(objectData) {
                if (root.agentHub && root.agentHub.selectionAgent)
                    root.agentHub.selectionAgent.setHoveredObject(objectData || {})
                else
                    root.appState.hoveredObject = objectData || null
            }

            onEditPointMoved: function(index, longitude, latitude) {
                root.updateEditPoint(index, longitude, latitude)
            }

            onEditPointRemoved: function(index) {
                root.removeEditPoint(index)
            }

            onEditPointAppended: function(longitude, latitude) {
                root.appendEditPoint(longitude, latitude)
            }

            onEditPointInserted: function(index, longitude, latitude) {
                root.insertEditPoint(index, longitude, latitude)
            }
        }
    }

    ContextMenuMap {
        id: mapMenu
        appState: root.appState
    }

    ContextMenuObject {
        id: objectMenu
        appState: root.appState
        agentHub: root.agentHub
    }

    GlassPanel {
        id: geometryEditorPanel

        visible: root.appState ? root.appState.geometryEditActive : false
        z: 70
        width: 360
        height: 154
        anchors.left: parent.left
        anchors.leftMargin: 18
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 26
        backdropSource: root.sceneContent
        surfaceColor: "#10161d"
        surfaceOpacity: 0.64
        radius: 18
        padding: 12

        Column {
            anchors.fill: parent
            spacing: 8

            Text {
                width: parent.width
                text: "Редактирование геометрии: " + (root.appState ? root.appState.geometryEditRole : "")
                color: "#f3f7fb"
                font.pixelSize: 17
                font.weight: Font.Medium
                elide: Text.ElideRight
            }

            Text {
                width: parent.width
                text: (root.appState ? root.appState.geometryEditObjectName : "") +
                      " | точек: " + String(root.appState && root.appState.geometryEditPoints ? root.appState.geometryEditPoints.length : 0)
                color: "#9fb0c3"
                font.pixelSize: 14
                elide: Text.ElideRight
            }

            Text {
                width: parent.width
                text: "ЛКМ по карте: добавить вершину в конец. ЛКМ по ромбу: вставить в сегмент. ПКМ по вершине: удалить."
                color: "#8b98a6"
                font.pixelSize: 13
                wrapMode: Text.WordWrap
            }

            Row {
                spacing: 6
                visible: root.appState ? root.appState.geometryEditAllActive : false

                Repeater {
                    model: root.editRoleOptions

                    delegate: Rectangle {
                        required property int index
                        readonly property var roleItem: root.editRoleOptions[index] || ({})
                        width: 104
                        height: 28
                        radius: 8
                        color: root.appState && root.appState.geometryEditRole === roleItem.roleId ? "#1d4ed8" : "#334155"

                        Text {
                            anchors.centerIn: parent
                            text: roleItem.label || ""
                            color: "#f8fafc"
                            font.pixelSize: 14
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: root.appState.setGeometryEditRole(roleItem.roleId || "")
                        }
                    }
                }
            }

            Row {
                spacing: 8

                Rectangle {
                    width: 110
                    height: 32
                    radius: 10
                    color: "#1d4ed8"

                    Text {
                        anchors.centerIn: parent
                        text: "Сохранить"
                        color: "#f8fafc"
                        font.pixelSize: 16
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: root.appState.commitGeometryEdit()
                    }
                }

                Rectangle {
                    width: 110
                    height: 32
                    radius: 10
                    color: "#334155"

                    Text {
                        anchors.centerIn: parent
                        text: "Отменить"
                        color: "#f8fafc"
                        font.pixelSize: 16
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: root.appState.cancelGeometryEdit()
                    }
                }
            }
        }
    }
}

