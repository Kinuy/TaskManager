#pragma once

#include <QAbstractListModel>
#include <QQmlEngine>
#include "Task.h"


/**
 * @file TaskModel.h
 * @brief Task management model for Qt/QML applications
 */

/**
 * @class TaskModel
 * @brief A list model for managing tasks in Qt/QML applications
 *
 * TaskModel extends QAbstractListModel to provide a data model for task management.
 * It supports adding, removing, toggling completion status, and clearing completed tasks.
 * The model is designed to work seamlessly with QML ListView and other Qt Quick components.
 *
 * @note This class is QML_ELEMENT enabled and can be directly used in QML files.
 *
 * Example usage:
 * @code
 * TaskModel *model = new TaskModel(this);
 * model->addTask("Complete project", "Finish the Qt application");
 * model->toggleCompleted(0);
 * @endcode
 */
class TaskModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    /**
     * @property count
     * @brief The number of tasks in the model
     *
     * This property provides the total count of tasks currently stored in the model.
     * It automatically updates when tasks are added or removed.
     */
    Q_PROPERTY(int count READ count NOTIFY countChanged)

private:

    QList<Task *> tasks; ///< Internal list of task pointers

public:

    /**
     * @enum TaskRoles
     * @brief Custom roles for accessing task data in the model
     *
     * These roles extend Qt::UserRole to provide access to specific task properties
     * when using the model with Qt's view classes or QML.
     */
    enum TaskRoles
    {
        TitleRole = Qt::UserRole + 1,   ///< Role for accessing task title (QString)
        DescriptionRole,                ///< Role for accessing task description (QString)
        CompletedRole,                  ///< Role for accessing completion status (bool)
        CreatedAtRole,                  ///< Role for accessing creation timestamp (QDateTime)
        PriorityRole,                   ///< Role for accessing task priority (int/enum)
        TaskObjectRole                  ///< Role for accessing the complete Task object (Task*)
    };

    /**
     * @brief Constructs a TaskModel with the specified parent
     * @param parent The parent QObject, typically nullptr or the owning object
     */
    explicit TaskModel(QObject *parent = nullptr);

    /**
     * @brief Returns the number of rows (tasks) in the model
     * @param parent The parent model index (unused for list models)
     * @return The number of tasks in the model
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief Returns data for the specified model index and role
     * @param index The model index to retrieve data for
     * @param role The role specifying which data to retrieve
     * @return QVariant containing the requested data, or invalid QVariant if not found
     *
     * Supports all TaskRoles enum values for accessing different task properties.
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief Sets data for the specified model index and role
     * @param index The model index to modify
     * @param value The new value to set
     * @param role The role specifying which data to modify
     * @return true if the data was successfully set, false otherwise
     *
     * Supports modification of editable task properties through their respective roles.
     */
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    /**
     * @brief Returns the total number of tasks in the model
     * @return The count of tasks currently stored in the model
     *
     * This is the getter for the count Q_PROPERTY.
     */
    int count() const { return tasks.size(); }

    /**
     * @brief Returns the mapping of role names to role identifiers
     * @return Hash map of role names (QByteArray) to role IDs (int)
     *
     * This enables QML to access model data using property names like "title", "completed", etc.
     */
    QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief Returns the item flags for the specified model index
     * @param index The model index to get flags for
     * @return Qt::ItemFlags indicating what operations are supported for this item
     */
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    /**
     * @brief Adds a new task to the model
     * @param title The title of the new task (required)
     * @param description The description of the new task (optional)
     * @return true if the task was successfully added, false otherwise
     *
     * The new task is added to the end of the list and will have default values
     * for completion status (false) and creation timestamp (current time).
     */
    Q_INVOKABLE bool addTask(const QString &title, const QString &description = QString());

    /**
     * @brief Removes a task from the model at the specified index
     * @param index The zero-based index of the task to remove
     * @return true if the task was successfully removed, false if index is invalid
     *
     * @warning The Task object will be deleted and any pointers to it will become invalid.
     */
    Q_INVOKABLE bool removeTask(int index);

    /**
     * @brief Toggles the completion status of a task at the specified index
     * @param index The zero-based index of the task to toggle
     *
     * If the task is completed, it becomes incomplete, and vice versa.
     * Does nothing if the index is invalid.
     */
    Q_INVOKABLE void toggleCompleted(int index);

    /**
     * @brief Removes all completed tasks from the model
     *
     * This method iterates through all tasks and removes those marked as completed.
     * The Task objects will be deleted and the model will be updated accordingly.
     */
    Q_INVOKABLE void clearCompleted();

    /**
     * @brief Retrieves a task object at the specified index
     * @param index The zero-based index of the task to retrieve
     * @return Pointer to the Task object, or nullptr if index is invalid
     *
     * @warning The returned pointer remains valid only while the task exists in the model.
     * Do not store this pointer long-term as it may become invalid if the task is removed.
     */
    Q_INVOKABLE Task *getTask(int index) const;



signals:

    /**
     * @brief Emitted when the number of tasks in the model changes
     *
     * This signal is connected to the count Q_PROPERTY and will be emitted
     * automatically when tasks are added or removed from the model.
     */
    void countChanged();

private slots:

    /**
     * @brief Handles changes to individual task properties
     *
     * This slot should be connected to Task objects' change signals to ensure
     * the model emits appropriate dataChanged() signals when task properties
     * are modified externally.
     */
    void onTaskChanged();


};
