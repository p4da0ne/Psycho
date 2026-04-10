import QtQuick 2.15
import QtQuick.Controls 2.15
import Saturn.Backend 1.0

Menu {
    id: root

    property var appState
    property var agentHub
    property var objectData
    font.pixelSize: 14

    background: Rectangle {
        color: "#111821"
        border.color: Qt.rgba(1, 1, 1, 0.07)
        border.width: 1
        radius: 14
    }

    MenuItem {
        text: objectData ? "Выбрать " + objectData.name : "Выбрать объект"
        onTriggered: {
            if (objectData)
                appState.selectObject(objectData)
        }
    }

    MenuItem {
        text: "Информация"
        enabled: !!objectData
        onTriggered: {
            if (!objectData) {
                appState.statusMessage = "Объект не выбран"
                return
            }
            appState.showObjectInfo(objectData)
        }
    }

    MenuItem {
        text: "Построить маршрут"
        enabled: !!objectData
        onTriggered: {
            if (!objectData) {
                appState.statusMessage = "Объект не выбран"
                return
            }
            var route = MapGeometryRoles.geometry(objectData.objectType, objectData.objectId, "route")
            if (route.length === 0) {
                appState.statusMessage = "Маршрут не задан"
                return
            }

            var lonSum = 0
            var latSum = 0
            for (var i = 0; i < route.length; ++i) {
                lonSum += Number(route[i].longitude)
                latSum += Number(route[i].latitude)
            }
            appState.selectObject(objectData)
            appState.centerLon = lonSum / route.length
            appState.centerLat = latSum / route.length
            appState.statusMessage = "Маршрут найден, точек: " + route.length
        }
    }

    MenuSeparator {}

    MenuItem {
        text: "Редактировать позицию"
        enabled: !!objectData
        onTriggered: {
            if (!objectData)
                return
            appState.selectObject(objectData)
            appState.beginGeometryEdit("position")
        }
    }

    MenuItem {
        text: "Редактировать маршрут"
        enabled: !!objectData
        onTriggered: {
            if (!objectData)
                return
            appState.selectObject(objectData)
            appState.beginGeometryEdit("route")
        }
    }

    MenuItem {
        text: "Редактировать покрытие"
        enabled: !!objectData
        onTriggered: {
            if (!objectData)
                return
            appState.selectObject(objectData)
            appState.beginGeometryEdit("coverage")
        }
    }

    MenuItem {
        text: "Редактировать все геометрии"
        enabled: !!objectData
        onTriggered: {
            if (!objectData)
                return
            appState.selectObject(objectData)
            appState.beginGeometryEditAll()
        }
    }

    MenuSeparator {}

    MenuItem {
        text: objectData ? "МППС " + objectData.mpps : "МППС"
    }

    MenuItem {
        text: objectData ? "Источник " + objectData.source : "Источник"
    }
}

