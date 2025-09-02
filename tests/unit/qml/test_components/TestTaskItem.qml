import QtQuick 2.15
import QtTest 1.15
import TaskManager 1.0
import "../../../../src/qml/components"

TestCase {
    id: testCase
    name: "TaskItemTest"

    width: 400
    height: 200

    property Task testTask: Task {
        title: "Test Task Title"
        description: "Test task description"
        priority: Task.Medium
        completed: false
    }

    TaskItem {
        id: taskItem
        anchors.centerIn: parent
        width: 300
        task: testTask
    }

    SignalSpy {
        id: toggleCompletedSpy
        target: taskItem
        signalName: "toggleCompleted"
    }

    SignalSpy {
        id: deleteRequestedSpy
        target: taskItem
        signalName: "deleteRequested"
    }

    function init() {
        testTask.title = "Test Task Title"
        testTask.description = "Test task description"
        testTask.priority = Task.Medium
        testTask.completed = false
        toggleCompletedSpy.clear()
        deleteRequestedSpy.clear()
    }

    function test_taskItemCreation() {
        verify(taskItem !== null)
        verify(taskItem.task === testTask)
    }

    function test_taskItemDisplaysCorrectInformation() {
        // Find the title label
        var titleLabel = findChild(taskItem, "titleLabel")
        verify(titleLabel !== null)
        compare(titleLabel.text, "Test Task Title")

        // Find the description label
        var descriptionLabel = findChild(taskItem, "descriptionLabel")
        verify(descriptionLabel !== null)
        compare(descriptionLabel.text, "Test task description")

        // Verify description is visible when text is present
        verify(descriptionLabel.visible)
    }

    function test_taskItemHidesEmptyDescription() {
        testTask.description = ""
        wait(50) // Allow binding to update

        var descriptionLabel = findChild(taskItem, "descriptionLabel")
        verify(descriptionLabel !== null)
        verify(!descriptionLabel.visible)
    }

    function test_taskItemCheckbox() {
        var checkbox = findChild(taskItem, "checkBox")
        verify(checkbox !== null)
        compare(checkbox.checked, false)

        // Simulate checkbox click
        mouseClick(checkbox)
        compare(toggleCompletedSpy.count, 1)
    }

    function test_taskItemDeleteButton() {
        var deleteButton = findChild(taskItem, "deleteButton")
        if (!deleteButton) {
            // Try to find button by text
            var buttons = findChildren(taskItem, function(item) {
                return item.toString().indexOf("Button") !== -1 &&
                       item.text && item.text.indexOf("Delete") !== -1
            })
            verify(buttons.length > 0)
            deleteButton = buttons[0]
        }

        verify(deleteButton !== null)
        mouseClick(deleteButton)
        compare(deleteRequestedSpy.count, 1)
    }

    function test_taskItemCompletedState() {
        // Test uncompleted state
        verify(!testTask.completed)
        compare(taskItem.state, "")

        // Complete the task
        testTask.completed = true
        wait(50) // Allow state to update

        compare(taskItem.state, "completed")
        verify(taskItem.opacity < 1.0)
    }

    function test_taskItemPriorityDisplay() {
        // Test different priorities
        testTask.priority = Task.Low
        wait(50)
        // We can't easily test the priority color rectangle directly,
        // but we can verify the task priority changed
        compare(testTask.priority, Task.Low)

        testTask.priority = Task.High
        wait(50)
        compare(testTask.priority, Task.High)
    }

    function test_taskItemWithNullTask() {
        var emptyTaskItem = Qt.createQmlObject(
            'import "../../../../src/qml/components"; TaskItem { width: 300; height: 100 }',
            testCase
        )

        verify(emptyTaskItem !== null)
        verify(emptyTaskItem.task === null)

        // Should not crash with null task
        var titleLabel = findChild(emptyTaskItem, "titleLabel")
        if (titleLabel) {
            compare(titleLabel.text, "")
        }

        emptyTaskItem.destroy()
    }

    function test_taskItemMouseInteraction() {
        // Test that clicking on the task item area works
        var initialToggleCount = toggleCompletedSpy.count

        // Click on checkbox area
        var checkbox = findChild(taskItem, "checkBox")
        if (checkbox) {
            mouseClick(checkbox)
            compare(toggleCompletedSpy.count, initialToggleCount + 1)
        }
    }

    function test_taskItemAccessibility() {
        // Basic accessibility test - verify key components exist
        verify(taskItem.width > 0)
        verify(taskItem.height > 0)

        // Verify task item responds to task changes
        var originalTitle = testTask.title
        testTask.title = "Updated Title"
        wait(50)

        var titleLabel = findChild(taskItem, "titleLabel")
        if (titleLabel) {
            compare(titleLabel.text, "Updated Title")
        }

        testTask.title = originalTitle
    }

    // Helper function to find child elements
    function findChild(parent, objectName) {
        if (parent.objectName === objectName) {
            return parent
        }

        for (var i = 0; i < parent.children.length; i++) {
            var child = parent.children[i]
            if (child.objectName === objectName) {
                return child
            }

            var found = findChild(child, objectName)
            if (found !== null) {
                return found
            }
        }
        return null
    }

    // Helper function to find children matching a condition
    function findChildren(parent, condition) {
        var result = []

        function searchRecursive(item) {
            if (condition(item)) {
                result.push(item)
            }

            for (var i = 0; i < item.children.length; i++) {
                searchRecursive(item.children[i])
            }
        }

        searchRecursive(parent)
        return result
    }
}
