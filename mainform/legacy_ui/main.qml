import QtQuick 2.15
import QtQuick.Window 2.15
import Saturn.Backend 1.0

Window {
    id: root
    width: 1000
    height: 680
    visible: true
    title: "Saturn Core Shell"
    color: "#11171f"

    // TEMPORARY(TEST): technical shell for backend smoke checks
    // until dedicated production UI is integrated.
    property string panel: "none"
    // TEMPORARY(TEST): diagnostic strings for backend validation.
    property string snapshotInfo: ""
    property string runtimeInfo: ""
    property string editingInfo: ""

    // TEMPORARY(TEST): autostart polling to validate backend data flow.
    Component.onCompleted: {
        Polling.intervalMs = 30000
        Polling.start()
    }

    Rectangle {
        anchors.fill: parent
        color: "#11171f"
    }

    // TEMPORARY(TEST): direct binding for smoke checks.
    Connections {
        target: Polling
        function onTick() {
            MapSnapshot.refreshAll()
        }
    }

    // TEMPORARY(TEST): snapshot diagnostics.
    Connections {
        target: MapSnapshot
        function onSnapshotUpdated() {
            var meta = MapSnapshot.snapshotMeta()
            var total = 0
            for (var i = 0; i < meta.length; ++i) {
                total += meta[i].featureCount
            }
            snapshotInfo = "Snapshot updated: " + MapSnapshot.lastUpdatedIso + ", total features: " + total
        }
    }

    // TEMPORARY(TEST): runtime source diagnostics.
    Connections {
        target: MapRuntime
        function onSourcesUpdated() {
            var meta = MapRuntime.updateMeta()
            var points = 0
            var lines = 0
            var polygons = 0
            for (var i = 0; i < meta.length; ++i) {
                if (meta[i].geometryFamily === "points") points = meta[i].featureCount
                if (meta[i].geometryFamily === "lines") lines = meta[i].featureCount
                if (meta[i].geometryFamily === "polygons") polygons = meta[i].featureCount
            }
            runtimeInfo = "Runtime sources: points=" + points + ", lines=" + lines + ", polygons=" + polygons + ", updated: " + MapRuntime.lastRuntimeUpdateIso
        }
    }

    Column {
        anchors.left: parent.left
        anchors.leftMargin: 28
        anchors.top: parent.top
        anchors.topMargin: 28
        spacing: 12

        Text {
            text: "Главное меню"
            color: "#e2e8f0"
            font.pixelSize: 28
            font.bold: true
        }

        Text {
            text: Auth.loggedIn
                ? "Текущий пользователь: " + Auth.currentUserDisplayName
                : "Пользователь не авторизован"
            color: Auth.loggedIn ? "#8be9a8" : "#fca5a5"
            font.pixelSize: 16
        }

        Text {
            width: 350
            wrapMode: Text.Wrap
            text: snapshotInfo
            color: "#94a3b8"
            font.pixelSize: 12
        }

        Text {
            width: 350
            wrapMode: Text.Wrap
            text: runtimeInfo
            color: "#94a3b8"
            font.pixelSize: 12
        }

        Text {
            width: 350
            wrapMode: Text.Wrap
            text: editingInfo
            color: "#94a3b8"
            font.pixelSize: 12
        }

        // TEMPORARY(TEST): manual geometry write action for backend validation.
        Rectangle {
            width: 320
            height: 54
            radius: 8
            color: "#1f2937"
            border.color: "#334155"
            border.width: 1

            Text {
                anchors.centerIn: parent
                text: "Авторизация"
                color: "#f8fafc"
                font.pixelSize: 17
            }

            MouseArea {
                anchors.fill: parent
                onClicked: root.panel = "auth"
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
            }
        }

        Rectangle {
            width: 320
            height: 54
            radius: 8
            color: "#1f2937"
            border.color: "#334155"
            border.width: 1

            Text {
                anchors.centerIn: parent
                text: "Тест записи геометрии"
                color: "#f8fafc"
                font.pixelSize: 17
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    var points = [{longitude: 44.5018, latitude: 48.7071}]
                    var ok = MapEditing.replaceGeometry(7, 1, "position", "Point", points, false)
                    if (ok) {
                        editingInfo = "Запись геометрии: успешно"
                    } else {
                        editingInfo = "Запись геометрии: ошибка: " + MapEditing.lastError
                    }
                }
            }
        }

        Rectangle {
            width: 320
            height: 54
            radius: 8
            color: "#1f2937"
            border.color: "#334155"
            border.width: 1

            Text {
                anchors.centerIn: parent
                text: "Настройки подключения к БД"
                color: "#f8fafc"
                font.pixelSize: 17
            }

            MouseArea {
                anchors.fill: parent
                onClicked: root.panel = "db"
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
            }
        }
    }

    Item {
        id: panelHost
        anchors.right: parent.right
        anchors.rightMargin: 28
        anchors.top: parent.top
        anchors.topMargin: 28
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 28
        width: parent.width - 420

        Loader {
            anchors.fill: parent
            sourceComponent: root.panel === "auth" ? authPanel
                           : root.panel === "db" ? dbPanel
                           : emptyPanel
        }
    }

    Component {
        id: authPanel
        LoginScreen { }
    }

    Component {
        id: dbPanel
        DBConnectScreen { }
    }

    Component {
        id: emptyPanel
        Rectangle {
            color: "#0f172a"
            radius: 12
            border.color: "#1e293b"
            border.width: 1

            Text {
                anchors.centerIn: parent
                text: "Выберите действие в главном меню"
                color: "#94a3b8"
                font.pixelSize: 18
            }
        }
    }
}
