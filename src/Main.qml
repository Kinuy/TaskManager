import QtQuick
import QtQuick.Controls

import TaskManager 1.0

ApplicationWindow {
    id: window
    width: 800
    height: 600
    visible: true

    title: qsTr("Task Manager")

    AppTheme {
        id: theme
    }

    TaskListPage {
        anchors.fill: parent
    }
}
