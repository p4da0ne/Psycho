pragma ComponentBehavior: Bound
import QtQuick 2.15
import mapview 1.0

Rectangle {
    id: root

    color: "#0B0F14"
    clip: true

    property var appState
    property var objects: []
    property var labels: []
    property alias sceneContent: mapScene

    signal interactionActivity()

    readonly property real lonSpan: 0.78 / Math.pow(1.18, Math.max(0, appState ? appState.zoomLevel - 10.0 : 0))
    readonly property real latSpan: lonSpan * (height / Math.max(width, 1)) * 0.78
    property real parallaxX: 0
    property real parallaxY: 0

    Behavior on parallaxX {
        NumberAnimation { duration: 180 }
    }

    Behavior on parallaxY {
        NumberAnimation { duration: 180 }
    }

    property var waterBodies: [
        { "lon": 37.49, "lat": 55.79, "w": 0.22, "h": 0.09, "r": -14 },
        { "lon": 37.73, "lat": 55.77, "w": 0.14, "h": 0.06, "r": 12 },
        { "lon": 37.58, "lat": 55.67, "w": 0.16, "h": 0.07, "r": 24 }
    ]

    property var routeLines: [
        [
            { "lon": 37.50, "lat": 55.782 },
            { "lon": 37.57, "lat": 55.764 },
            { "lon": 37.63, "lat": 55.748 },
            { "lon": 37.69, "lat": 55.735 }
        ],
        [
            { "lon": 37.48, "lat": 55.724 },
            { "lon": 37.56, "lat": 55.733 },
            { "lon": 37.63, "lat": 55.750 },
            { "lon": 37.70, "lat": 55.768 }
        ]
    ]

    function projectX(lon) {
        return ((lon - (appState.centerLon - lonSpan / 2)) / lonSpan) * width
    }

    function projectY(lat) {
        return ((appState.centerLat + latSpan / 2 - lat) / latSpan) * height
    }

    function unprojectLon(xPos) {
        return (appState.centerLon - lonSpan / 2) + (xPos / width) * lonSpan
    }

    function unprojectLat(yPos) {
        return (appState.centerLat + latSpan / 2) - (yPos / height) * latSpan
    }

    function markerVisible(objectData) {
        if (!objectData || !appState)
            return false

        if (objectData.side === "friendly" && !appState.showFriendly)
            return false
        if (objectData.side === "foreign" && !appState.showForeign)
            return false
        if (objectData.side === "infrastructure" && !appState.showBaseStations)
            return false
        if (appState.searchText !== "") {
            var needle = appState.searchText.toLowerCase()
            var haystack = (objectData.name + " " + objectData.id + " " + objectData.kind).toLowerCase()
            if (haystack.indexOf(needle) === -1)
                return false
        }

        var px = projectX(objectData.lon)
        var py = projectY(objectData.lat)
        return px >= -80 && px <= width + 80 && py >= -80 && py <= height + 80
    }

    function filteredObjects() {
        return objects.filter(function(objectData) {
            return markerVisible(objectData)
        })
    }

    function coverageObjects() {
        return filteredObjects().filter(function(objectData) {
            return objectData.kind === "lbs"
        })
    }

    function ambientGlowObjects() {
        var visible = filteredObjects()
        var ambient = []
        var selected = appState ? appState.selectedObject : null

        if (selected && markerVisible(selected))
            ambient.push(selected)

        for (var i = 0; i < visible.length && ambient.length < 2; ++i) {
            var candidate = visible[i]
            var alreadyAdded = selected && candidate.id === selected.id
            if (!alreadyAdded && (candidate.kind === "lbs" || candidate.side === "friendly"))
                ambient.push(candidate)
        }

        return ambient
    }

    function markerColor(objectData) {
        if (objectData.side === "friendly")
            return "#7298f6"
        if (objectData.side === "foreign")
            return "#d7dee7"
        return "#909ba7"
    }

    function markerTopColor(objectData) {
        if (objectData.side === "friendly")
            return "#8DB7FF"
        if (objectData.side === "foreign")
            return "#EEF4FB"
        return "#ACB6C2"
    }

    function markerInnerOpacity(objectData) {
        return objectData.side === "foreign" ? 0.72 : 0.88
    }

    function gridStep(spanValue) {
        var rough = spanValue / 6
        var base = Math.pow(10, Math.floor(Math.log(rough) / Math.LN10))
        var ratio = rough / base

        if (ratio > 5)
            return base * 5
        if (ratio > 2)
            return base * 2
        return base
    }

    function openMapMenu(xPos, yPos) {
        appState.lastContextLon = unprojectLon(xPos)
        appState.lastContextLat = unprojectLat(yPos)
        mapMenu.popup(xPos, yPos)
    }

    function repaintOverlays() {
        baseLayer.requestPaint()
        heatCanvas.requestPaint()
        coverageCanvas.requestPaint()
    }

    Item {
        id: mapScene
        anchors.fill: parent
        transform: Translate {
            x: root.parallaxX
            y: root.parallaxY
        }

        // TEMPORARY(TEST): transitional integration layer.
        // Real MapLibre surface is already mounted; legacy custom overlays remain
        // until full migration of interactions and symbol rendering is completed.
        MyMapView {
            id: mapLibreSurface
            anchors.fill: parent
        }

        Canvas {
            id: baseLayer

            anchors.fill: parent

            onPaint: {
                var ctx = getContext("2d")
                ctx.clearRect(0, 0, width, height)

                var backgroundGradient = ctx.createLinearGradient(0, 0, 0, height)
                backgroundGradient.addColorStop(0.0, "#0B0F14")
                backgroundGradient.addColorStop(0.56, "#0F151D")
                backgroundGradient.addColorStop(1.0, "#101722")
                ctx.fillStyle = backgroundGradient
                ctx.fillRect(0, 0, width, height)

                var ambientObjects = root.ambientGlowObjects()
                for (var t = 0; t < ambientObjects.length; ++t) {
                    var formObject = ambientObjects[t]
                    var formX = root.projectX(formObject.lon)
                    var formY = root.projectY(formObject.lat)
                    var glowRadius = 12 + formObject.mpps * 0.14
                    var glow = ctx.createRadialGradient(formX, formY, 2, formX, formY, glowRadius)

                    glow.addColorStop(0.0, formObject.kind === "lbs"
                        ? "rgba(150,174,205,0.028)"
                        : "rgba(111,168,255,0.024)")
                    glow.addColorStop(0.42, formObject.kind === "lbs"
                        ? "rgba(150,174,205,0.012)"
                        : "rgba(111,168,255,0.010)")
                    glow.addColorStop(0.72, formObject.kind === "lbs"
                        ? "rgba(150,174,205,0.005)"
                        : "rgba(111,168,255,0.004)")
                    glow.addColorStop(1.0, "rgba(255,255,255,0.0)")

                    ctx.fillStyle = glow
                    ctx.beginPath()
                    ctx.arc(formX, formY, glowRadius, 0, Math.PI * 2)
                    ctx.fill()
                }

                var vStep = root.gridStep(root.lonSpan)
                var hStep = root.gridStep(root.latSpan)
                var startLon = Math.floor((root.appState.centerLon - root.lonSpan / 2) / vStep) * vStep
                var endLon = root.appState.centerLon + root.lonSpan / 2
                var startLat = Math.floor((root.appState.centerLat - root.latSpan / 2) / hStep) * hStep
                var endLat = root.appState.centerLat + root.latSpan / 2

                ctx.strokeStyle = "rgba(255,255,255,0.04)"
                ctx.lineWidth = 0.5

                for (var lon = startLon; lon <= endLon; lon += vStep) {
                    var x = root.projectX(lon)
                    ctx.beginPath()
                    ctx.moveTo(x, 0)
                    ctx.lineTo(x, height)
                    ctx.stroke()
                }

                for (var lat = startLat; lat <= endLat; lat += hStep) {
                    var y = root.projectY(lat)
                    ctx.beginPath()
                    ctx.moveTo(0, y)
                    ctx.lineTo(width, y)
                    ctx.stroke()
                }

                for (var w = 0; w < root.waterBodies.length; ++w) {
                    var area = root.waterBodies[w]
                    var px = root.projectX(area.lon)
                    var py = root.projectY(area.lat)
                    var rx = (area.w / root.lonSpan) * width / 2
                    var ry = (area.h / root.latSpan) * height / 2

                    ctx.save()
                    ctx.translate(px, py)
                    ctx.rotate(area.r * Math.PI / 180)
                    ctx.scale(rx, ry)
                    ctx.beginPath()
                    ctx.arc(0, 0, 1, 0, Math.PI * 2)
                    ctx.fillStyle = "rgba(74,95,122,0.15)"
                    ctx.fill()
                    ctx.strokeStyle = "rgba(152,174,198,0.035)"
                    ctx.lineWidth = 0.7
                    ctx.stroke()
                    ctx.restore()
                }

                ctx.strokeStyle = root.appState.selectedObject
                    ? "rgba(120,160,200,0.12)"
                    : "rgba(120,160,200,0.16)"
                ctx.lineWidth = 0.6
                ctx.lineCap = "round"
                ctx.lineJoin = "round"

                for (var i = 0; i < root.routeLines.length; ++i) {
                    var route = root.routeLines[i]
                    ctx.beginPath()
                    for (var p = 0; p < route.length; ++p) {
                        var point = route[p]
                        var routeX = root.projectX(point.lon)
                        var routeY = root.projectY(point.lat)
                        if (p === 0)
                            ctx.moveTo(routeX, routeY)
                        else
                            ctx.lineTo(routeX, routeY)
                    }
                    ctx.stroke()
                }
            }
        }

        Canvas {
            id: heatCanvas

            anchors.fill: parent
            visible: root.appState && root.appState.mapMode === "heatmap" && root.appState.showHeatmapLayer
            opacity: root.appState && root.appState.selectedObject ? 0.08 : 0.115

            onPaint: {
                var ctx = getContext("2d")
                ctx.clearRect(0, 0, width, height)

                var visibleObjects = root.filteredObjects()
                for (var i = 0; i < visibleObjects.length; ++i) {
                    var objectData = visibleObjects[i]
                    var px = root.projectX(objectData.lon)
                    var py = root.projectY(objectData.lat)
                    var radius = 14 + objectData.mpps * 0.32
                    var gradient = ctx.createRadialGradient(px, py, 4, px, py, radius)

                    gradient.addColorStop(0.0, "rgba(120,160,200,0.10)")
                    gradient.addColorStop(0.24, "rgba(120,160,200,0.055)")
                    gradient.addColorStop(0.55, "rgba(120,160,200,0.022)")
                    gradient.addColorStop(0.78, "rgba(157,194,255,0.008)")
                    gradient.addColorStop(1.0, "rgba(255,255,255,0.0)")

                    ctx.fillStyle = gradient
                    ctx.beginPath()
                    ctx.arc(px, py, radius, 0, Math.PI * 2)
                    ctx.fill()
                }
            }
        }

        Canvas {
            id: coverageCanvas

            anchors.fill: parent
            visible: root.appState && root.appState.showCoverageLine

            onPaint: {
                var ctx = getContext("2d")
                ctx.clearRect(0, 0, width, height)

                var lineObjects = root.coverageObjects()
                if (lineObjects.length < 2)
                    return

                ctx.strokeStyle = root.appState.selectedObject
                    ? "rgba(120,160,200,0.15)"
                    : "rgba(120,160,200,0.18)"
                ctx.lineWidth = 0.7
                ctx.lineCap = "round"
                ctx.lineJoin = "round"
                ctx.beginPath()

                for (var i = 0; i < lineObjects.length; ++i) {
                    var point = lineObjects[i]
                    var px = root.projectX(point.lon)
                    var py = root.projectY(point.lat)
                    if (i === 0)
                        ctx.moveTo(px, py)
                    else
                        ctx.lineTo(px, py)
                }

                ctx.stroke()
            }
        }

        Repeater {
            model: root.labels

            delegate: Item {
                id: labelItem

                required property var modelData

                visible: root.appState && root.appState.showLabels
                x: root.projectX(labelItem.modelData.lon) - 68
                y: root.projectY(labelItem.modelData.lat) - 11
                width: 136
                height: 22
                opacity: root.appState.selectedObject ? 0.42 : 0.80

                Text {
                    anchors.centerIn: parent
                    width: parent.width
                    text: labelItem.modelData.name
                    color: "#808995"
                    font.pixelSize: 11
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignHCenter
                    elide: Text.ElideRight
                }
            }
        }

        Repeater {
            model: root.filteredObjects()

            delegate: Item {
                id: markerItem

                required property var modelData

                readonly property bool selected: root.appState
                    && root.appState.selectedObject
                    && root.appState.selectedObject.id === markerItem.modelData.id
                readonly property bool hovered: root.appState
                    && root.appState.hoveredObject
                    && root.appState.hoveredObject.id === markerItem.modelData.id
                readonly property bool muted: root.appState.selectedObject && !selected

                width: 22
                height: 22
                x: root.projectX(markerItem.modelData.lon) - width / 2
                y: root.projectY(markerItem.modelData.lat) - height / 2
                z: selected ? 20 : 10
                scale: selected ? 1.15 : hovered ? 1.035 : 1.0
                opacity: muted ? 0.56 : 1.0

                Behavior on scale {
                    SpringAnimation {
                        spring: 4.2
                        damping: 0.36
                    }
                }

                Behavior on opacity {
                    NumberAnimation {
                        duration: 180
                    }
                }

                Rectangle {
                    anchors.centerIn: parent
                    width: markerItem.selected ? 10 : 8
                    height: width
                    radius: markerItem.modelData.kind === "lbs" ? 3 : width / 2
                    border.width: 1
                    border.color: markerItem.selected ? "#F2F7FC" : Qt.rgba(1, 1, 1, 0.26)
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: root.markerTopColor(markerItem.modelData) }
                        GradientStop { position: 1.0; color: root.markerColor(markerItem.modelData) }
                    }
                }

                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: markerItem.selected ? 1 : 0.5
                    width: markerItem.selected ? 10 : 8
                    height: width
                    radius: markerItem.modelData.kind === "lbs" ? 3 : width / 2
                    color: "#000000"
                    opacity: markerItem.selected ? 0.06 : 0.04
                }

                Rectangle {
                    anchors.centerIn: parent
                    width: markerItem.selected ? 18 : 12
                    height: width
                    radius: width / 2
                    color: root.markerColor(markerItem.modelData)
                    opacity: markerItem.selected ? 0.045 : markerItem.hovered ? 0.008 : 0.0

                    Behavior on opacity {
                        NumberAnimation {
                            duration: 180
                        }
                    }
                }

                Rectangle {
                    anchors.centerIn: parent
                    width: markerItem.selected ? 5 : 4
                    height: width
                    radius: width / 2
                    color: "#f6f9fc"
                    opacity: root.markerInnerOpacity(markerItem.modelData)
                }

                Rectangle {
                    width: markerItem.selected ? 4 : 3
                    height: markerItem.selected ? 2 : 1
                    radius: height / 2
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.verticalCenterOffset: markerItem.selected ? -2 : -1
                    color: "#ffffff"
                    opacity: markerItem.selected ? 0.18 : 0.12
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    cursorShape: Qt.PointingHandCursor

                    onEntered: root.appState.hoveredObject = markerItem.modelData
                    onExited: {
                        if (root.appState.hoveredObject && root.appState.hoveredObject.id === markerItem.modelData.id)
                            root.appState.hoveredObject = null
                    }

                    onClicked: function(mouse) {
                        root.interactionActivity()
                        root.appState.selectObject(markerItem.modelData)
                        if (mouse.button === Qt.RightButton) {
                            objectMenu.objectData = markerItem.modelData
                            objectMenu.popup(markerItem.x + 18, markerItem.y + 12)
                        }
                    }
                }
            }
        }
    }

    GlassPanel {
        id: markerCard

        visible: !!root.appState.selectedObject
        z: 30
        width: 208
        height: 82
        radius: 20
        padding: 12
        backdropSource: root.sceneContent
        surfaceColor: "#10161d"
        surfaceOpacity: 0.58
        shadowOpacity: 0.08
        highlightOpacity: 0.05
        x: root.appState.selectedObject
            ? Math.max(22, Math.min(root.width - width - 22, root.projectX(root.appState.selectedObject.lon) + 20))
            : 22
        y: root.appState.selectedObject
            ? Math.max(78, Math.min(root.height - height - 146, root.projectY(root.appState.selectedObject.lat) - height - 14))
            : 78
        opacity: visible ? 1 : 0
        scale: visible ? 1 : 0.95

        Behavior on opacity {
            NumberAnimation {
                duration: 240
            }
        }

        Behavior on scale {
            NumberAnimation {
                duration: 240
            }
        }

        Column {
            width: parent.width
            spacing: 4

            Text {
                width: parent.width
                text: root.appState.selectedObject ? root.appState.selectedObject.name : ""
                color: "#f3f7fb"
                font.pixelSize: 13
                font.weight: Font.Medium
                lineHeight: 1.12
                elide: Text.ElideRight
            }

            Text {
                width: parent.width
                text: root.appState.selectedObject
                    ? root.appState.selectedObject.kind + " · " + root.appState.selectedObject.side
                    : ""
                color: "#909daa"
                font.pixelSize: 9
                lineHeight: 1.12
                elide: Text.ElideRight
            }

            Text {
                width: parent.width
                text: root.appState.selectedObject
                    ? "MPPS " + root.appState.selectedObject.mpps
                    : ""
                color: "#a3beff"
                font.pixelSize: 10
                elide: Text.ElideRight
            }

            Text {
                width: parent.width
                text: root.appState.selectedObject
                    ? Number(root.appState.selectedObject.lon).toFixed(5) + ", "
                      + Number(root.appState.selectedObject.lat).toFixed(5)
                    : ""
                color: "#8b98a6"
                font.family: "Menlo"
                font.pixelSize: 9
                elide: Text.ElideRight
            }
        }
    }

    MouseArea {
        id: mapMouse

        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true
        preventStealing: true

        property real startMouseX: 0
        property real startMouseY: 0
        property real startCenterLon: 0
        property real startCenterLat: 0

        onPressed: function(mouse) {
            root.interactionActivity()
            startMouseX = mouse.x
            startMouseY = mouse.y
            startCenterLon = root.appState.centerLon
            startCenterLat = root.appState.centerLat
            root.appState.cursorLon = root.unprojectLon(mouse.x)
            root.appState.cursorLat = root.unprojectLat(mouse.y)
        }

        onPositionChanged: function(mouse) {
            root.appState.cursorLon = root.unprojectLon(mouse.x)
            root.appState.cursorLat = root.unprojectLat(mouse.y)
            root.parallaxX = (mouse.x / Math.max(root.width, 1) - 0.5) * 2.4
            root.parallaxY = (mouse.y / Math.max(root.height, 1) - 0.5) * 2.4

            if (pressedButtons & Qt.LeftButton) {
                root.interactionActivity()
                var dx = mouse.x - startMouseX
                var dy = mouse.y - startMouseY
                root.appState.centerLon = startCenterLon - (dx / root.width) * root.lonSpan
                root.appState.centerLat = startCenterLat + (dy / root.height) * root.latSpan
                root.repaintOverlays()
            }
        }

        onClicked: function(mouse) {
            root.appState.cursorLon = root.unprojectLon(mouse.x)
            root.appState.cursorLat = root.unprojectLat(mouse.y)

            if (mouse.button === Qt.RightButton) {
                root.openMapMenu(mouse.x, mouse.y)
            } else {
                root.appState.clearSelection()
            }
        }

        onExited: {
            root.parallaxX = 0
            root.parallaxY = 0
        }
    }

    WheelHandler {
        acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad

        onWheel: function(event) {
            root.interactionActivity()
            var delta = event.angleDelta.y > 0 ? 0.35 : -0.35
            root.appState.zoomLevel = Math.max(9.5, Math.min(15.5, root.appState.zoomLevel + delta))
            root.repaintOverlays()
        }
    }

    Connections {
        target: root.appState

        function onMapModeChanged() { root.repaintOverlays() }
        function onSearchTextChanged() { root.repaintOverlays() }
        function onShowFriendlyChanged() { root.repaintOverlays() }
        function onShowForeignChanged() { root.repaintOverlays() }
        function onShowBaseStationsChanged() { root.repaintOverlays() }
        function onShowCoverageLineChanged() { root.repaintOverlays() }
        function onShowHeatmapLayerChanged() { root.repaintOverlays() }
        function onShowLabelsChanged() { root.repaintOverlays() }
        function onCenterLonChanged() { root.repaintOverlays() }
        function onCenterLatChanged() { root.repaintOverlays() }
        function onZoomLevelChanged() { root.repaintOverlays() }
    }

    onWidthChanged: root.repaintOverlays()
    onHeightChanged: root.repaintOverlays()

    ContextMenuMap {
        id: mapMenu
        appState: root.appState
    }

    ContextMenuObject {
        id: objectMenu
        appState: root.appState
    }
}
