#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>


/**
 * @file Task.h
 * @brief Task entity class for task management applications
 */

/**
 * @class Task
 * @brief Represents a single task with title, description, completion status, and priority
 *
 * Task is a QObject-derived class that represents an individual task item in a task management
 * system. It provides properties for task metadata and supports Qt's property system with
 * automatic change notifications. The class is designed to work seamlessly with Qt's model/view
 * architecture and QML.
 *
 * Each task has a unique creation timestamp that is set when the task is constructed and
 * cannot be modified afterwards. The task supports three priority levels: Low, Medium, and High.
 *
 * Example usage:
 * @code
 * Task *task = new Task("Buy groceries", "Milk, bread, and eggs", this);
 * task->setPriority(Task::High);
 * task->setCompleted(true);
 * qDebug() << "Task created:" << task->createdAt();
 * qDebug() << "Priority:" << task->priorityString();
 * @endcode
 */
class Task : public QObject
{
    Q_OBJECT

    /**
     * @property title
     * @brief The title/name of the task
     *
     * A brief, descriptive title for the task. This property is read-write and emits
     * titleChanged() when modified.
     */
    Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged)

    /**
     * @property description
     * @brief Detailed description of the task
     *
     * An optional, longer description providing additional details about the task.
     * This property is read-write and emits descriptionChanged() when modified.
     */
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)

    /**
     * @property completed
     * @brief Completion status of the task
     *
     * Boolean flag indicating whether the task has been completed.
     * This property is read-write and emits completedChanged() when modified.
     * Default value is false for new tasks.
     */
    Q_PROPERTY(bool completed READ getCompleted WRITE setCompleted NOTIFY completedChanged)

    /**
     * @property createdAt
     * @brief Timestamp when the task was created
     *
     * Read-only property containing the QDateTime when the task object was constructed.
     * This value is set automatically during construction and cannot be changed.
     */
    Q_PROPERTY(QDateTime dateTime READ getDateTime CONSTANT)

    /**
     * @property priority
     * @brief Priority level of the task
     *
     * Integer value representing the task priority (0=Low, 1=Medium, 2=High).
     * This property is read-write and emits priorityChanged() when modified.
     * Use the Priority enum values for better code readability.
     */
    Q_PROPERTY(int priority READ getPriority WRITE setPriority NOTIFY priorityChanged)


private:


    QString title;        ///< Internal storage for task title
    QString description;  ///< Internal storage for task description
    bool completed;       ///< Internal storage for completion status
    QDateTime createdAt;  ///< Internal storage for creation timestamp
    int priority;         ///< Internal storage for priority level

public:

    /**
     * @enum Priority
     * @brief Enumeration of available task priority levels
     *
     * This enum defines the three available priority levels for tasks.
     * The values are also exposed to QML through Q_ENUM registration.
     */
    enum Priority
    {
        Low = 0, ///< Low priority task (value: 0)
        Medium = 1, ///< Medium priority task (value: 1)
        High = 2 ///< High priority task (value: 2)
    };
    Q_ENUM(Priority)

    /**
     * @brief Default constructor for creating an empty task
     * @param parent The parent QObject, typically nullptr or the owning object
     *
     * Creates a task with empty title and description, not completed,
     * Low priority, and createdAt set to the current date/time.
     */
    explicit Task(QObject *parent = nullptr);

    /**
     * @brief Constructor for creating a task with title and optional description
     * @param title The title of the task (required)
     * @param description The description of the task (optional, defaults to empty string)
     * @param parent The parent QObject, typically nullptr or the owning object
     *
     * Creates a task with the specified title and description, not completed,
     * Low priority, and createdAt set to the current date/time.
     */
    Task(const QString &title, const QString &description = QString(), QObject *parent = nullptr);

    // Getters
    /**
     * @brief Gets the task getTitle
     * @return The current getTitle of the task
     *
     * This is the getter function for the getTitle Q_PROPERTY.
     */
    QString getTitle() const { return title; }

    /**
     * @brief Gets the task getDescription
     * @return The current getDescription of the task
     *
     * This is the getter function for the getDescription Q_PROPERTY.
     */
    QString getDescription() const { return description; }

    /**
     * @brief Gets the completion status
     * @return true if the task is getCompleted, false otherwise
     *
     * This is the getter function for the getCompleted Q_PROPERTY.
     */
    bool getCompleted() const { return completed; }

    /**
     * @brief Gets the creation timestamp
     * @return QDateTime when the task was created
     *
     * This is the getter function for the getDateTime Q_PROPERTY.
     * The timestamp is set during construction and never changes.
     */
    QDateTime getDateTime() const { return createdAt; }

    /**
     * @brief Gets the priority level as an integer
     * @return Priority level (0=Low, 1=Medium, 2=High)
     *
     * This is the getter function for the priority Q_PROPERTY.
     * Consider using the Priority enum for better code readability.
     */
    int getPriority() const { return priority; }

    // Setters

    /**
     * @brief Sets the task title
     * @param title The new title for the task
     *
     * Updates the task title and emits titleChanged() if the value actually changes.
     * Empty titles are allowed but may not be meaningful for users.
     */
    void setTitle(const QString &title);

    /**
     * @brief Sets the task description
     * @param description The new description for the task
     *
     * Updates the task description and emits descriptionChanged() if the value actually changes.
     * Empty descriptions are allowed and common for simple tasks.
     */
    void setDescription(const QString &description);

    /**
     * @brief Sets the completion status
     * @param completed The new completion status
     *
     * Updates the task completion status and emits completedChanged() if the value actually changes.
     * Typically used to mark tasks as done or to reopen completed tasks.
     */
    void setCompleted(bool completed);

    /**
     * @brief Sets the priority level
     * @param priority The new priority level (0=Low, 1=Medium, 2=High)
     *
     * Updates the task priority and emits priorityChanged() if the value actually changes.
     * Invalid priority values (outside 0-2 range) may be accepted but could cause unexpected behavior.
     * Consider using the Priority enum values instead of raw integers.
     */
    void setPriority(int priority);

    // Utility methods
    /**
     * @brief Checks if the task has valid/meaningful content
     * @return true if the task is considered valid, false otherwise
     *
     * Determines whether the task contains sufficient information to be considered
     * a valid task. Typically checks if the title is non-empty, but the exact
     * validation criteria may vary based on business requirements.
     */
    Q_INVOKABLE bool isValid() const;

    /**
     * @brief Gets the priority level as a human-readable string
     * @return String representation of the priority ("Low", "Medium", or "High")
     *
     * Converts the numeric priority value to a localized string suitable for
     * display in user interfaces. Useful for QML bindings and UI components.
     */
    Q_INVOKABLE QString priorityString() const;

signals:

    /**
     * @brief Emitted when the task title changes
     *
     * This signal is emitted by setTitle() when the title actually changes
     * (i.e., the new value differs from the current value).
     * Connected to the title Q_PROPERTY for automatic QML property updates.
     */
    void titleChanged();

    /**
     * @brief Emitted when the task description changes
     *
     * This signal is emitted by setDescription() when the description actually changes
     * (i.e., the new value differs from the current value).
     * Connected to the description Q_PROPERTY for automatic QML property updates.
     */
    void descriptionChanged();

    /**
     * @brief Emitted when the task completion status changes
     *
     * This signal is emitted by setCompleted() when the completion status actually changes
     * (i.e., the new value differs from the current value).
     * Connected to the completed Q_PROPERTY for automatic QML property updates.
     */
    void completedChanged();

    /**
     * @brief Emitted when the task priority changes
     *
     * This signal is emitted by setPriority() when the priority actually changes
     * (i.e., the new value differs from the current value).
     * Connected to the priority Q_PROPERTY for automatic QML property updates.
     */
    void priorityChanged();


};
