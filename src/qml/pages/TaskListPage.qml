import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../components"
import "../dialogs"
import "../styles"

Page {
    id: root

    AppTheme {
        id: theme
    }

    AddTaskDialog {
        id: addTaskDialog
        anchors.centerIn: parent
    }

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            anchors.margins: theme.spacing

            Label {
                text: qsTr("Kinuys Task Manager")
                font.pixelSize: theme.fontSizeXLarge
                font.bold: true
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Total: %1 | Completed: %2 | Pending: %3")
                     .arg(taskController.totalTasks)
                     .arg(taskController.completedTasks)
                     .arg(taskController.pendingTasks)
                font.pixelSize: theme.fontSizeMedium
                color: theme.textSecondary
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: theme.spacingMedium
        spacing: theme.spacingMedium

        RowLayout {
            Layout.fillWidth: true
            spacing: theme.spacing

            CustomButton {
                text: qsTr("Add Task")
                onClicked: addTaskDialog.open()
                primary: true
            }

            CustomButton {
                text: qsTr("Clear Completed")
                enabled: taskController.completedTasks > 0
                onClicked: taskController.clearCompletedTasks()
                // Optional: Add visual feedback for disabled state
                opacity: enabled ? 1.0 : 0.6
            }

            Item {
                Layout.fillWidth: true
            }

            CustomButton {
                text: qsTr("Load Sample Data")
                onClicked: taskController.loadSampleData()
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: listView
                model: taskController.taskModel
                spacing: theme.spacing

                delegate: TaskItem {
                    width: listView.width
                    task: model.taskObject

                    onToggleCompleted: {
                        taskController.toggleTask(index)
                    }

                    onDeleteRequested: {
                        taskController.deleteTask(index)
                    }
                }

                section.property: "completed"
                section.criteria: ViewSection.FullString
                section.delegate: Rectangle {
                    width: listView.width
                    height: sectionLabel.height + theme.spacingMedium
                    color: "transparent"

                    Label {
                        id: sectionLabel
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        text: section === "true" ? qsTr("Completed Tasks") : qsTr("Pending Tasks")
                        font.pixelSize: theme.fontSizeLarge
                        font.bold: true
                        color: theme.textPrimary
                    }
                }

                // Empty state
                Label {
                    anchors.centerIn: parent
                    visible: listView.count === 0
                    text: qsTr("No tasks yet.\nClick 'Add Task' to create your first task.")
                    font.pixelSize: theme.fontSizeMedium
                    color: theme.textSecondary
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }
    }
}
