pragma Singleton
import QtQuick 2.15

// Единая палитра приложения. Все цвета UI берутся отсюда.
// ink(a) — белая накладка с прозрачностью a (заменяет Qt.rgba(1,1,1,a)).
QtObject {
    function ink(a) { return Qt.rgba(1, 1, 1, a) }

    readonly property color shadow: "#000000"
    readonly property color bg: "#0b0f14"
    readonly property color bgDeep: "#040a15"
    readonly property color bgDeep2: "#050b17"
    readonly property color surfaceSlate: "#0f172a"
    readonly property color surface1: "#10161d"
    readonly property color surface1b: "#10161e"
    readonly property color surface2: "#111821"
    readonly property color surface3: "#121a23"
    readonly property color surface: "#141c24"
    readonly property color surface4: "#1a242f"
    readonly property color surface5: "#1e2937"
    readonly property color panelSlate: "#1e293b"
    readonly property color surface6: "#1f2937"
    readonly property color navy: "#132746"
    readonly property color navyMid: "#24497a"
    readonly property color slate: "#334155"
    readonly property color slateMid: "#475569"
    readonly property color slateSoft: "#64748b"
    readonly property color accentDeep: "#1d4ed8"
    readonly property color accentStrong: "#2563eb"
    readonly property color accent: "#3b82f6"
    readonly property color friendly: "#7298f6"
    readonly property color friendlyLight: "#8db7ff"
    readonly property color white: "#ffffff"
    readonly property color textBright: "#f8fafc"
    readonly property color textBright2: "#f3f7fb"
    readonly property color textLight: "#e7eef7"
    readonly property color foreignLight: "#eef4fb"
    readonly property color textLight2: "#eaf2fb"
    readonly property color textLight3: "#e2e8f0"
    readonly property color textSoft: "#d9e2ed"
    readonly property color textSoft2: "#d7e0ea"
    readonly property color textSoft3: "#dce4ed"
    readonly property color foreign: "#d7dee7"
    readonly property color textMuted: "#c8d4e2"
    readonly property color muted1: "#bac6d2"
    readonly property color muted2: "#acb6c2"
    readonly property color muted3: "#9fb0c3"
    readonly property color muted4: "#94a3b8"
    readonly property color muted5: "#91a0b0"
    readonly property color muted6: "#909ba7"
    readonly property color muted7: "#9ba8b5"
    readonly property color muted8: "#8b98a6"
    readonly property color successLight: "#b8f5ca"
    readonly property color success: "#8be9a8"
    readonly property color danger: "#fca5a5"
    readonly property color dangerLight: "#fecaca"

    readonly property color accentVivid: Qt.rgba(0.50, 0.72, 1.0, 1.0)
    readonly property color slateRgb: Qt.rgba(0.56, 0.64, 0.73, 1.0)
    readonly property color muteRgb: Qt.rgba(0.58, 0.63, 0.68, 1.0)
    readonly property color friendlyRgb: Qt.rgba(0.58, 0.66, 1.0, 1.0)
    readonly property color structureDot: Qt.rgba(0.68, 0.76, 0.90, 0.90)
    readonly property color dangerRgb: Qt.rgba(0.71, 0.56, 0.58, 1.0)
}
