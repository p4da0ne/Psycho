import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtLocation
import QtPositioning
import MapLibre 3.0

Item {
    id: root
    width: 800
    height: 600
    visible: true

    Plugin {
        id: mapPlugin
        name: "maplibre"

        PluginParameter {
            name: "maplibre.map.styles"
            value: "http://localhost:8080/styles/maptiler-basic/style.json"
        }
    }

    MapView {
        id: mapview
        anchors.fill: parent

        map.plugin: mapPlugin
        map.zoomLevel: 5
    }
}
