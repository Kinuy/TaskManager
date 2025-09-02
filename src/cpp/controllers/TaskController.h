#pragma once

#include <QObject>
#include <QQmlEngine>
#include "TaskModel.h"


/**
 * @file TaskController.h
 * @brief Controller class for managing task operations and statistics
 */

/**
 * @class TaskController
 * @brief High-level controller for task management operations and statistics
 *
 * TaskController serves as the primary interface for task management in the application,
 * implementing a controller pattern that coordinates between the TaskModel and the UI layer.
 * It provides simplified access to common task operations, maintains real-time statistics,
 * and offers filtering capabilities for task queries.
 *
 * The controller automatically tracks and updates statistics (total, completed, and pending
 * task counts) whenever the underlying model changes. It exposes these statistics as
 * Q_PROPERTY bindings for seamless QML integration.
 *
 * Key responsibilities:
 * - Wrapping TaskModel operations with higher-level business logic
 * - Maintaining real-time task statistics
 * - Providing convenient methods for common task operations
 * - Offering filtering and querying capabilities
 * - Serving as the primary QML interface for task management
 *
 * @note This class is QML_ELEMENT enabled and designed to be the main interface
 * between QML views and the task management system.
 *
 * Example usage:
 * @code
 * TaskController *controller = new TaskController(this);
 * controller->createTask("Buy groceries", "Milk, bread, eggs", Task::High);
 * controller->loadSampleData();
 *
 * qDebug() << "Total tasks:" << controller->totalTasks();
 * qDebug() << "Completed:" << controller->completedTasks();
 *
 * QList<int> highPriorityTasks = controller->getTasksByPriority(Task::High);
 * @endcode
 */
class TaskController : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    /**
     * @property model
     * @brief The underlying TaskModel that stores the task data
     *
     * Provides direct access to the TaskModel for advanced operations or when
     * the controller's simplified interface is insufficient. This property is
     * read-only (CONSTANT) to prevent external code from replacing the model.
     */
    Q_PROPERTY(TaskModel *taskModel READ taskModel CONSTANT)

    /**
     * @property totalTasks
     * @brief The total number of tasks in the system
     *
     * Read-only property that automatically updates when tasks are added or removed.
     * Emits totalTasksChanged() when the value changes.
     */
    Q_PROPERTY(int totalTasks READ totalTasks NOTIFY totalTasksChanged)

    /**
     * @property completedTasks
     * @brief The number of completed tasks
     *
     * Read-only property that automatically updates when tasks are completed/uncompleted
     * or when completed tasks are added/removed. Emits completedTasksChanged() when the value changes.
     */
    Q_PROPERTY(int completedTasks READ completedTasks NOTIFY completedTasksChanged)

    /**
     * @property pendingTasks
     * @brief The number of pending (incomplete) tasks
     *
     * Read-only property that automatically updates when task completion status changes
     * or when tasks are added/removed. Emits pendingTasksChanged() when the value changes.
     * This value equals totalTasks - completedTasks.
     */
    Q_PROPERTY(int pendingTasks READ pendingTasks NOTIFY pendingTasksChanged)


private:

    TaskModel *model; ///< Internal TaskModel instance that stores task data

    /**
     * @brief Updates all task statistics and emits change signals if needed
     *
     * Internal method that recalculates totalTasks, completedTasks, and pendingTasks
     * counters and emits the appropriate change signals when values differ from
     * previous calculations. Called automatically when the model changes.
     */
    void updateStatistics();

public:

    /**
     * @brief Constructs a TaskController with the specified parent
     * @param parent The parent QObject, typically the application or main window
     *
     * Creates a new TaskController instance with an internal TaskModel.
     * Automatically connects to the model's signals to track changes and
     * update statistics in real-time.
     */
    explicit TaskController(QObject *parent = nullptr);

    /**
     * @brief Gets the underlying TaskModel
     * @return Pointer to the internal TaskModel instance
     *
     * Provides access to the TaskModel for operations not covered by the
     * controller's simplified interface. The returned pointer remains valid
     * for the lifetime of the TaskController.
     */
    TaskModel *taskModel() const { return model; }

    // Statistics
    /**
     * @brief Gets the total number of tasks
     * @return Total count of all tasks (completed and pending)
     *
     * This is the getter for the totalTasks Q_PROPERTY.
     * The value is calculated dynamically from the underlying model.
     */
    int totalTasks() const;

    /**
     * @brief Gets the number of completed tasks
     * @return Count of tasks marked as completed
     *
     * This is the getter for the completedTasks Q_PROPERTY.
     * The value is calculated by iterating through all tasks and counting
     * those with completed status set to true.
     */
    int completedTasks() const;

    /**
     * @brief Gets the number of pending (incomplete) tasks
     * @return Count of tasks not yet completed
     *
     * This is the getter for the pendingTasks Q_PROPERTY.
     * The value equals totalTasks() - completedTasks().
     */
    int pendingTasks() const;

    // Actions
    /**
     * @brief Creates a new task with the specified parameters
     * @param title The title of the new task (required)
     * @param description Optional description for the task (default: empty)
     * @param priority Priority level for the task (default: 1/Medium)
     * @return true if the task was successfully created, false otherwise
     *
     * Creates a new task using the provided parameters and adds it to the model.
     * The task will be created with completed status set to false and createdAt
     * timestamp set to the current time. Statistics are updated automatically.
     *
     * @note Priority should typically use Task::Priority enum values (0=Low, 1=Medium, 2=High)
     *
     * Example:
     * @code
     * controller->createTask("Buy groceries");
     * controller->createTask("Meeting prep", "Prepare slides for Monday", Task::High);
     * @endcode
     */
    Q_INVOKABLE bool createTask(const QString &title, const QString &description = QString(), int priority = 1);

    /**
     * @brief Deletes a task at the specified index
     * @param index Zero-based index of the task to delete
     * @return true if the task was successfully deleted, false if index is invalid
     *
     * Removes the task at the given index from the model. The task object will be
     * destroyed and statistics will be updated automatically.
     *
     * @warning After deletion, all task indices greater than the deleted index
     * will shift down by one. Update any stored indices accordingly.
     */
    Q_INVOKABLE bool deleteTask(int index);

    /**
     * @brief Toggles the completion status of a task at the specified index
     * @param index Zero-based index of the task to toggle
     *
     * Flips the completion status of the specified task. Completed tasks become
     * pending, and pending tasks become completed. Statistics are updated automatically.
     * Does nothing if the index is invalid.
     */
    Q_INVOKABLE void toggleTask(int index);

    /**
     * @brief Removes all completed tasks from the system
     *
     * Deletes all tasks that are marked as completed. This operation cannot be undone.
     * Statistics are updated automatically after the operation completes.
     *
     * @warning This permanently deletes task data. Consider providing user confirmation
     * before calling this method.
     */
    Q_INVOKABLE void clearCompletedTasks();

    /**
     * @brief Loads sample task data for demonstration purposes
     *
     * Populates the task list with predefined sample tasks for testing, demonstration,
     * or initial setup. The exact content and number of sample tasks may vary.
     * Existing tasks are not removed; sample tasks are added to the current list.
     *
     * This method is typically used during development, testing, or for providing
     * users with example data when first using the application.
     */
    Q_INVOKABLE void loadSampleData();


    /**
     * @brief Gets indices of all tasks with the specified priority
     * @param priority The priority level to search for (0=Low, 1=Medium, 2=High)
     * @return List of zero-based indices of tasks matching the priority
     *
     * Searches through all tasks and returns the indices of those with the
     * specified priority level. The returned indices can be used with other
     * controller methods or model operations.
     *
     * @note The returned indices are valid at the time of the call but may become
     * invalid if tasks are added/removed/reordered after this call.
     *
     * Example:
     * @code
     * QList<int> highPriorityIndices = controller->getTasksByPriority(Task::High);
     * for (int index : highPriorityIndices) {
     *     Task *task = controller->model()->getTask(index);
     *     qDebug() << "High priority task:" << task->title();
     * }
     * @endcode
     */
    Q_INVOKABLE QList<int> getTasksByPriority(int priority) const;

    /**
     * @brief Gets indices of all completed tasks
     * @return List of zero-based indices of completed tasks
     *
     * Returns the indices of all tasks that are marked as completed.
     * Useful for bulk operations on completed tasks or for UI filtering.
     *
     * @note The returned indices are valid at the time of the call but may become
     * invalid if tasks are added/removed/reordered after this call.
     */
    Q_INVOKABLE QList<int> getCompletedTasks() const;

    /**
     * @brief Gets indices of all pending (incomplete) tasks
     * @return List of zero-based indices of pending tasks
     *
     * Returns the indices of all tasks that are not yet completed.
     * Useful for bulk operations on pending tasks or for UI filtering.
     *
     * @note The returned indices are valid at the time of the call but may become
     * invalid if tasks are added/removed/reordered after this call.
     */
    Q_INVOKABLE QList<int> getPendingTasks() const;

signals:

    /**
     * @brief Emitted when the total number of tasks changes
     *
     * This signal is automatically emitted when tasks are added or removed.
     * Connected to the totalTasks Q_PROPERTY for automatic QML property updates.
     */
    void totalTasksChanged();

    /**
     * @brief Emitted when the number of completed tasks changes
     *
     * This signal is automatically emitted when:
     * - Tasks are completed or marked as incomplete
     * - Completed tasks are added or removed
     * - Task completion status is modified
     *
     * Connected to the completedTasks Q_PROPERTY for automatic QML property updates.
     */
    void completedTasksChanged();

    /**
     * @brief Emitted when the number of pending tasks changes
     *
     * This signal is automatically emitted when:
     * - Tasks are completed or marked as incomplete
     * - Tasks are added or removed
     * - Task completion status is modified
     *
     * Connected to the pendingTasks Q_PROPERTY for automatic QML property updates.
     */
    void pendingTasksChanged();

private slots:

    /**
     * @brief Handles changes to the model's task count
     *
     * Connected to the TaskModel's countChanged() signal. Updates statistics
     * and emits appropriate change signals when the total number of tasks changes.
     */
    void onModelCountChanged();

    /**
     * @brief Handles data changes within the model
     * @param topLeft Top-left index of the changed data range
     * @param bottomRight Bottom-right index of the changed data range
     * @param roles List of roles that changed
     *
     * Connected to the TaskModel's dataChanged() signal. Updates statistics
     * when task properties change, particularly completion status changes
     * that affect the completed/pending task counts.
     */
    void onModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles);


};
