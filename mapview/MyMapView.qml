import QtQuick 2.15
import QtLocation 6.5
import QtPositioning 6.5
import MapLibre 3.0
import Saturn.Backend 1.0

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
    property bool interactionInProgress: false
    property double lastInteractionSignalTs: 0
    property point pendingCursorPoint: Qt.point(0, 0)
    // Опционально: каталог с файлами gadm41_RUS_<level>.json для overlay админграниц.
    // Пусто => overlay выключен (границы берутся из стиля tileserver). См. README.
    property string gadmBasePath: ""
    property var gadmAdm0Data: emptyFeatureCollection()
    property var gadmAdm1Data: emptyFeatureCollection()
    property var gadmAdm2Data: emptyFeatureCollection()
    property var gadmAdm3Data: emptyFeatureCollection()
    property bool gadmAdm0Loaded: false
    property bool gadmAdm1Loaded: false
    property bool gadmAdm2Loaded: false
    property bool gadmAdm3Loaded: false

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
        var minZoom = mapView.map.minimumZoomLevel !== undefined ? Number(mapView.map.minimumZoomLevel) : 1
        var maxZoom = mapView.map.maximumZoomLevel !== undefined ? Number(mapView.map.maximumZoomLevel) : 20
        return Math.max(minZoom, Math.min(maxZoom, Number(nextZoom)))
    }

    function markInteraction() {
        interactionInProgress = true
        interactionIdleTimer.restart()
        var now = Date.now()
        if (now - lastInteractionSignalTs >= 120) {
            lastInteractionSignalTs = now
            root.interactionActivity()
        }
    }

    function scheduleViewportUpdate(immediate) {
        if (immediate) {
            updateScaleMetrics()
            updateViewportBounds()
            return
        }
        viewportUpdateTimer.restart()
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
        var coordA = mapView.map.toCoordinate(pointA, false)
        var coordB = mapView.map.toCoordinate(pointB, false)
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

        var topLeft = mapView.map.toCoordinate(Qt.point(0, 0), false)
        var topRight = mapView.map.toCoordinate(Qt.point(mapView.width, 0), false)
        var bottomLeft = mapView.map.toCoordinate(Qt.point(0, mapView.height), false)
        var bottomRight = mapView.map.toCoordinate(Qt.point(mapView.width, mapView.height), false)
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

    function geoPointFromLonLat(lon, lat) {
        return {
            "type": "Point",
            "coordinates": [Number(lon), Number(lat)]
        }
    }

    function geoLineFromPath(pathData) {
        var coords = []
        var src = pathData || []
        for (var i = 0; i < src.length; ++i) {
            coords.push([Number(src[i].lon), Number(src[i].lat)])
        }
        if (coords.length < 2)
            return null
        return {
            "type": "LineString",
            "coordinates": coords
        }
    }

    function geoPolygonFromPath(pathData) {
        var ring = []
        var src = pathData || []
        for (var i = 0; i < src.length; ++i) {
            ring.push([Number(src[i].lon), Number(src[i].lat)])
        }
        if (ring.length < 3)
            return null
        var first = ring[0]
        var last = ring[ring.length - 1]
        if (first[0] !== last[0] || first[1] !== last[1])
            ring.push([first[0], first[1]])
        return {
            "type": "Polygon",
            "coordinates": [ring]
        }
    }

    function buildFeatureCollection(features) {
        return {
            "type": "FeatureCollection",
            "features": features
        }
    }

    function emptyFeatureCollection() {
        return buildFeatureCollection([])
    }

    function lineFeaturesGeoJson() {
        var items = root.lines || []
        var out = []
        for (var i = 0; i < items.length; ++i) {
            var row = items[i] || {}
            var geometry = geoLineFromPath(row.path)
            if (!geometry)
                continue
            out.push({
                "type": "Feature",
                "id": String(row.id || ("line_" + i)),
                "geometry": geometry,
                "properties": {
                    "side": String(row.side || ""),
                    "heatValue": Number(row.heatValue || 0)
                }
            })
        }
        return buildFeatureCollection(out)
    }

    function polygonFeaturesGeoJson() {
        var items = root.polygons || []
        var out = []
        for (var i = 0; i < items.length; ++i) {
            var row = items[i] || {}
            var geometry = geoPolygonFromPath(row.path)
            if (!geometry)
                continue
            out.push({
                "type": "Feature",
                "id": String(row.id || ("polygon_" + i)),
                "geometry": geometry,
                "properties": {
                    "side": String(row.side || ""),
                    "heatValue": Number(row.heatValue || 0)
                }
            })
        }
        return buildFeatureCollection(out)
    }

    function labelFeaturesGeoJson() {
        var items = root.labels || []
        var out = []
        for (var i = 0; i < items.length; ++i) {
            var row = items[i] || {}
            out.push({
                "type": "Feature",
                "id": String(row.id || ("label_" + i)),
                "geometry": geoPointFromLonLat(row.lon, row.lat),
                "properties": {
                    "name": String(row.name || "")
                }
            })
        }
        return buildFeatureCollection(out)
    }

    function heatFeaturesGeoJson() {
        var items = root.objects || []
        var out = []
        for (var i = 0; i < items.length; ++i) {
            var row = items[i] || {}
            var heatValue = Number(row.heatValue || 0)
            if (!(heatValue > 0.01))
                continue
            out.push({
                "type": "Feature",
                "id": String(row.id || ("heat_" + i)),
                "geometry": geoPointFromLonLat(row.lon, row.lat),
                "properties": {
                    "heatValue": heatValue
                }
            })
        }
        return buildFeatureCollection(out)
    }

    function objectFeaturesGeoJson() {
        var items = root.objects || []
        var out = []
        for (var i = 0; i < items.length; ++i) {
            var row = items[i] || {}
            out.push({
                "type": "Feature",
                "id": String(row.id || ("obj_" + i)),
                "geometry": geoPointFromLonLat(row.lon, row.lat),
                "properties": {
                    "id": String(row.id || ""),
                    "name": String(row.name || ""),
                    "side": String(row.side || ""),
                    "kind": String(row.kind || ""),
                    "heatValue": Number(row.heatValue || 0)
                }
            })
        }
        return buildFeatureCollection(out)
    }

    function interactionRadiusMeters() {
        if (!root.appState)
            return 1200
        var mpp = Number(root.appState.mapMetersPerPixel || 0)
        if (!(mpp > 0))
            return 1200
        return Math.max(120, Math.min(26000, mpp * 28))
    }

    function nearestInteractiveTargetAt(coord) {
        if (!coord || !coord.isValid)
            return null

        var maxDistance = interactionRadiusMeters()
        var bestDistance = maxDistance + 1
        var best = null

        var objectsSource = root.objects || []
        for (var i = 0; i < objectsSource.length; ++i) {
            var objectData = objectsSource[i] || {}
            var objectCoord = QtPositioning.coordinate(Number(objectData.lat), Number(objectData.lon))
            if (!objectCoord.isValid)
                continue
            var distance = coord.distanceTo(objectCoord)
            if (distance < bestDistance) {
                bestDistance = distance
                best = objectData
            }
        }

        var linesSource = root.lines || []
        for (var j = 0; j < linesSource.length; ++j) {
            var lineData = linesSource[j] || {}
            var lineCenter = pathCenter(lineData.path)
            if (!lineCenter.isValid)
                continue
            var lineDistance = coord.distanceTo(lineCenter)
            if (lineDistance < bestDistance) {
                bestDistance = lineDistance
                best = lineData
            }
        }

        var polygonsSource = root.polygons || []
        for (var k = 0; k < polygonsSource.length; ++k) {
            var polygonData = polygonsSource[k] || {}
            var polygonCenter = pathCenter(polygonData.path)
            if (!polygonCenter.isValid)
                continue
            var polygonDistance = coord.distanceTo(polygonCenter)
            if (polygonDistance < bestDistance) {
                bestDistance = polygonDistance
                best = polygonData
            }
        }

        if (bestDistance > maxDistance)
            return null
        return best
    }

    function gadmGeoJsonPath(level) {
        var base = String(root.gadmBasePath || "")
        if (base.length === 0)
            return ""
        base = base.replace(/\\/g, "/")
        if (base.charAt(base.length - 1) !== "/")
            base += "/"
        return base + "gadm41_RUS_" + String(level) + ".json"
    }

    function activeAdmLevelForZoom(zoomLevel) {
        var z = Number(zoomLevel)
        if (!(z >= 0))
            z = 0
        if (z < 4)
            return 0
        if (z < 6)
            return 1
        if (z < 9)
            return 2
        if (z <= 12)
            return 3
        return 3
    }

    function isAdmLevelLoaded(level) {
        if (level === 0)
            return root.gadmAdm0Loaded
        if (level === 1)
            return root.gadmAdm1Loaded
        if (level === 2)
            return root.gadmAdm2Loaded
        if (level === 3)
            return root.gadmAdm3Loaded
        return false
    }

    function setAdmLevelLoaded(level, loaded) {
        if (level === 0)
            root.gadmAdm0Loaded = loaded
        else if (level === 1)
            root.gadmAdm1Loaded = loaded
        else if (level === 2)
            root.gadmAdm2Loaded = loaded
        else if (level === 3)
            root.gadmAdm3Loaded = loaded
    }

    function setAdmLevelData(level, collection) {
        var data = collection
        if (!data || data.type !== "FeatureCollection")
            data = root.emptyFeatureCollection()
        if (level === 0)
            root.gadmAdm0Data = data
        else if (level === 1)
            root.gadmAdm1Data = data
        else if (level === 2)
            root.gadmAdm2Data = data
        else if (level === 3)
            root.gadmAdm3Data = data
    }

    function ensureAdmLevelLoaded(level) {
        if (level < 0 || level > 3)
            return
        if (isAdmLevelLoaded(level))
            return
        var path = gadmGeoJsonPath(level)
        if (path.length === 0) {
            setAdmLevelLoaded(level, true)
            return
        }
        var loaded = LocalGeoJsonRepo.loadGeoJson(path)
        setAdmLevelData(level, loaded)
        setAdmLevelLoaded(level, true)
    }

    function ensureAdmForCurrentZoom() {
        var zoom = mapView && mapView.map ? mapView.map.zoomLevel : (root.appState ? root.appState.zoomLevel : 5)
        ensureAdmLevelLoaded(activeAdmLevelForZoom(zoom))
    }

    Timer {
        id: viewportUpdateTimer
        interval: 85
        repeat: false
        onTriggered: {
            root.updateScaleMetrics()
            root.updateViewportBounds()
        }
    }

    Timer {
        id: cursorUpdateTimer
        interval: 40
        repeat: false
        onTriggered: {
            if (!root.appState)
                return
            var coord = mapView.map.toCoordinate(root.pendingCursorPoint, false)
            root.appState.cursorLon = coord.longitude
            root.appState.cursorLat = coord.latitude
        }
    }

    Timer {
        id: interactionIdleTimer
        interval: 170
        repeat: false
        onTriggered: root.interactionInProgress = false
    }

    Timer {
        id: deferredGeoLoadTimer
        interval: 220
        repeat: false
        onTriggered: {
            root.ensureAdmForCurrentZoom()
        }
    }

    // Список стилей tileserver-gl, доступных переключателю.
    readonly property var availableMapStyles: {
        var host = (root.appState && root.appState.tileServerHost)
            ? root.appState.tileServerHost
            : "http://localhost:8080"
        return [
            { "name": "maptiler-basic", "title": "MapTiler Basic",
              "url": host + "/styles/maptiler-basic/style.json" },
            { "name": "osm-bright",     "title": "OSM Bright",
              "url": host + "/styles/osm-bright/style.json" }
        ]
    }

    Plugin {
        id: mapPlugin
        name: "maplibre"

        PluginParameter {
            name: "maplibre.map.styles"
            value: {
                var urls = []
                for (var i = 0; i < root.availableMapStyles.length; ++i)
                    urls.push(root.availableMapStyles[i].url)
                return urls.join(",")
            }
        }
    }

    MapView {
        id: mapView
        anchors.fill: parent

        map.plugin: mapPlugin
        map.zoomLevel: root.appState ? root.appState.zoomLevel : 5
        map.center: QtPositioning.coordinate(
                        root.appState ? root.appState.centerLat : 55.7558,
                        root.appState ? root.appState.centerLon : 37.6176)
        onWidthChanged: {
            root.scheduleViewportUpdate(true)
        }
        onHeightChanged: {
            root.scheduleViewportUpdate(true)
        }
        function applyActiveMapStyle() {
            if (!mapView.map.supportedMapTypes || mapView.map.supportedMapTypes.length === 0)
                return
            var wantedName = root.appState ? String(root.appState.mapStyleName || "") : ""
            var wantedUrl = ""
            for (var i = 0; i < root.availableMapStyles.length; ++i) {
                if (root.availableMapStyles[i].name === wantedName) {
                    wantedUrl = root.availableMapStyles[i].url
                    break
                }
            }
            var selectedType = null
            for (var j = 0; j < mapView.map.supportedMapTypes.length; ++j) {
                var mapType = mapView.map.supportedMapTypes[j]
                var metadata = mapType && mapType.metadata ? mapType.metadata : ({})
                var styleUrl = String(metadata.url || "")
                if (wantedUrl.length > 0 && styleUrl === wantedUrl) {
                    selectedType = mapType
                    break
                }
            }
            if (!selectedType)
                selectedType = mapView.map.supportedMapTypes[0]
            if (selectedType && mapView.map.activeMapType !== selectedType)
                mapView.map.activeMapType = selectedType
        }

        Component.onCompleted: {
            mapView.applyActiveMapStyle()
            deferredGeoLoadTimer.start()
            root.scheduleViewportUpdate(true)
        }

        Connections {
            target: root.appState
            ignoreUnknownSignals: true
            function onMapStyleNameChanged() { mapView.applyActiveMapStyle() }
            function onTileServerHostChanged() { mapView.applyActiveMapStyle() }
        }

        MapLibre.style: Style {
            SourceParameter {
                styleId: "saturn-runtime-lines"
                type: "geojson"
                property var data: root.lineFeaturesGeoJson()
            }

            SourceParameter {
                styleId: "saturn-runtime-polygons"
                type: "geojson"
                property var data: root.polygonFeaturesGeoJson()
            }

            SourceParameter {
                styleId: "saturn-runtime-labels"
                type: "geojson"
                property var data: root.labelFeaturesGeoJson()
            }

            SourceParameter {
                styleId: "saturn-runtime-heat"
                type: "geojson"
                property var data: root.heatFeaturesGeoJson()
            }

            SourceParameter {
                styleId: "saturn-runtime-objects"
                type: "geojson"
                property var data: root.objectFeaturesGeoJson()
            }

            SourceParameter {
                styleId: "saturn-rus-adm0"
                type: "geojson"
                property var data: root.gadmAdm0Data
            }

            SourceParameter {
                styleId: "saturn-rus-adm1"
                type: "geojson"
                property var data: root.gadmAdm1Data
            }

            SourceParameter {
                styleId: "saturn-rus-adm2"
                type: "geojson"
                property var data: root.gadmAdm2Data
            }

            SourceParameter {
                styleId: "saturn-rus-adm3"
                type: "geojson"
                property var data: root.gadmAdm3Data
            }

            LayerParameter {
                styleId: "saturn-rus-adm0-lines"
                type: "line"
                property string source: "saturn-rus-adm0"
                paint: {
                    "line-color": "#8e99a4",
                    "line-width": ["interpolate", ["linear"], ["zoom"], 0, 1.2, 4, 1.8],
                    "line-opacity": ["step", ["zoom"], 0.72, 4, 0.0]
                }
            }

            LayerParameter {
                styleId: "saturn-rus-adm1-lines"
                type: "line"
                property string source: "saturn-rus-adm1"
                paint: {
                    "line-color": "#8a95a0",
                    "line-width": ["interpolate", ["linear"], ["zoom"], 4, 0.9, 6, 1.4],
                    "line-opacity": ["step", ["zoom"], 0.0, 4, 0.66, 6, 0.0]
                }
            }

            LayerParameter {
                styleId: "saturn-rus-adm2-lines"
                type: "line"
                property string source: "saturn-rus-adm2"
                paint: {
                    "line-color": "#87939e",
                    "line-width": ["interpolate", ["linear"], ["zoom"], 6, 0.8, 9, 1.2],
                    "line-opacity": ["step", ["zoom"], 0.0, 6, 0.62, 9, 0.0]
                }
            }

            LayerParameter {
                styleId: "saturn-rus-adm3-lines"
                type: "line"
                property string source: "saturn-rus-adm3"
                paint: {
                    "line-color": "#7f8a95",
                    "line-width": ["interpolate", ["linear"], ["zoom"], 9, 0.7, 12, 1.1],
                    "line-opacity": ["step", ["zoom"], 0.0, 9, 0.58, 12, 0.0]
                }
            }

            LayerParameter {
                styleId: "saturn-runtime-polygons-fill"
                type: "fill"
                property string source: "saturn-runtime-polygons"
                paint: {
                    "fill-color": root.appState && root.appState.mapMode === "heatmap"
                        ? ["interpolate", ["linear"], ["coalesce", ["get", "heatValue"], 0], 0, "#4799d6", 0.5, "#dcb840", 1, "#ca4839"]
                        : ["match", ["get", "side"], "friendly", "#78a3ff", "foreign", "#dbe6f2", "#9dabb8"],
                    "fill-opacity": root.appState && root.appState.mapMode === "heatmap" ? 0.28 : 0.14
                }
            }

            LayerParameter {
                styleId: "saturn-runtime-polygons-line"
                type: "line"
                property string source: "saturn-runtime-polygons"
                paint: {
                    "line-color": root.appState && root.appState.mapMode === "heatmap"
                        ? ["interpolate", ["linear"], ["coalesce", ["get", "heatValue"], 0], 0, "#63b5e8", 0.5, "#e6c557", 1, "#d85649"]
                        : ["match", ["get", "side"], "friendly", "#78a3ff", "foreign", "#dbe6f2", "#9dabb8"],
                    "line-width": 1.6
                }
            }

            LayerParameter {
                styleId: "saturn-runtime-lines-layer"
                type: "line"
                property string source: "saturn-runtime-lines"
                paint: {
                    "line-color": root.appState && root.appState.mapMode === "heatmap"
                        ? ["interpolate", ["linear"], ["coalesce", ["get", "heatValue"], 0], 0, "#63b5e8", 0.5, "#e6c557", 1, "#d85649"]
                        : ["match", ["get", "side"], "friendly", "#78a3ff", "foreign", "#dbe6f2", "#9dabb8"],
                    "line-width": 2.0
                }
            }

            LayerParameter {
                styleId: "saturn-runtime-heat-circles"
                type: "circle"
                property string source: "saturn-runtime-heat"
                paint: {
                    "circle-radius": ["interpolate", ["linear"], ["coalesce", ["get", "heatValue"], 0], 0.01, 6, 1.0, 28],
                    "circle-color": ["interpolate", ["linear"], ["coalesce", ["get", "heatValue"], 0], 0.0, "#5ca6d7", 0.5, "#dfb641", 1.0, "#cc4c3b"],
                    "circle-opacity": root.appState
                        && root.appState.mapMode === "heatmap"
                        && !root.interactionInProgress ? 0.22 : 0.0,
                    "circle-stroke-width": 0
                }
            }

            LayerParameter {
                styleId: "saturn-runtime-objects-circles"
                type: "circle"
                property string source: "saturn-runtime-objects"
                paint: {
                    "circle-radius": ["interpolate", ["linear"], ["zoom"], 3, 4, 8, 7, 12, 11],
                    "circle-color": ["match", ["get", "side"], "friendly", "#7298f6", "foreign", "#d7dee7", "#909ba7"],
                    "circle-stroke-color": "#f2f7fc",
                    "circle-stroke-width": ["interpolate", ["linear"], ["zoom"], 3, 0.8, 10, 1.6],
                    "circle-opacity": root.appState && root.appState.mapMode === "heatmap" ? 0.46 : 0.96
                }
            }

            LayerParameter {
                styleId: "saturn-runtime-labels-layer"
                type: "symbol"
                property string source: "saturn-runtime-labels"
                layout: {
                    "text-field": ["get", "name"],
                    "text-size": 13,
                    "text-offset": [0, 0.9],
                    "text-anchor": "top"
                }
                paint: {
                    "text-color": "#808995",
                    "text-halo-color": "#0b0f14",
                    "text-halo-width": 0.3,
                    "text-opacity": (root.appState ? root.appState.showLabels : true)
                        && mapView.map.zoomLevel >= root.labelMinZoom
                        && !root.interactionInProgress ? 0.78 : 0.0
                }
            }
        }

        Connections {
            target: mapView.map

            function onCenterChanged() {
                if (!root.appState || !mapView.map.center.isValid)
                    return
                root.appState.centerLat = mapView.map.center.latitude
                root.appState.centerLon = mapView.map.center.longitude
                root.scheduleViewportUpdate(false)
            }

            function onZoomLevelChanged() {
                root.ensureAdmForCurrentZoom()
                if (!root.appState)
                    return
                root.appState.zoomLevel = mapView.map.zoomLevel
                root.scheduleViewportUpdate(false)
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

                root.markInteraction()
                var zoomStep = rawDelta > 0 ? 0.6 : -0.6
                mapView.map.zoomLevel = root.clampZoom(mapView.map.zoomLevel + zoomStep)
                event.accepted = true
            }
        }
    }

    MapPolyline {
        visible: root.editActive && root.editGeometryType === "LineString" && (root.editPoints || []).length >= 2
        parent: mapView.map
        path: root.editPathCoordinates()
        line.width: 3
        line.color: "#60a5fa"
        smooth: !root.interactionInProgress
        antialiasing: !root.interactionInProgress
    }

    MapPolygon {
        visible: root.editActive && root.editGeometryType === "Polygon" && (root.editPoints || []).length >= 3
        parent: mapView.map
        path: root.editPathCoordinates()
        color: Qt.rgba(0.38, 0.64, 1.0, 0.16)
        border.width: 2
        border.color: "#60a5fa"
        smooth: !root.interactionInProgress
        antialiasing: !root.interactionInProgress
    }

    MapItemView {
        parent: mapView.map
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
                        var p = vertexHit.mapToItem(mapView.map, mouse.x, mouse.y)
                        var c = mapView.map.toCoordinate(Qt.point(p.x, p.y), false)
                        root.editPointMoved(editVertex.index, c.longitude, c.latitude)
                    }

                    onReleased: dragging = false
                    onCanceled: dragging = false
                }
            }
        }
    }

    MapItemView {
        parent: mapView.map
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

    MouseArea {
        id: mapHoverArea
        anchors.fill: parent
        acceptedButtons: Qt.NoButton
        hoverEnabled: true
        propagateComposedEvents: true

        onPositionChanged: function(mouse) {
            if (!root.appState || root.interactionInProgress)
                return
            root.pendingCursorPoint = Qt.point(mouse.x, mouse.y)
            cursorUpdateTimer.restart()
        }

    }

    TapHandler {
        id: mapTapHandler
        target: null
        acceptedDevices: PointerDevice.Mouse
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        grabPermissions: PointerHandler.CanTakeOverFromAnything

        onTapped: function(point, button) {
            root.markInteraction()
            var coord = mapView.map.toCoordinate(point.position, false)
            var hitObject = root.nearestInteractiveTargetAt(coord)
            if (button === Qt.RightButton) {
                if (hitObject) {
                    root.requestObjectMenu(hitObject, point.position.x + 16, point.position.y + 12)
                } else {
                    root.requestMapMenu(coord.longitude, coord.latitude, point.position.x, point.position.y)
                }
                return
            }

            if (root.editActive) {
                root.editPointAppended(coord.longitude, coord.latitude)
                return
            }
            if (hitObject)
                root.requestSelectObject(hitObject)
            else
                root.requestClearSelection()
        }
    }
}




