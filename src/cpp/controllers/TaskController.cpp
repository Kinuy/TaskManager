#include "TaskController.h"

TaskController::TaskController(QObject *parent)
    : QObject(parent), model(new TaskModel(this))
{
    connect(model, &TaskModel::countChanged, this, &TaskController::onModelCountChanged);
    connect(model, &TaskModel::dataChanged, this, &TaskController::onModelDataChanged);
}

int TaskController::totalTasks() const
{
    return model->count();
}

int TaskController::completedTasks() const
{
    int count = 0;
    for (int i = 0; i < model->count(); ++i)
    {
        if (model->data(model->index(i), TaskModel::CompletedRole).toBool())
        {
            count++;
        }
    }
    return count;
}

int TaskController::pendingTasks() const
{
    return totalTasks() - completedTasks();
}

bool TaskController::createTask(const QString &title, const QString &description, int priority)
{
    bool success = model->addTask(title, description);
    if (success && priority >= Task::Low && priority <= Task::High)
    {
        // Set priority on the newly created task
        int lastIndex = model->count() - 1;
        model->setData(model->index(lastIndex), priority, TaskModel::PriorityRole);
    }
    return success;
}

bool TaskController::deleteTask(int index)
{
    return model->removeTask(index);
}

void TaskController::toggleTask(int index)
{
    model->toggleCompleted(index);
    updateStatistics();
}

void TaskController::clearCompletedTasks()
{
    model->clearCompleted();
}

void TaskController::loadSampleData()
{
    createTask("Learn Qt QML", "Study Qt Quick and QML basics", Task::Medium);
    createTask("Write unit tests", "Add comprehensive test coverage", Task::High);
    createTask("Documentation", "Write project documentation", Task::Low);
    createTask("Code review", "Review pull requests", Task::Medium);
}

QList<int> TaskController::getTasksByPriority(int priority) const
{
    QList<int> indices;
    for (int i = 0; i < model->count(); ++i)
    {
        if (model->data(model->index(i), TaskModel::PriorityRole).toInt() == priority)
        {
            indices.append(i);
        }
    }
    return indices;
}

QList<int> TaskController::getCompletedTasks() const
{
    QList<int> indices;
    for (int i = 0; i < model->count(); ++i)
    {
        if (model->data(model->index(i), TaskModel::CompletedRole).toBool())
        {
            indices.append(i);
        }
    }
    return indices;
}

QList<int> TaskController::getPendingTasks() const
{
    QList<int> indices;
    for (int i = 0; i < model->count(); ++i)
    {
        if (!model->data(model->index(i), TaskModel::CompletedRole).toBool())
        {
            indices.append(i);
        }
    }
    return indices;
}

void TaskController::onModelCountChanged()
{
    updateStatistics();
}

void TaskController::onModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles)
{
    Q_UNUSED(topLeft)
    Q_UNUSED(bottomRight)

    if (roles.contains(TaskModel::CompletedRole))
    {
        updateStatistics();
    }
}

void TaskController::updateStatistics()
{
    emit totalTasksChanged();
    emit completedTasksChanged();
    emit pendingTasksChanged();
}
