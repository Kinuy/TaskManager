#include <QTest>
#include <QSignalSpy>
#include "models/Task.h"

class TestTask : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Basic functionality tests
    void testTaskCreation();
    void testTaskCreationWithParameters();
    void testTaskProperties();
    void testTaskSignals();

    // Property validation tests
    void testSetTitle();
    void testSetDescription();
    void testSetCompleted();
    void testSetPriority();

    // Utility method tests
    void testIsValid();
    void testPriorityString();
    void testPriorityStringData();

private:
    Task *task;
};

void TestTask::initTestCase()
{
    // Setup that runs once before all tests
}

void TestTask::cleanupTestCase()
{
    // Cleanup that runs once after all tests
}

void TestTask::init()
{
    task = new Task(this);
}

void TestTask::cleanup()
{
    delete task;
    task = nullptr;
}

void TestTask::testTaskCreation()
{
    QVERIFY(task != nullptr);
    QVERIFY(task->getTitle().isEmpty());
    QVERIFY(task->getDescription().isEmpty());
    QCOMPARE(task->getCompleted(), false);
    QCOMPARE(task->getPriority(), Task::Medium);
    QVERIFY(!task->getDateTime().isNull());
}

void TestTask::testTaskCreationWithParameters()
{
    Task *paramTask = new Task("Test Title", "Test Description", this);

    QCOMPARE(paramTask->getTitle(), "Test Title");
    QCOMPARE(paramTask->getDescription(), "Test Description");
    QCOMPARE(paramTask->getCompleted(), false);
    QCOMPARE(paramTask->getPriority(), Task::Medium);

    delete paramTask;
}

void TestTask::testTaskProperties()
{
    // Test property access through QObject property system
    task->setProperty("title", "Property Title");
    task->setProperty("description", "Property Description");
    task->setProperty("completed", true);
    task->setProperty("priority", Task::High);

    QCOMPARE(task->property("title").toString(), "Property Title");
    QCOMPARE(task->property("description").toString(), "Property Description");
    QCOMPARE(task->property("completed").toBool(), true);
    QCOMPARE(task->property("priority").toInt(), Task::High);
}

void TestTask::testTaskSignals()
{
    QSignalSpy titleSpy(task, &Task::titleChanged);
    QSignalSpy descriptionSpy(task, &Task::descriptionChanged);
    QSignalSpy completedSpy(task, &Task::completedChanged);
    QSignalSpy prioritySpy(task, &Task::priorityChanged);

    task->setTitle("New Title");
    task->setDescription("New Description");
    task->setCompleted(true);
    task->setPriority(Task::High);

    QCOMPARE(titleSpy.count(), 1);
    QCOMPARE(descriptionSpy.count(), 1);
    QCOMPARE(completedSpy.count(), 1);
    QCOMPARE(prioritySpy.count(), 1);

    // Setting same values should not emit signals
    task->setTitle("New Title");
    task->setCompleted(true);

    QCOMPARE(titleSpy.count(), 1);
    QCOMPARE(completedSpy.count(), 1);
}

void TestTask::testSetTitle()
{
    task->setTitle("Test Title");
    QCOMPARE(task->getTitle(), "Test Title");

    // Test with empty string
    task->setTitle("");
    QCOMPARE(task->getTitle(), "");

    // Test with whitespace
    task->setTitle("   Whitespace Title   ");
    QCOMPARE(task->getTitle(), "   Whitespace Title   ");
}

void TestTask::testSetDescription()
{
    task->setDescription("Test Description");
    QCOMPARE(task->getDescription(), "Test Description");

    task->setDescription("");
    QCOMPARE(task->getDescription(), "");
}

void TestTask::testSetCompleted()
{
    task->setCompleted(true);
    QCOMPARE(task->getCompleted(), true);

    task->setCompleted(false);
    QCOMPARE(task->getCompleted(), false);
}

void TestTask::testSetPriority()
{
    task->setPriority(Task::Low);
    QCOMPARE(task->getPriority(), Task::Low);

    task->setPriority(Task::High);
    QCOMPARE(task->getPriority(), Task::High);

    // Test invalid priority (should not change)
    // task->setPriority(-1);
    // QCOMPARE(task->getPriority(), Task::High);

    // task->setPriority(999);
    // QCOMPARE(task->getPriority(), Task::High);
}

void TestTask::testIsValid()
{
    // Empty title should be invalid
    QVERIFY(!task->isValid());

    // Title with only whitespace should be invalid
    task->setTitle("   ");
    QVERIFY(!task->isValid());

    // Non-empty title should be valid
    task->setTitle("Valid Title");
    QVERIFY(task->isValid());

    // Title with leading/trailing whitespace should be valid
    task->setTitle("  Valid Title  ");
    QVERIFY(task->isValid());
}

void TestTask::testPriorityString()
{
    // QFETCH(int, priority);
    // QFETCH(QString, expected);

    // task->setPriority(priority);
    // QCOMPARE(task->priorityString(), expected);
}

void TestTask::testPriorityStringData()
{
    QTest::addColumn<int>("priority");
    QTest::addColumn<QString>("expected");

    QTest::newRow("Low") << static_cast<int>(Task::Low) << "Low";
    QTest::newRow("Medium") << static_cast<int>(Task::Medium) << "Medium";
    QTest::newRow("High") << static_cast<int>(Task::High) << "High";
}

QTEST_MAIN(TestTask)
#include "test_task.moc"
