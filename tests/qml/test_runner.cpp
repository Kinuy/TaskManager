#include <QtQuickTest/quicktest.h>
#include <QtQml/qqml.h>
#include <QtQml/QQmlEngine>
#include <QtCore/QDir>

#include "models/Task.h"
#include "models/TaskModel.h"
#include "controllers/TaskController.h"

class QMLTestSetup : public QObject
{
    Q_OBJECT

public slots:
    void qmlEngineAvailable(QQmlEngine *engine)
    {
        // Register types for testing
        qmlRegisterType<Task>("TaskManager", 1, 0, "Task");
        qmlRegisterType<TaskModel>("TaskManager", 1, 0, "TaskModel");
        qmlRegisterType<TaskController>("TaskManager", 1, 0, "TaskController");

        // Add import paths
        const QString importPath = QStringLiteral(QML_IMPORT_PATH);
        if (!importPath.isEmpty())
        {
            engine->addImportPath(importPath);
        }

        const QString sourceDir = QStringLiteral(QUICK_TEST_SOURCE_DIR);
        if (!sourceDir.isEmpty() && QDir(sourceDir).exists())
        {
            engine->addImportPath(QDir(sourceDir).absolutePath());
        }
    }
};

QUICK_TEST_MAIN_WITH_SETUP(qml_tests, QMLTestSetup)

#include "test_runner.moc"
