import QtQuick 2.15
import QtLocation 6.5
import QtPositioning 6.5
import MapLibre 3.0

Item {
    id: root

    property var appState
    property var objects: []
    property var lines: []
    property var polygons: []
    property var labels: []
    property bool editActive: false
    property string editRole: ""
    property string editGeometryType: ""
    property var editPoints: []
    property bool editClosed: false
    property real labelMinZoom: 8.0
    property int markerHitSize: 42
    property int markerOuterSize: 18
    property int markerOuterSizeSelected: 24
    property int markerInnerSize: 9
    property int markerInnerSizeSelected: 12

    signal interactionActivity()
    signal requestMapMenu(real lon, real lat, real screenX, real screenY)
    signal requestObjectMenu(var objectData, real screenX, real screenY)
    signal requestSelectObject(var objectData)
    signal requestClearSelection()
    signal hoverObject(var objectData)
    signal editPointMoved(int index, real longitude, real latitude)
    signal editPointRemoved(int index)
    signal editPointAppended(real longitude, real latitude)
    signal editPointInserted(int index, real longitude, real latitude)

    function clampZoom(nextZoom) {
        var minZoom = mapView.minimumZoomLevel !== undefined ? Number(mapView.minimumZoomLevel) : 1
        var maxZoom = mapView.maximumZoomLevel !== undefined ? Number(mapView.maximumZoomLevel) : 20
        return Math.max(minZoom, Math.min(maxZoom, Number(nextZoom)))
    }

    function updateScaleMetrics() {
        if (!root.appState || mapView.width < 2 || mapView.height < 2)
            return

        var samplePixels = Math.max(48, Math.min(220, mapView.width * 0.22))
        if (samplePixels >= mapView.width)
            samplePixels = mapView.width - 1
        if (samplePixels < 1)
            return

        var pointA = Qt.point((mapView.width - samplePixels) / 2, mapView.height / 2)
        var pointB = Qt.point(pointA.x + samplePixels, pointA.y)
        var coordA = mapView.toCoordinate(pointA, false)
        var coordB = mapView.toCoordinate(pointB, false)
        if (!coordA.isValid || !coordB.isValid)
            return

        var meters = coordA.distanceTo(coordB)
        if (!(meters > 0))
            return

        var metersPerPixel = meters / samplePixels
        var pixelsPerMillimeter = 3.78
        root.appState.mapMetersPerPixel = metersPerPixel
        root.appState.mapScaleDenominator = metersPerPixel * pixelsPerMillimeter * 1000.0
    }

    function updateViewportBounds() {
        if (!root.appState || mapView.width < 2 || mapView.height < 2)
            return

        var topLeft = mapView.toCoordinate(Qt.point(0, 0), false)
        var topRight = mapView.toCoordinate(Qt.point(mapView.width, 0), false)
        var bottomLeft = mapView.toCoordinate(Qt.point(0, mapView.height), false)
        var bottomRight = mapView.toCoordinate(Qt.point(mapView.width, mapView.height), false)
        if (!topLeft.isValid || !topRight.isValid || !bottomLeft.isValid || !bottomRight.isValid)
            return

        var lats = [topLeft.latitude, topRight.latitude, bottomLeft.latitude, bottomRight.latitude]
        var lons = [topLeft.longitude, topRight.longitude, bottomLeft.longitude, bottomRight.longitude]
        root.appState.viewNorth = Math.max(lats[0], lats[1], lats[2], lats[3])
        root.appState.viewSouth = Math.min(lats[0], lats[1], lats[2], lats[3])
        root.appState.viewEast = Math.max(lons[0], lons[1], lons[2], lons[3])
        root.appState.viewWest = Math.min(lons[0], lons[1], lons[2], lons[3])
    }

    function pathToCoordinates(pathData) {
        var out = []
        var src = pathData || []
        for (var i = 0; i < src.length; ++i) {
            var point = src[i]
            out.push(QtPositioning.coordinate(Number(point.lat), Number(point.lon)))
        }
        return out
    }

    function pathCenter(pathData) {
        var src = pathData || []
        if (src.length === 0)
            return QtPositioning.coordinate(55.7558, 37.6176)
        var latSum = 0
        var lonSum = 0
        for (var i = 0; i < src.length; ++i) {
            latSum += Number(src[i].lat || 0)
            lonSum += Number(src[i].lon || 0)
        }
        return QtPositioning.coordinate(latSum / src.length, lonSum / src.length)
    }

    function strokeColor(sideValue) {
        if (sideValue === "friendly")
            return "#78a3ff"
        if (sideValue === "foreign")
            return "#dbe6f2"
        return "#9dabb8"
    }

    function fillColor(sideValue) {
        if (sideValue === "friendly")
            return Qt.rgba(0.47, 0.64, 1.0, 0.13)
        if (sideValue === "foreign")
            return Qt.rgba(0.92, 0.95, 0.99, 0.11)
        return Qt.rgba(0.62, 0.69, 0.75, 0.10)
    }

    function clamp01(value) {
        var v = Number(value)
        if (isNaN(v))
            return 0
        if (v < 0)
            return 0
        if (v > 1)
            return 1
        return v
    }

    function heatFillColor(value) {
        var v = clamp01(value)
        if (v < 0.25)
            return Qt.rgba(0.27, 0.60, 0.84, 0.20)
        if (v < 0.50)
            return Qt.rgba(0.86, 0.72, 0.25, 0.24)
        if (v < 0.75)
            return Qt.rgba(0.89, 0.45, 0.24, 0.28)
        return Qt.rgba(0.77, 0.24, 0.21, 0.34)
    }

    function heatLineColor(value) {
        var v = clamp01(value)
        if (v < 0.25)
            return Qt.rgba(0.39, 0.71, 0.91, 0.92)
        if (v < 0.50)
            return Qt.rgba(0.90, 0.77, 0.33, 0.94)
        if (v < 0.75)
            return Qt.rgba(0.93, 0.52, 0.28, 0.95)
        return Qt.rgba(0.85, 0.33, 0.30, 0.97)
    }

    function editPathCoordinates() {
        var source = root.editPoints || []
        var out = []
        for (var i = 0; i < source.length; ++i) {
            out.push(QtPositioning.coordinate(
                         Number(source[i].latitude),
                         Number(source[i].longitude)))
        }
        return out
    }

    function editSegmentMidpoints() {
        var source = root.editPoints || []
        var result = []
        if (!root.editActive || source.length < 2)
            return result
        var isLine = root.editGeometryType === "LineString"
        var isPolygon = root.editGeometryType === "Polygon"
        if (!isLine && !isPolygon)
            return result

        for (var i = 0; i < source.length - 1; ++i) {
            var a = source[i]
            var b = source[i + 1]
            result.push({
                "index": i + 1,
                "longitude": (Number(a.longitude) + Number(b.longitude)) / 2.0,
                "latitude": (Number(a.latitude) + Number(b.latitude)) / 2.0
            })
        }

        if (isPolygon && root.editClosed && source.length >= 3) {
            var first = source[0]
            var last = source[source.length - 1]
            result.push({
                "index": source.length,
                "longitude": (Number(first.longitude) + Number(last.longitude)) / 2.0,
                "latitude": (Number(first.latitude) + Number(last.latitude)) / 2.0
            })
        }
        return result
    }

    Plugin {
        id: mapPlugin
        name: "maplibre"

        PluginParameter {
            name: "maplibre.map.styles"
            value: (root.appState && root.appState.mapStyleUrl && root.appState.mapStyleUrl.length > 0)
                ? root.appState.mapStyleUrl
                : "http://localhost:8080/styles/maptiler-basic/style.json"
        }
    }

    Map {
        id: mapView
        anchors.fill: parent

        plugin: mapPlugin
        zoomLevel: root.appState ? root.appState.zoomLevel : 5
        center: QtPositioning.coordinate(
                        root.appState ? root.appState.centerLat : 55.7558,
                        root.appState ? root.appState.centerLon : 37.6176)
        onWidthChanged: {
            root.updateScaleMetrics()
            root.updateViewportBounds()
        }
        onHeightChanged: {
            root.updateScaleMetrics()
            root.updateViewportBounds()
        }
        Component.onCompleted: {
            root.updateScaleMetrics()
            root.updateViewportBounds()
        }

        Connections {
            target: mapView

            function onCenterChanged() {
                if (!root.appState || !mapView.center.isValid)
                    return
                root.appState.centerLat = mapView.center.latitude
                root.appState.centerLon = mapView.center.longitude
                root.updateScaleMetrics()
                root.updateViewportBounds()
            }

            function onZoomLevelChanged() {
                if (!root.appState)
                    return
                root.appState.zoomLevel = mapView.zoomLevel
                root.updateScaleMetrics()
                root.updateViewportBounds()
            }
        }

        WheelHandler {
            target: null

            onWheel: function(event) {
                var rawDelta = 0
                if (event.angleDelta && event.angleDelta.y !== 0)
                    rawDelta = event.angleDelta.y
                else if (event.pixelDelta && event.pixelDelta.y !== 0)
                    rawDelta = event.pixelDelta.y
                if (rawDelta === 0)
                    return

                root.interactionActivity()
                var zoomStep = rawDelta > 0 ? 0.6 : -0.6
                mapView.zoomLevel = root.clampZoom(mapView.zoomLevel + zoomStep)
                event.accepted = true
            }
        }
    }

    MapItemView {
        parent: mapView
        model: root.polygons || []

        delegate: MapPolygon {
            id: polygonItem

            required property var modelData

            path: root.pathToCoordinates(modelData.path)
            border.width: root.appState && root.appState.selectedObject && root.appState.selectedObject.id === modelData.id ? 2 : 1
            border.color: root.appState && root.appState.mapMode === "heatmap"
                ? root.heatLineColor(modelData.heatValue)
                : root.strokeColor(modelData.side)
            color: root.appState && root.appState.mapMode === "heatmap"
                ? root.heatFillColor(modelData.heatValue)
                : root.fillColor(modelData.side)
            smooth: true
            antialiasing: true
        }
    }

    MapItemView {
        parent: mapView
        model: root.polygons || []

        delegate: MapQuickItem {
            id: polygonHotspot

            required property var modelData

            coordinate: root.pathCenter(modelData.path)
            anchorPoint.x: hitArea.width / 2
            anchorPoint.y: hitArea.height / 2
            z: 19

            sourceItem: Item {
                id: hitArea
                width: 44
                height: 44

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    cursorShape: Qt.PointingHandCursor

                    onEntered: root.hoverObject(polygonHotspot.modelData)
                    onExited: root.hoverObject(null)

                    onPressed: function(mouse) {
                        if (mouse.button !== Qt.RightButton)
                            return
                        root.interactionActivity()
                        var point = mapView.fromCoordinate(polygonHotspot.coordinate, false)
                        root.requestObjectMenu(polygonHotspot.modelData, point.x + 16, point.y + 12)
                        mouse.accepted = true
                    }

                    onClicked: function(mouse) {
                        if (mouse.button !== Qt.LeftButton)
                            return
                        root.interactionActivity()
                        root.requestSelectObject(polygonHotspot.modelData)
                    }
                }
            }
        }
    }

    MapItemView {
        parent: mapView
        model: root.lines || []

        delegate: MapPolyline {
            id: lineItem

            required property var modelData

            path: root.pathToCoordinates(modelData.path)
            line.width: root.appState && root.appState.selectedObject && root.appState.selectedObject.id === modelData.id ? 3 : 2
            line.color: root.appState && root.appState.mapMode === "heatmap"
                ? root.heatLineColor(modelData.heatValue)
                : root.strokeColor(modelData.side)
            smooth: true
            antialiasing: true
        }
    }

    MapItemView {
        parent: mapView
        model: root.objects || []

        delegate: MapCircle {
            id: heatCircle

            required property var modelData

            visible: root.appState
                && root.appState.mapMode === "heatmap"
                && root.appState.showHeatmapLayer
                && Number(modelData.heatValue || 0) > 0.01
            center: QtPositioning.coordinate(Number(modelData.lat), Number(modelData.lon))
            radius: 260 + 1740 * root.clamp01(modelData.heatValue)
            color: root.heatFillColor(modelData.heatValue)
            border.width: 0
            z: 8
        }
    }

    MapItemView {
        parent: mapView
        model: root.lines || []

        delegate: MapQuickItem {
            id: lineHotspot

            required property var modelData

            coordinate: root.pathCenter(modelData.path)
            anchorPoint.x: hitArea.width / 2
            anchorPoint.y: hitArea.height / 2
            z: 19

            sourceItem: Item {
                id: hitArea
                width: 38
                height: 38

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    cursorShape: Qt.PointingHandCursor

                    onEntered: root.hoverObject(lineHotspot.modelData)
                    onExited: root.hoverObject(null)

                    onPressed: function(mouse) {
                        if (mouse.button !== Qt.RightButton)
                            return
                        root.interactionActivity()
                        var point = mapView.fromCoordinate(lineHotspot.coordinate, false)
                        root.requestObjectMenu(lineHotspot.modelData, point.x + 16, point.y + 12)
                        mouse.accepted = true
                    }

                    onClicked: function(mouse) {
                        if (mouse.button !== Qt.LeftButton)
                            return
                        root.interactionActivity()
                        root.requestSelectObject(lineHotspot.modelData)
                    }
                }
            }
        }
    }

    MapItemView {
        parent: mapView
        model: root.labels || []

        delegate: MapQuickItem {
            id: labelItem

            required property var modelData

            coordinate: QtPositioning.coordinate(Number(modelData.lat), Number(modelData.lon))
            anchorPoint.x: labelText.width / 2
            anchorPoint.y: labelText.height / 2
            visible: (root.appState ? root.appState.showLabels : true) && mapView.zoomLevel >= root.labelMinZoom

            sourceItem: Text {
                id: labelText
                text: String(labelItem.modelData.name || "")
                color: "#808995"
                font.pixelSize: 17
                opacity: root.appState && root.appState.selectedObject ? 0.42 : 0.80
            }
        }
    }

    MapPolyline {
        visible: root.editActive && root.editGeometryType === "LineString" && (root.editPoints || []).length >= 2
        parent: mapView
        path: root.editPathCoordinates()
        line.width: 3
        line.color: "#60a5fa"
        smooth: true
        antialiasing: true
    }

    MapPolygon {
        visible: root.editActive && root.editGeometryType === "Polygon" && (root.editPoints || []).length >= 3
        parent: mapView
        path: root.editPathCoordinates()
        color: Qt.rgba(0.38, 0.64, 1.0, 0.16)
        border.width: 2
        border.color: "#60a5fa"
        smooth: true
        antialiasing: true
    }

    MapItemView {
        parent: mapView
        model: root.editPoints || []
        visible: root.editActive

        delegate: MapQuickItem {
            id: editVertex

            required property int index
            required property var modelData

            coordinate: QtPositioning.coordinate(Number(modelData.latitude), Number(modelData.longitude))
            anchorPoint.x: vertexHit.width / 2
            anchorPoint.y: vertexHit.height / 2
            z: 100

            sourceItem: Item {
                id: vertexHit
                width: 36
                height: 36

                Rectangle {
                    anchors.centerIn: parent
                    width: 14
                    height: 14
                    radius: 7
                    color: "#f8fafc"
                    border.width: 2
                    border.color: "#2563eb"
                }

                MouseArea {
                    id: vertexMouse
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    cursorShape: Qt.PointingHandCursor
                    drag.target: null
                    property bool dragging: false

                    onPressed: function(mouse) {
                        if (mouse.button === Qt.RightButton) {
                            root.editPointRemoved(editVertex.index)
                            return
                        }
                        dragging = true
                    }

                    onPositionChanged: function(mouse) {
                        if (!dragging)
                            return
                        var p = vertexHit.mapToItem(mapView, mouse.x, mouse.y)
                        var c = mapView.toCoordinate(Qt.point(p.x, p.y), false)
                        root.editPointMoved(editVertex.index, c.longitude, c.latitude)
                    }

                    onReleased: dragging = false
                    onCanceled: dragging = false
                }
            }
        }
    }

    MapItemView {
        parent: mapView
        model: root.editSegmentMidpoints()
        visible: root.editActive && (root.editGeometryType === "LineString" || root.editGeometryType === "Polygon")

        delegate: MapQuickItem {
            id: editSegmentPoint

            required property var modelData

            coordinate: QtPositioning.coordinate(Number(modelData.latitude), Number(modelData.longitude))
            anchorPoint.x: segmentHit.width / 2
            anchorPoint.y: segmentHit.height / 2
            z: 95

            sourceItem: Item {
                id: segmentHit
                width: 26
                height: 26

                Rectangle {
                    anchors.centerIn: parent
                    width: 12
                    height: 12
                    rotation: 45
                    color: "#bfdbfe"
                    border.width: 1
                    border.color: "#1d4ed8"
                }

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
                    cursorShape: Qt.CrossCursor
                    onClicked: root.editPointInserted(
                                   Number(editSegmentPoint.modelData.index),
                                   Number(editSegmentPoint.modelData.longitude),
                                   Number(editSegmentPoint.modelData.latitude))
                }
            }
        }
    }

    MapItemView {
        parent: mapView
        model: root.objects || []

        delegate: MapQuickItem {
            id: markerItem

            required property var modelData

            readonly property bool selected: root.appState
                && root.appState.selectedObject
                && root.appState.selectedObject.id === modelData.id
            readonly property bool hovered: root.appState
                && root.appState.hoveredObject
                && root.appState.hoveredObject.id === modelData.id
            readonly property bool muted: root.appState && root.appState.selectedObject && !selected
            readonly property string markerColor: modelData.side === "friendly"
                ? "#7298f6"
                : (modelData.side === "foreign" ? "#d7dee7" : "#909ba7")
            readonly property string markerTopColor: modelData.side === "friendly"
                ? "#8DB7FF"
                : (modelData.side === "foreign" ? "#EEF4FB" : "#ACB6C2")

            coordinate: QtPositioning.coordinate(Number(modelData.lat), Number(modelData.lon))
            anchorPoint.x: markerRoot.width / 2
            anchorPoint.y: markerRoot.height / 2
            z: selected ? 20 : 10

            sourceItem: Item {
                id: markerRoot
                width: root.markerHitSize
                height: root.markerHitSize
                opacity: markerItem.muted ? 0.56 : 1.0
                scale: markerItem.selected ? 1.15 : markerItem.hovered ? 1.03 : 1.0

                Rectangle {
                    anchors.centerIn: parent
                    width: markerItem.selected ? root.markerOuterSizeSelected : root.markerOuterSize
                    height: width
                    radius: markerItem.modelData.kind === "lbs" ? 3 : width / 2
                    border.width: 1
                    border.color: markerItem.selected ? "#F2F7FC" : Qt.rgba(1, 1, 1, 0.26)
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: markerItem.markerTopColor }
                        GradientStop { position: 1.0; color: markerItem.markerColor }
                    }
                }

                Rectangle {
                    anchors.centerIn: parent
                    width: markerItem.selected ? root.markerInnerSizeSelected : root.markerInnerSize
                    height: width
                    radius: width / 2
                    color: "#f6f9fc"
                    opacity: markerItem.modelData.side === "foreign" ? 0.72 : 0.88
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    cursorShape: Qt.PointingHandCursor

                    onEntered: root.hoverObject(markerItem.modelData)
                    onExited: root.hoverObject(null)

                    onPressed: function(mouse) {
                        if (mouse.button !== Qt.RightButton)
                            return
                        root.interactionActivity()
                        var point = mapView.fromCoordinate(
                                        QtPositioning.coordinate(Number(markerItem.modelData.lat),
                                                                 Number(markerItem.modelData.lon)),
                                        false)
                        root.requestObjectMenu(markerItem.modelData, point.x + 16, point.y + 12)
                        mouse.accepted = true
                    }

                    onClicked: function(mouse) {
                        if (mouse.button !== Qt.LeftButton)
                            return
                        root.interactionActivity()
                        root.requestSelectObject(markerItem.modelData)
                    }
                }
            }
        }
    }

    DragHandler {
        id: mapPanHandler
        target: null
        acceptedButtons: Qt.LeftButton
        enabled: !root.editActive
        property point lastPosition: Qt.point(0, 0)

        onActiveChanged: {
            if (!active)
                return
            lastPosition = centroid.position
            root.interactionActivity()
        }

        onCentroidChanged: {
            if (!active)
                return
            var dx = centroid.position.x - lastPosition.x
            var dy = centroid.position.y - lastPosition.y
            if (dx === 0 && dy === 0)
                return
            mapView.pan(-dx, -dy)
            lastPosition = centroid.position
            root.interactionActivity()
        }
    }

    MouseArea {
        id: mapHoverArea
        anchors.fill: parent
        acceptedButtons: Qt.NoButton
        hoverEnabled: true
        propagateComposedEvents: true

        onPositionChanged: function(mouse) {
            if (!root.appState)
                return
            var coord = mapView.toCoordinate(Qt.point(mouse.x, mouse.y), false)
            root.appState.cursorLon = coord.longitude
            root.appState.cursorLat = coord.latitude
        }

    }

    TapHandler {
        id: mapTapHandler
        target: null
        acceptedDevices: PointerDevice.Mouse
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        grabPermissions: PointerHandler.CanTakeOverFromAnything

        onTapped: function(point, button) {
            root.interactionActivity()
            if (button === Qt.RightButton) {
                var coord = mapView.toCoordinate(point.position, false)
                root.requestMapMenu(coord.longitude, coord.latitude, point.position.x, point.position.y)
                return
            }

            if (root.editActive) {
                var c = mapView.toCoordinate(point.position, false)
                root.editPointAppended(c.longitude, c.latitude)
                return
            }
            root.requestClearSelection()
        }
    }
}




