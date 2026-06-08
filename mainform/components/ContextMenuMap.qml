import QtQuick 2.15
import QtQuick.Controls 2.15
import Saturn.Backend 1.0
import "qrc:/theme"

Menu {
    id: root

    property var appState
    font.pixelSize: 14

    background: Rectangle {
        color: Theme.surface2
        border.color: Theme.ink(0.07)
        border.width: 1
        radius: 14
    }

    MenuItem {
        text: "Обновить данные карты"
        onTriggered: {
            Polling.triggerNow()
            appState.statusMessage = "Запрошено обновление данных карты"
        }
    }

    MenuItem {
        text: "Задать позицию выбранного объекта"
        enabled: !!appState.selectedObject
        onTriggered: {
            if (!appState.selectedObject) {
                appState.statusMessage = "Не выбран объект"
                return
            }
            var ok = MapGeometryRoles.upsertPosition(
                        appState.selectedObject.objectType,
                        appState.selectedObject.objectId,
                        { "longitude": appState.lastContextLon, "latitude": appState.lastContextLat })
            appState.statusMessage = ok
                ? "Позиция обновлена для " + appState.selectedObject.name
                : "Ошибка обновления позиции: " + MapGeometryRoles.lastError
        }
    }

    MenuItem {
        text: "Добавить точку в редактируемую геометрию"
        enabled: appState.geometryEditActive
        onTriggered: {
            if (!appState.geometryEditActive)
                return
            var current = appState.geometryEditPoints ? appState.geometryEditPoints.slice(0) : []
            if (appState.geometryEditRole === "position") {
                current = [{ "longitude": appState.lastContextLon, "latitude": appState.lastContextLat }]
            } else {
                current.push({ "longitude": appState.lastContextLon, "latitude": appState.lastContextLat })
            }
            appState.geometryEditPoints = current
            appState.geometryEditDirty = true
            appState.statusMessage = "Точка добавлена в " + appState.geometryEditRole
        }
    }

    MenuItem {
        text: "Переключить в heatmap"
        onTriggered: {
            appState.mapMode = "heatmap"
            appState.statusMessage = "Включен heatmap режим"
        }
    }

    MenuSeparator {}

    MenuItem {
        text: "Lon " + Number(appState.lastContextLon).toFixed(5)
    }

    MenuItem {
        text: "Lat " + Number(appState.lastContextLat).toFixed(5)
    }
}

