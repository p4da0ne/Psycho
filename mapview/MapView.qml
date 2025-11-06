import QtQml
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls.Basic 2.15
import QtLocation
import QtPositioning
import MapLibre

Item {
    id: root
    width: 640
    height: 480
    visible: true
    MapView {
        id: mapview
    }
}
