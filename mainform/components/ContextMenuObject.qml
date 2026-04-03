import QtQuick 2.15
import QtQuick.Controls 2.15
import Saturn.Backend 1.0

Menu {
    id: root

    property var appState
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
            appState.statusMessage = summary && summary.name
                ? ("Карточка: " + summary.name + " (ID " + summary.objectId + ")")
                : ("Карточка: " + objectData.name)
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
        text: objectData ? "МППС " + objectData.mpps : "МППС"
    }

    MenuItem {
        text: objectData ? "Источник " + objectData.source : "Источник"
    }
}
