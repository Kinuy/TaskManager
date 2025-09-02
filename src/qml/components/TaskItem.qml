import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../styles"
import TaskManager 1.0

Rectangle {
    id: root

    property Task task: null

    signal toggleCompleted()
    signal deleteRequested()

    height: contentLayout.height + theme.spacingMedium * 2
    color: theme.cardColor
    border.color: theme.textDisabled
    border.width: 1
    radius: theme.borderRadius

    AppTheme {
        id: theme
    }

    states: [
        State {
            name: "completed"
            when: task && task.completed
            PropertyChanges {
                target: root
                opacity: 0.6
            }
            PropertyChanges {
                target: titleLabel
                color: theme.textSecondary
                font.strikeout: true
            }
            PropertyChanges {
                target: descriptionLabel
                color: theme.textDisabled
                font.strikeout: true
            }
        }
    ]

    RowLayout {
        id: contentLayout
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: theme.spacingMedium
        spacing: theme.spacingMedium

        CheckBox {
            id: checkBox
            checked: task ? task.completed : false
            onToggled: root.toggleCompleted()
        }

        Rectangle {
            width: 4
            height: parent.height
            color: task ? theme.priorityColor(task.priority) : theme.textDisabled
            radius: 2
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: theme.spacing / 2

            Label {
                id: titleLabel
                text: task ? task.title : ""
                font.pixelSize: theme.fontSizeMedium
                font.bold: true
                color: theme.textPrimary
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
            }

            Label {
                id: descriptionLabel
                text: task ? task.description : ""
                font.pixelSize: theme.fontSizeSmall
                color: theme.textSecondary
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
                visible: text.length > 0
            }

            RowLayout {
                spacing: theme.spacing

                Label {
                    text: task ? qsTr("Priority: %1").arg(task.priorityString()) : ""
                    font.pixelSize: theme.fontSizeSmall
                    color: task ? theme.priorityColor(task.priority) : theme.textDisabled
                }

                Label {
                    text: task ? Qt.formatDateTime(task.dateTime, "MMM dd, yyyy") : ""
                    font.pixelSize: theme.fontSizeSmall
                    color: theme.textDisabled
                }
            }
        }

        Button {
            text: qsTr("Delete")
            font.pixelSize: theme.fontSizeSmall
            onClicked: root.deleteRequested()

            background: Rectangle {
                color: parent.pressed ? theme.error : "transparent"
                border.color: theme.error
                border.width: 1
                radius: theme.borderRadius
            }

            contentItem: Text {
                text: parent.text
                color: parent.pressed ? "white" : theme.error
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    // Subtle shadow
    Rectangle {
        anchors.fill: parent
        color: "transparent"
        border.color: Qt.rgba(0, 0, 0, 0.1)
        border.width: 1
        radius: theme.borderRadius
        z: -1
        anchors.topMargin: 1
        anchors.leftMargin: 1
    }
}
