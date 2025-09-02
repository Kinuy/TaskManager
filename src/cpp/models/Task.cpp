#include "Task.h"

Task::Task(QObject *parent)
    : QObject(parent), completed(false), createdAt(QDateTime::currentDateTime()), priority(Medium)
{
}

Task::Task(const QString &title, const QString &description, QObject *parent)
    : QObject(parent), title(title), description(description), completed(false), createdAt(QDateTime::currentDateTime()), priority(Medium)
{
}

void Task::setTitle(const QString &ttl)
{
    if (title != ttl)
    {
        title = ttl;
        emit titleChanged();
    }
}

void Task::setDescription(const QString &desc)
{
    if (description != desc)
    {
        description = desc;
        emit descriptionChanged();
    }
}

void Task::setCompleted(bool comp)
{
    if (completed != comp)
    {
        completed = comp;
        emit completedChanged();
    }
}

void Task::setPriority(int prio)
{
    if (priority >= Low && priority <= High && priority != prio)
    {
        priority = prio;
        emit priorityChanged();
    }
}

bool Task::isValid() const
{
    return !title.trimmed().isEmpty();
}

QString Task::priorityString() const
{
    switch (priority)
    {
    case Low:
        return "Low";
    case Medium:
        return "Medium";
    case High:
        return "High";
    default:
        return "Unknown";
    }
}
