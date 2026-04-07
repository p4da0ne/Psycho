import QtQuick 2.15
import QtQuick.Controls 2.15
import Saturn.Backend 1.0

Menu {
    id: root

    property var appState
    property var agentHub
    property var objectData

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
        text: "Показать карточку объекта"
        enabled: !!objectData
        onTriggered: {
            if (!objectData) {
                appState.statusMessage = "Объект не выбран"
                return
            }
            var summary = ObjectDetailsRepo.objectSummary(objectData.objectType, objectData.objectId)
            var summaryName = summary && summary.objectName ? summary.objectName : ""
            appState.statusMessage = summaryName !== ""
                ? ("Карточка: " + summaryName + " (ID " + summary.objectId + ")")
                : ("Карточка: " + objectData.name)
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
            appState.statusMessage = route.length > 0
                ? ("Маршрут найден, точек: " + route.length)
                : "Маршрут не задан"
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
