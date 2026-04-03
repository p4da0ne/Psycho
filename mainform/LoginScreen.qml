import QtQuick 2.15
import Saturn.Backend 1.0

Rectangle {
    color: "#0f172a"
    radius: 12
    border.color: "#1e293b"
    border.width: 1

    Column {
        anchors.centerIn: parent
        width: Math.min(parent.width - 64, 420)
        spacing: 12

        Text {
            text: "Авторизация"
            color: "#e2e8f0"
            font.pixelSize: 24
            font.bold: true
        }

        Rectangle {
            id: loginField
            width: parent.width
            height: 40
            color: "#1e293b"
            radius: 6
            border.width: 1
            border.color: "#334155"

            TextInput {
                id: loginInput
                anchors.fill: parent
                anchors.margins: 10
                color: "#f8fafc"
                verticalAlignment: TextInput.AlignVCenter
                clip: true
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 12
                text: loginInput.text.length === 0 ? "Логин" : ""
                color: "#64748b"
            }
        }

        Rectangle {
            id: passwordField
            width: parent.width
            height: 40
            color: "#1e293b"
            radius: 6
            border.width: 1
            border.color: "#334155"

            TextInput {
                id: passwordInput
                anchors.fill: parent
                anchors.margins: 10
                color: "#f8fafc"
                verticalAlignment: TextInput.AlignVCenter
                echoMode: TextInput.Password
                clip: true
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 12
                text: passwordInput.text.length === 0 ? "Пароль" : ""
                color: "#64748b"
            }
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
                    text: "Войти"
                    color: "#f8fafc"
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        if (!Auth.login(loginInput.text, passwordInput.text)) {
                            messageText.text = Auth.lastError
                        } else {
                            messageText.text = "Успешная авторизация"
                        }
                    }
                }
            }

            Rectangle {
                width: 130
                height: 40
                radius: 6
                color: "#475569"
                border.color: "#64748b"

                Text {
                    anchors.centerIn: parent
                    text: "Выйти"
                    color: "#f8fafc"
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
            color: "#fca5a5"
            text: ""
        }
    }
}
