pragma Singleton
import QtQuick

QtObject {
    // Colors
    readonly property color background: "#1a1a2e"
    readonly property color surface: "#16213e"
    readonly property color surfaceLight: "#1f3460"
    readonly property color accent: "#0f969c"
    readonly property color accentLight: "#6dcbcf"
    readonly property color textPrimary: "#ffffff"
    readonly property color textSecondary: "#a0a0a0"
    readonly property color divider: "#2a2a4a"

    // Typography
    readonly property int fontSizeSmall: 11
    readonly property int fontSizeMedium: 14
    readonly property int fontSizeLarge: 18
    readonly property int fontSizeTitle: 28
    readonly property int fontSizeHeader: 36

    // Spacing
    readonly property int spacingSmall: 4
    readonly property int spacingMedium: 8
    readonly property int spacingLarge: 16
    readonly property int spacingXLarge: 24

    // Dimensions
    readonly property int episodeItemHeight: 80
    readonly property int characterCardWidth: 140
    readonly property int characterCardHeight: 170
    readonly property int avatarSize: 100
    readonly property int thumbnailWidth: 100
    readonly property int thumbnailHeight: 56
    readonly property int borderRadius: 8
    readonly property int borderRadiusSmall: 4

    // Panel widths
    readonly property int episodePanelWidth: 320
}
