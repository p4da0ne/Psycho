import QtQML
import QtQuick
import QtLocation
import QtPositioning
import MapLibre

Item {
    id: root
    width: 640
    height: 480
    visible: true

    Plugin {
        id: mapPlugin
        name: "maplibre"
        // specify plugin parameters if necessary
        PluginParameter {
            name: "maplibre.map.styles"
            value: "https://demotiles.maplibre.org/style.json"
        }
    }

    MapView {
        id: mapView
        anchors.fill: parent
        anchors.topMargin: fullWindow ? 0 : Math.round(parent.height / 6)
        anchors.leftMargin: fullWindow ? 0 : Math.round(parent.width / 6)
        map.plugin: mapPlugin
        map.center: window.coordinate
        map.zoomLevel: 5
        map.activeMapType: Street

        MapLibre.style: Style {
            id: style

            SourceParameter {
                id: radarSourceParam
                styleId: "radar"
                type: "image"
                property string url: "https://maplibre.org/maplibre-gl-js/docs/assets/radar1.gif"
                property var coordinates: [
                    [-80.425, 46.437],
                    [-71.516, 46.437],
                    [-71.516, 37.936],
                    [-80.425, 37.936]
                ]
            }

            LayerParameter {
                id: radarLayerParam
                styleId: "radar-layer"
                type: "raster"
                property string source: "radar"

                paint: {
                    "raster-opacity": 0.9
                }
            }
        }
    }
}
