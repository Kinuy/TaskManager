import QtQuick 2.15

QtObject {
    readonly property color primaryColor: "#2196F3"
    readonly property color primaryDark: "#1976D2"
    readonly property color primaryLight: "#BBDEFB"

    readonly property color backgroundColor: "#FAFAFA"
    readonly property color surfaceColor: "#FFFFFF"
    readonly property color cardColor: "#FFFFFF"

    readonly property color textPrimary: "#212121"
    readonly property color textSecondary: "#757575"
    readonly property color textDisabled: "#BDBDBD"

    readonly property color success: "#4CAF50"
    readonly property color warning: "#FF9800"
    readonly property color error: "#F44336"

    // Priority colors
    readonly property color priorityLow: "#4CAF50"
    readonly property color priorityMedium: "#FF9800"
    readonly property color priorityHigh: "#F44336"

    // Spacing
    readonly property int spacing: 8
    readonly property int spacingMedium: 16
    readonly property int spacingLarge: 24

    // Border radius
    readonly property int borderRadius: 4
    readonly property int borderRadiusLarge: 8

    // Font sizes
    readonly property int fontSizeSmall: 12
    readonly property int fontSizeMedium: 14
    readonly property int fontSizeLarge: 16
    readonly property int fontSizeXLarge: 20

    function priorityColor(priority) {
        switch(priority) {
        case 0: return priorityLow
        case 1: return priorityMedium
        case 2: return priorityHigh
        default: return textSecondary
        }
    }
}
