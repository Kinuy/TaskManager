import QtQuick 2.15
import QtQuick.Controls 2.15
import "../styles"

Button {
    id: root

    property bool primary: false

    AppTheme {
        id: theme
    }

    font.pixelSize: theme.fontSizeMedium
    implicitHeight: 36

    background: Rectangle {
        color: {
            if (!root.enabled) {
                return theme.textDisabled
            }
            if (root.pressed) {
                return root.primary ? theme.primaryDark : Qt.darker(theme.surfaceColor, 1.1)
            }
            if (root.hovered) {
                return root.primary ? theme.primaryLight : Qt.lighter(theme.surfaceColor, 1.1)
            }
            return root.primary ? theme.primaryColor : theme.surfaceColor
        }

        border.color: root.primary ? "transparent" : theme.textDisabled
        border.width: root.primary ? 0 : 1
        radius: theme.borderRadius

        Behavior on color {
            ColorAnimation {
                duration: 150
            }
        }
    }

    contentItem: Text {
        text: root.text
        font: root.font
        color: {
            if (!root.enabled) {
                return theme.textDisabled
            }
            return root.primary ? "white" : theme.textPrimary
        }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
