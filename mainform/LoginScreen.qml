import QtQuick 2.15
import Saturn.Backend 1.0
import "qrc:/theme"

Rectangle {
    id: root
    signal loginSucceeded()

    color: Theme.surfaceSlate
    radius: 12
    border.color: Theme.panelSlate
    border.width: 1

    Column {
        anchors.centerIn: parent
        width: Math.min(parent.width - 64, 420)
        spacing: 12

        Text {
            text: "Авторизация"
            color: Theme.textLight3
            font.pixelSize: 24
            font.bold: true
        }

        Rectangle {
            id: loginField
            width: parent.width
            height: 40
            color: Theme.panelSlate
            radius: 6
            border.width: 1
            border.color: Theme.slate

            TextInput {
                id: loginInput
                anchors.fill: parent
                anchors.margins: 10
                color: Theme.textBright
                verticalAlignment: TextInput.AlignVCenter
                clip: true
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 12
                text: loginInput.text.length === 0 ? "Логин" : ""
                color: Theme.slateSoft
            }
        }

        Rectangle {
            id: passwordField
            width: parent.width
            height: 40
            color: Theme.panelSlate
            radius: 6
            border.width: 1
            border.color: Theme.slate

            TextInput {
                id: passwordInput
                anchors.fill: parent
                anchors.margins: 10
                color: Theme.textBright
                verticalAlignment: TextInput.AlignVCenter
                echoMode: TextInput.Password
                clip: true
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 12
                text: passwordInput.text.length === 0 ? "Пароль" : ""
                color: Theme.slateSoft
            }
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
                    text: "Войти"
                    color: Theme.textBright
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        DbSettings.load()
                        // TEST/TEMP NOTE: force PostgreSQL driver in current architecture.
                        DbSettings.driver = "QPSQL"
                        if (!DbSettings.applyAndConnect()) {
                            messageText.text = "Нет подключения к БД: " + Database.lastError
                            return
                        }
                        if (!Auth.login(loginInput.text, passwordInput.text)) {
                            messageText.text = Auth.lastError
                        } else {
                            messageText.text = "Успешная авторизация"
                            root.loginSucceeded()
                        }
                    }
                }
            }

            Rectangle {
                width: 130
                height: 40
                radius: 6
                color: Theme.slateMid
                border.color: Theme.slateSoft

                Text {
                    anchors.centerIn: parent
                    text: "Выйти"
                    color: Theme.textBright
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        Auth.logout()
                        messageText.text = "Сессия завершена"
                    }
                }
            }
        }

        Text {
            id: messageText
            width: parent.width
            wrapMode: Text.Wrap
            color: Theme.danger
            text: ""
        }
    }
}
