import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../components"
import "../styles"
import TaskManager 1.0

Dialog {
    id: root

    title: qsTr("Add New Task")
    modal: true

    width: 400
    height: 400

    AppTheme {
        id: theme
    }

    onOpened: {
        titleField.text = ""
        descriptionField.text = ""
        priorityComboBox.currentIndex = 1  // Medium priority default
        titleField.forceActiveFocus()
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: theme.spacingMedium

        Label {
            text: qsTr("Task Title:")
            font.pixelSize: theme.fontSizeMedium
        }

        TextField {
            id: titleField
            Layout.fillWidth: true
            placeholderText: qsTr("Enter task title...")
            font.pixelSize: theme.fontSizeMedium

            background: Rectangle {
                color: theme.surfaceColor
                border.color: titleField.activeFocus ? theme.primaryColor : theme.textDisabled
                border.width: titleField.activeFocus ? 2 : 1
                radius: theme.borderRadius
            }

            Keys.onReturnPressed: {
                if (titleField.text.trim().length > 0) {
                    acceptButton.clicked()
                }
            }
        }

        Label {
            text: qsTr("Description (Optional):")
            font.pixelSize: theme.fontSizeMedium
        }

        //ScrollView {
            //Layout.fillWidth: true
            //Layout.fillHeight: true

            TextField {
                id: descriptionField
                placeholderText: qsTr("Enter task description...")
                wrapMode: TextArea.Wrap
                font.pixelSize: theme.fontSizeMedium

                background: Rectangle {
                    color: theme.surfaceColor
                    border.color: descriptionField.activeFocus ? theme.primaryColor : theme.textDisabled
                    border.width: descriptionField.activeFocus ? 2 : 1
                    radius: theme.borderRadius
                }
            }
        //}

        RowLayout {
            Layout.fillWidth: true

            Label {
                text: qsTr("Priority:")
                font.pixelSize: theme.fontSizeMedium
            }

            ComboBox {
                id: priorityComboBox
                Layout.preferredWidth: 120
                font.pixelSize: 12  // Selected item font

                model: [
                    { text: qsTr("Low"), value: Task.Low },
                    { text: qsTr("Medium"), value: Task.Medium },
                    { text: qsTr("High"), value: Task.High }
                ]

                textRole: "text"
                valueRole: "value"
                currentIndex: 1

                background: Rectangle {
                    color: theme.surfaceColor
                    border.color: theme.textDisabled
                    border.width: 1
                    radius: theme.borderRadius
                }

                // Custom delegate with responsive sizing
                delegate: ItemDelegate {
                    width: priorityComboBox.width
                    height: 20  // Just change this for taller/shorter items
                    text: modelData.text
                }
            }

            Item {
                Layout.fillWidth: true
            }
        }
    }

    footer: DialogButtonBox {
        CustomButton {
            id: acceptButton
            text: qsTr("Add Task")
            primary: true
            enabled: titleField.text.trim().length > 0
            opacity: enabled ? 1.0 : 0.6

            DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole

            onClicked: {
                if (taskController.createTask(
                    titleField.text.trim(),
                    descriptionField.text.trim(),
                    priorityComboBox.currentValue
                )) {
                    root.close()
                }
            }
        }

        CustomButton {
            text: qsTr("Cancel")
            DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
            onClicked: root.close()
        }
    }
}
