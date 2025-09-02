#include "TaskModel.h"

TaskModel::TaskModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int TaskModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return tasks.size();
}

QVariant TaskModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= tasks.size())
        return QVariant();

    Task *task = tasks[index.row()];

    switch (role)
    {
    case TitleRole:
        return task->getTitle();
    case DescriptionRole:
        return task->getDescription();
    case CompletedRole:
        return task->getCompleted();
    case CreatedAtRole:
        return task->getDateTime();
    case PriorityRole:
        return task->getPriority();
    case TaskObjectRole:
        return QVariant::fromValue(task);
    }

    return QVariant();
}

bool TaskModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= tasks.size())
        return false;

    Task *task = tasks[index.row()];

    switch (role)
    {
    case TitleRole:
        task->setTitle(value.toString());
        break;
    case DescriptionRole:
        task->setDescription(value.toString());
        break;
    case CompletedRole:
        task->setCompleted(value.toBool());
        break;
    case PriorityRole:
        task->setPriority(value.toInt());
        break;
    default:
        return false;
    }

    emit dataChanged(index, index, {role});
    return true;
}

QHash<int, QByteArray> TaskModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[DescriptionRole] = "description";
    roles[CompletedRole] = "completed";
    roles[CreatedAtRole] = "createdAt";
    roles[PriorityRole] = "priority";
    roles[TaskObjectRole] = "taskObject";
    return roles;
}

Qt::ItemFlags TaskModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool TaskModel::addTask(const QString &title, const QString &description)
{
    if (title.trimmed().isEmpty())
        return false;

    beginInsertRows(QModelIndex(), tasks.size(), tasks.size());

    Task *task = new Task(title.trimmed(), description, this);
    connect(task, &Task::titleChanged, this, &TaskModel::onTaskChanged);
    connect(task, &Task::descriptionChanged, this, &TaskModel::onTaskChanged);
    connect(task, &Task::completedChanged, this, &TaskModel::onTaskChanged);
    connect(task, &Task::priorityChanged, this, &TaskModel::onTaskChanged);

    tasks.append(task);
    endInsertRows();

    emit countChanged();
    return true;
}

bool TaskModel::removeTask(int index)
{
    if (index < 0 || index >= tasks.size())
        return false;

    beginRemoveRows(QModelIndex(), index, index);
    Task *task = tasks.takeAt(index);
    task->deleteLater();
    endRemoveRows();

    emit countChanged();
    return true;
}

void TaskModel::toggleCompleted(int index)
{
    if (index < 0 || index >= tasks.size())
        return;

    Task *task = tasks[index];
    task->setCompleted(!task->getCompleted());
}

void TaskModel::clearCompleted()
{
    for (int i = tasks.size() - 1; i >= 0; --i)
    {
        if (tasks[i]->getCompleted())
        {
            removeTask(i);
        }
    }
}

Task *TaskModel::getTask(int index) const
{
    if (index < 0 || index >= tasks.size())
        return nullptr;

    return tasks[index];
}

void TaskModel::onTaskChanged()
{
    Task *task = qobject_cast<Task *>(sender());
    if (!task)
        return;

    int index = tasks.indexOf(task);
    if (index >= 0)
    {
        QModelIndex modelIndex = createIndex(index, 0);
        emit dataChanged(modelIndex, modelIndex);
    }
}
