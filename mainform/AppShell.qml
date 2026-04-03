import QtQuick 2.15
import QtQuick.Window 2.15
import Saturn.Backend 1.0
import "components"

Window {
    id: root

    visible: true
    width: 1540
    height: 920
    minimumWidth: 1180
    minimumHeight: 760
    title: "Saturn GIS"
    color: "#0B0F14"

    property string panel: "none"

    // TEMPORARY(TEST): fallback labels before dedicated reference backend adapter.
    property var referenceTreeData: [
        {
            "id": "countries",
            "label": "Countries",
            "children": [
                {
                    "id": "russia",
                    "label": "Russia",
                    "children": [
                        { "id": "moscow", "label": "Moscow" }
                    ]
                }
            ]
        }
    ]

    property var mapObjects: []
    property var locationLabels: []

    QtObject {
        id: appState

        property real centerLon: 37.6176
        property real centerLat: 55.7558
        property real cursorLon: 37.6176
        property real cursorLat: 55.7558
        property real zoomLevel: 11.8
        property string mapMode: "point"
        property string activeTool: "navigate"
        property string searchText: ""
        property bool showFriendly: true
        property bool showForeign: true
        property bool showBaseStations: true
        property bool showHeatmapLayer: true
        property bool showCoverageLine: true
        property bool showLabels: true
        property bool controlsVisible: true
        property var selectedObject: null
        property var hoveredObject: null
        property string selectionType: "none"
        property var selectedObjects: []
        property real lastContextLon: centerLon
        property real lastContextLat: centerLat
        property string statusMessage: "Готово."

        function selectObject(objectData) {
            selectedObject = objectData
            hoveredObject = objectData
            selectionType = objectData ? "object" : "none"
            selectedObjects = objectData ? [objectData] : []
            statusMessage = objectData ? "Выбран объект: " + objectData.name : "Выделение снято"
        }

        function clearSelection() {
            selectedObject = null
            hoveredObject = null
            selectionType = "none"
            selectedObjects = []
            statusMessage = "Выделение снято"
        }
    }

    function sideByType(objectType) {
        if (objectType === 3 || objectType === 4 || objectType === 5 || objectType === 6 || objectType === 9)
            return "infrastructure"
        if (objectType === 8)
            return "foreign"
        return "friendly"
    }

    function kindByType(objectType) {
        if (objectType === 6)
            return "region"
        if (objectType === 7)
            return "personnel"
        if (objectType === 8)
            return "event"
        if (objectType === 3 || objectType === 4 || objectType === 5)
            return "lbs"
        return "unit"
    }

    function refreshObjectsFromRuntime() {
        var pointsJson = MapRuntime.pointsSource
        if (!pointsJson || pointsJson.length === 0)
            return

        var parsed = JSON.parse(pointsJson)
        var features = parsed.features || []
        var objects = []
        var labels = []

        for (var i = 0; i < features.length; ++i) {
            var feature = features[i]
            if (!feature || !feature.geometry || feature.geometry.type !== "Point")
                continue

            var coords = feature.geometry.coordinates || []
            if (coords.length < 2)
                continue

            var props = feature.properties || {}
            var objectType = Number(props.objectType || 0)
            var objectId = Number(props.objectId || 0)

            var row = {
                "id": objectType + "-" + objectId,
                "objectType": objectType,
                "objectId": objectId,
                "name": props.title || ("Object " + objectId),
                "side": sideByType(objectType),
                "kind": kindByType(objectType),
                "lat": Number(coords[1]),
                "lon": Number(coords[0]),
                "mpps": 50,
                "speed": 0,
                "course": 0,
                "source": "backend",
                "notes": props.subtitle || ""
            }
            objects.push(row)

            if (labels.length < 24) {
                labels.push({ "name": row.name, "lat": row.lat, "lon": row.lon })
            }
        }

        mapObjects = objects
        locationLabels = labels
        appState.statusMessage = "Объектов на карте: " + objects.length
    }

    function objectVisible(objectData) {
        if (!objectData)
            return false

        if (appState.searchText !== "") {
            var needle = appState.searchText.toLowerCase()
            var haystack = (objectData.name + " " + objectData.id + " " + objectData.kind).toLowerCase()
            if (haystack.indexOf(needle) === -1)
                return false
        }

        if (objectData.side === "friendly" && !appState.showFriendly)
            return false
        if (objectData.side === "foreign" && !appState.showForeign)
            return false
        if (objectData.side === "infrastructure" && !appState.showBaseStations)
            return false

        return true
    }

    function visibleObjectCount() {
        var count = 0
        for (var i = 0; i < mapObjects.length; ++i) {
            if (objectVisible(mapObjects[i]))
                count += 1
        }
        return count
    }

    function averageMpps() {
        var total = 0
        var count = 0
        for (var i = 0; i < mapObjects.length; ++i) {
            if (!objectVisible(mapObjects[i]))
                continue
            total += mapObjects[i].mpps
            count += 1
        }
        return count === 0 ? 0 : Math.round(total / count)
    }

    Component.onCompleted: {
        Polling.intervalMs = 30000
        Polling.start()
        MapSnapshot.refreshAll()
        MapRuntime.refreshNow()
        refreshObjectsFromRuntime()
    }

    Connections {
        target: MapRuntime
        function onSourcesUpdated() {
            refreshObjectsFromRuntime()
        }
    }

    Timer {
        id: controlsReturnTimer
        interval: 1100
        repeat: false
        onTriggered: appState.controlsVisible = true
    }

    Item {
        anchors.fill: parent

        MapCanvas {
            id: mapCanvas
            anchors.fill: parent
            appState: appState
            objects: root.mapObjects
            labels: root.locationLabels
            onInteractionActivity: {
                appState.controlsVisible = false
                controlsReturnTimer.restart()
            }
        }

        Toolbar {
            id: topBar
            z: 40
            width: Math.min(parent.width - 32, 620)
            anchors.top: parent.top
            anchors.topMargin: 18
            anchors.horizontalCenter: parent.horizontalCenter
            appState: appState
            backdropSource: mapCanvas.sceneContent
        }

        RightReferencePanel {
            id: referencePanel
            anchors.fill: parent
            z: 45
            topInset: topBar.y + topBar.height + 8
            backdropSource: mapCanvas.sceneContent
            treeData: root.referenceTreeData
        }

        BottomSheet {
            id: bottomSheet
            anchors.fill: parent
            anchors.margins: 16
            z: 35
            appState: appState
            objects: root.mapObjects
            visibleCount: root.visibleObjectCount()
            averageMppsValue: root.averageMpps()
            topInset: topBar.y + topBar.height + 8
            rightReservedWidth: referencePanel.reservedWidth
            backdropSource: mapCanvas.sceneContent
        }

        GlassPanel {
            id: mainMenu
            z: 60
            width: 272
            height: 170
            anchors.left: parent.left
            anchors.leftMargin: 18
            anchors.top: parent.top
            anchors.topMargin: 18
            backdropSource: mapCanvas.sceneContent
            surfaceColor: "#121a23"
            surfaceOpacity: 0.56

            Column {
                anchors.fill: parent
                spacing: 10

                Text {
                    text: "Главное меню"
                    color: "#f3f7fb"
                    font.pixelSize: 14
                    font.weight: Font.Medium
                }

                Rectangle {
                    width: parent.width
                    height: 44
                    radius: 12
                    color: "#1e2937"
                    border.color: "#334155"

                    Text {
                        anchors.centerIn: parent
                        text: "Авторизация"
                        color: "#f8fafc"
                        font.pixelSize: 14
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: root.panel = "auth"
                    }
                }

                Rectangle {
                    width: parent.width
                    height: 44
                    radius: 12
                    color: "#1e2937"
                    border.color: "#334155"

                    Text {
                        anchors.centerIn: parent
                        text: "Настройки подключения к БД"
                        color: "#f8fafc"
                        font.pixelSize: 14
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: root.panel = "db"
                    }
                }
            }
        }

        Item {
            id: panelHost
            z: 65
            anchors.right: parent.right
            anchors.rightMargin: 18
            anchors.top: parent.top
            anchors.topMargin: 18
            width: Math.min(parent.width * 0.42, 580)
            height: Math.min(parent.height * 0.48, 420)
            visible: root.panel !== "none"

            GlassPanel {
                anchors.fill: parent
                backdropSource: mapCanvas.sceneContent
                surfaceColor: "#10161e"
                surfaceOpacity: 0.62

                Loader {
                    id: panelLoader
                    anchors.fill: parent
                    sourceComponent: root.panel === "auth" ? authPanel : dbPanel
                }
            }

            Rectangle {
                width: 28
                height: 28
                radius: 14
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: 8
                color: "#1f2937"
                border.color: "#334155"

                Text {
                    anchors.centerIn: parent
                    text: "×"
                    color: "#e2e8f0"
                    font.pixelSize: 14
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: root.panel = "none"
                }
            }
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

    Shortcut {
        sequence: "Ctrl+1"
        onActivated: appState.mapMode = "point"
    }

    Shortcut {
        sequence: "Ctrl+2"
        onActivated: appState.mapMode = "heatmap"
    }

    Shortcut {
        sequence: "Escape"
        onActivated: {
            if (root.panel !== "none")
                root.panel = "none"
            else
                appState.clearSelection()
        }
    }
}
