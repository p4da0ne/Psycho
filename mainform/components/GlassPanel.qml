import QtQuick 2.15
import QtQuick.Effects

Item {
    id: root

    property real radius: 24
    property real padding: 16
    property color surfaceColor: "#10161e"
    property real surfaceOpacity: 0.50
    property real noiseOpacity: 0.008
    property real shadowOpacity: 0.09
    property real highlightOpacity: 0.11
    property real edgeOpacity: 0.06
    property Item backdropSource: null
    property bool useBackdrop: backdropSource !== null
    default property alias contentData: contentRoot.data
    property alias contentItem: contentRoot

    implicitWidth: contentRoot.implicitWidth + padding * 2
    implicitHeight: contentRoot.implicitHeight + padding * 2

    Rectangle {
        anchors.fill: panel
        anchors.margins: -8
        y: 10
        radius: root.radius + 8
        color: "#000000"
        opacity: root.shadowOpacity * 0.10
    }

    Rectangle {
        anchors.fill: panel
        anchors.margins: -3
        y: 5
        radius: root.radius + 3
        color: "#000000"
        opacity: root.shadowOpacity * 0.36
    }

    ShaderEffectSource {
        id: sourceCapture

        visible: false
        live: true
        hideSource: false
        recursive: false
        sourceItem: root.backdropSource
        sourceRect: {
            if (!root.backdropSource)
                return Qt.rect(0, 0, root.width, root.height)

            var pointInSource = root.backdropSource.mapFromItem(root, 0, 0)
            return Qt.rect(pointInSource.x, pointInSource.y, root.width, root.height)
        }
    }

    Rectangle {
        id: panel

        anchors.fill: parent
        radius: root.radius
        color: root.surfaceColor
        opacity: root.surfaceOpacity
        clip: true

        MultiEffect {
            anchors.fill: parent
            visible: root.useBackdrop
            source: sourceCapture
            blurEnabled: true
            blurMax: 24
            blur: 0.46
            saturation: 1.01
            brightness: 0.01
        }

        Rectangle {
            anchors.fill: parent
            radius: parent.radius
            color: root.surfaceColor
            opacity: root.surfaceOpacity
        }

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 1
            radius: parent.radius
            color: "#ffffff"
            opacity: root.highlightOpacity
        }

        Rectangle {
            width: parent.width * 0.60
            height: parent.height * 0.14
            radius: parent.radius
            x: parent.width * 0.20
            y: 1
            color: "#ffffff"
            opacity: root.highlightOpacity * 0.07
        }

        Rectangle {
            anchors.fill: parent
            radius: parent.radius
            color: "#000000"
            opacity: 0.032
        }

        Rectangle {
            anchors.fill: parent
            radius: parent.radius
            color: "transparent"
            border.width: 1
            border.color: Qt.rgba(1, 1, 1, root.edgeOpacity)
        }

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: parent.height * 0.16
            radius: parent.radius
            color: "#000000"
            opacity: 0.035
        }

        Rectangle {
            anchors.fill: parent
            radius: parent.radius
            color: "transparent"
            border.width: 1
            border.color: Qt.rgba(1, 1, 1, 0.03)
        }

        Canvas {
            anchors.fill: parent
            opacity: root.noiseOpacity

            onPaint: {
                var ctx = getContext("2d")
                ctx.clearRect(0, 0, width, height)

                var points = Math.max(80, width * height / 260)
                for (var i = 0; i < points; ++i) {
                    var shade = i % 3 === 0 ? 255 : 0
                    ctx.fillStyle = "rgba(" + shade + "," + shade + "," + shade + ",1)"
                    ctx.fillRect(Math.random() * width, Math.random() * height, 1, 1)
                }
            }

            Component.onCompleted: requestPaint()
            onWidthChanged: requestPaint()
            onHeightChanged: requestPaint()
        }
    }

    Item {
        id: contentRoot
        anchors.fill: parent
        anchors.margins: root.padding
    }
}
