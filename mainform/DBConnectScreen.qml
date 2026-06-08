import QtQuick 2.15
import Saturn.Backend 1.0
import "qrc:/theme"

Rectangle {
    color: Theme.surfaceSlate
    radius: 12
    border.color: Theme.panelSlate
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
            color: Theme.textLight3
            font.pixelSize: 24
            font.bold: true
        }

        Rectangle {
            id: driverField
            width: parent.width
            height: 40
            color: Theme.panelSlate
            radius: 6
            border.width: 1
            border.color: Theme.slate
            property alias text: driverInput.text
            TextInput {
                id: driverInput
                anchors.fill: parent
                anchors.margins: 10
                color: Theme.muted4
                verticalAlignment: TextInput.AlignVCenter
                readOnly: true
                text: "QPSQL"
            }
        }
        Rectangle {
            id: hostField
            width: parent.width
            height: 40
            color: Theme.panelSlate
            radius: 6
            border.width: 1
            border.color: Theme.slate
            property alias text: hostInput.text
            TextInput { id: hostInput; anchors.fill: parent; anchors.margins: 10; color: Theme.textBright; verticalAlignment: TextInput.AlignVCenter }
        }
        Rectangle {
            id: passwordField
            width: parent.width
            height: 40
            color: Theme.panelSlate
            radius: 6
            border.width: 1
            border.color: Theme.slate
            property alias text: passwordInput.text
            TextInput { id: passwordInput; anchors.fill: parent; anchors.margins: 10; color: Theme.textBright; verticalAlignment: TextInput.AlignVCenter; echoMode: TextInput.Password }
        }
        Rectangle {
            id: portField
            width: parent.width
            height: 40
            color: Theme.panelSlate
            radius: 6
            border.width: 1
            border.color: Theme.slate
            property alias text: portInput.text
            TextInput { id: portInput; anchors.fill: parent; anchors.margins: 10; color: Theme.textBright; verticalAlignment: TextInput.AlignVCenter; inputMethodHints: Qt.ImhDigitsOnly }
        }
        Rectangle {
            id: dbNameField
            width: parent.width
            height: 40
            color: Theme.panelSlate
            radius: 6
            border.width: 1
            border.color: Theme.slate
            property alias text: dbNameInput.text
            TextInput { id: dbNameInput; anchors.fill: parent; anchors.margins: 10; color: Theme.textBright; verticalAlignment: TextInput.AlignVCenter }
        }
        Rectangle {
            id: userField
            width: parent.width
            height: 40
            color: Theme.panelSlate
            radius: 6
            border.width: 1
            border.color: Theme.slate
            property alias text: userInput.text
            TextInput { id: userInput; anchors.fill: parent; anchors.margins: 10; color: Theme.textBright; verticalAlignment: TextInput.AlignVCenter }
        }

        Row {
            spacing: 8

            Rectangle {
                width: 130
                height: 40
                radius: 6
                color: Theme.accentStrong
                border.color: Theme.accent

                Text {
                    anchors.centerIn: parent
                    text: "Сохранить"
                    color: Theme.textBright
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
                color: Theme.slateMid
                border.color: Theme.slateSoft

                Text {
                    anchors.centerIn: parent
                    text: "Проверить подключение"
                    color: Theme.textBright
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        syncSettings()
                        DbSettings.save()
                        if (DbSettings.applyAndConnect()) {
                            statusText.color = Theme.success
                            statusText.text = "Подключение успешно"
                        } else {
                            statusText.color = Theme.danger
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
            color: Theme.muted4
            text: ""
        }
    }
}
