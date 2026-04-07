import QtQuick 2.15
import Saturn.Backend 1.0

Rectangle {
    color: "#0f172a"
    radius: 12
    border.color: "#1e293b"
    border.width: 1

    Component.onCompleted: {
        DbSettings.load()
        driverField.text = DbSettings.driver
        hostField.text = DbSettings.host
        portField.text = DbSettings.port
        dbNameField.text = DbSettings.dbName
        userField.text = DbSettings.user
        passwordField.text = DbSettings.password
    }

    function syncSettings() {
        // TEST/TEMP NOTE: until multi-driver support is implemented, lock to PostgreSQL.
        DbSettings.driver = "QPSQL"
        DbSettings.host = hostField.text
        DbSettings.port = Number(portField.text)
        DbSettings.dbName = dbNameField.text
        DbSettings.user = userField.text
        DbSettings.password = passwordField.text
    }

    Column {
        anchors.centerIn: parent
        width: Math.min(parent.width - 64, 480)
        spacing: 10

        Text {
            text: "Настройки подключения к БД"
            color: "#e2e8f0"
            font.pixelSize: 24
            font.bold: true
        }

        Rectangle {
            id: driverField
            width: parent.width
            height: 40
            color: "#1e293b"
            radius: 6
            border.width: 1
            border.color: "#334155"
            property alias text: driverInput.text
            TextInput {
                id: driverInput
                anchors.fill: parent
                anchors.margins: 10
                color: "#94a3b8"
                verticalAlignment: TextInput.AlignVCenter
                readOnly: true
                text: "QPSQL"
            }
        }
        Rectangle {
            id: hostField
            width: parent.width
            height: 40
            color: "#1e293b"
            radius: 6
            border.width: 1
            border.color: "#334155"
            property alias text: hostInput.text
            TextInput { id: hostInput; anchors.fill: parent; anchors.margins: 10; color: "#f8fafc"; verticalAlignment: TextInput.AlignVCenter }
        }
        Rectangle {
            id: passwordField
            width: parent.width
            height: 40
            color: "#1e293b"
            radius: 6
            border.width: 1
            border.color: "#334155"
            property alias text: passwordInput.text
            TextInput { id: passwordInput; anchors.fill: parent; anchors.margins: 10; color: "#f8fafc"; verticalAlignment: TextInput.AlignVCenter; echoMode: TextInput.Password }
        }
        Rectangle {
            id: portField
            width: parent.width
            height: 40
            color: "#1e293b"
            radius: 6
            border.width: 1
            border.color: "#334155"
            property alias text: portInput.text
            TextInput { id: portInput; anchors.fill: parent; anchors.margins: 10; color: "#f8fafc"; verticalAlignment: TextInput.AlignVCenter; inputMethodHints: Qt.ImhDigitsOnly }
        }
        Rectangle {
            id: dbNameField
            width: parent.width
            height: 40
            color: "#1e293b"
            radius: 6
            border.width: 1
            border.color: "#334155"
            property alias text: dbNameInput.text
            TextInput { id: dbNameInput; anchors.fill: parent; anchors.margins: 10; color: "#f8fafc"; verticalAlignment: TextInput.AlignVCenter }
        }
        Rectangle {
            id: userField
            width: parent.width
            height: 40
            color: "#1e293b"
            radius: 6
            border.width: 1
            border.color: "#334155"
            property alias text: userInput.text
            TextInput { id: userInput; anchors.fill: parent; anchors.margins: 10; color: "#f8fafc"; verticalAlignment: TextInput.AlignVCenter }
        }

        Row {
            spacing: 8

            Rectangle {
                width: 130
                height: 40
                radius: 6
                color: "#2563eb"
                border.color: "#3b82f6"

                Text {
                    anchors.centerIn: parent
                    text: "Сохранить"
                    color: "#f8fafc"
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        syncSettings()
                        DbSettings.save()
                        statusText.text = "Настройки сохранены"
                    }
                }
            }

            Rectangle {
                width: 220
                height: 40
                radius: 6
                color: "#475569"
                border.color: "#64748b"

                Text {
                    anchors.centerIn: parent
                    text: "Проверить подключение"
                    color: "#f8fafc"
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        syncSettings()
                        DbSettings.save()
                        if (DbSettings.applyAndConnect()) {
                            statusText.color = "#8be9a8"
                            statusText.text = "Подключение успешно"
                        } else {
                            statusText.color = "#fca5a5"
                            statusText.text = Database.lastError
                        }
                    }
                }
            }
        }

        Text {
            id: statusText
            width: parent.width
            wrapMode: Text.Wrap
            color: "#94a3b8"
            text: ""
        }
    }
}
