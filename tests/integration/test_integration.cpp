#include <QtTest/QtTest>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QQuickItem>
#include "controllers/TaskController.h"



class TestIntegration : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testFullApplicationWorkflow();

private:
    QQmlApplicationEngine *engine;
    TaskController *taskController;
};

void TestIntegration::initTestCase()
{
    // Register QML types
    qmlRegisterType<Task>("TaskManager", 1, 0, "Task");
    qmlRegisterType<TaskModel>("TaskManager", 1, 0, "TaskModel");
    qmlRegisterType<TaskController>("TaskManager", 1, 0, "TaskController");
}

void TestIntegration::cleanupTestCase()
{
    // Cleanup after all tests
}

void TestIntegration::init()
{
    engine = new QQmlApplicationEngine(this);
    taskController = new TaskController(this);
    engine->rootContext()->setContextProperty("taskController", taskController);
}

void TestIntegration::cleanup()
{
    delete engine;
    engine = nullptr;
    taskController = nullptr;
}

void TestIntegration::testFullApplicationWorkflow()
{
    // Load the main QML file
    const QUrl url("qrc:/qt/qml/TaskManager/main.qml");
    engine->load(url);

    QVERIFY(!engine->rootObjects().isEmpty());

    auto window = qobject_cast<QQuickWindow *>(engine->rootObjects().first());
    QVERIFY(window != nullptr);

    // Test complete workflow: Create -> Complete -> Delete tasks

    // 1. Start with empty task list
    QCOMPARE(taskController->totalTasks(), 0);

    // 2. Create some tasks
    taskController->createTask("Integration Test Task 1", "Description 1", Task::High);
    taskController->createTask("Integration Test Task 2", "Description 2", Task::Medium);
    taskController->createTask("Integration Test Task 3", "Description 3", Task::Low);

    QCOMPARE(taskController->totalTasks(), 3);
    QCOMPARE(taskController->pendingTasks(), 3);
    QCOMPARE(taskController->completedTasks(), 0);

    // 3. Complete some tasks
    taskController->toggleTask(0);
    taskController->toggleTask(1);

    QCOMPARE(taskController->totalTasks(), 3);
    QCOMPARE(taskController->pendingTasks(), 1);
    QCOMPARE(taskController->completedTasks(), 2);

    // 4. Clear completed tasks
    taskController->clearCompletedTasks();

    QCOMPARE(taskController->totalTasks(), 1);
    QCOMPARE(taskController->pendingTasks(), 1);
    QCOMPARE(taskController->completedTasks(), 0);

    // 5. Verify remaining task
    Task *remainingTask = taskController->taskModel()->getTask(0);
    QVERIFY(remainingTask != nullptr);
    QCOMPARE(remainingTask->getTitle(), "Integration Test Task 3");
    QCOMPARE(remainingTask->getPriority(), Task::Low);
    QVERIFY(!remainingTask->getCompleted());
}

QTEST_MAIN(TestIntegration)
#include "test_integration.moc"
