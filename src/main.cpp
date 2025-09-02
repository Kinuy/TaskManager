#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QIcon>

#include "Task.h"
#include "TaskModel.h"
#include "TaskController.h"

using namespace Qt::StringLiterals;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setApplicationName("TaskManager");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Kinuy-Lab");

    // Set the Qt Quick Controls style BEFORE creating QQmlApplicationEngine
    QQuickStyle::setStyle("Material");  // or "Basic", "Fusion", "Universal"

    // Register c++ class as QML types
    qmlRegisterType<Task>("TaskManager", 1, 0, "Task");
    qmlRegisterType<TaskModel>("TaskManager", 1, 0, "TaskModel");
    qmlRegisterType<TaskController>("TaskManager", 1, 0, "TaskController");

    QQmlApplicationEngine engine;

    // Create and expose global task controller
    TaskController taskController;
    engine.rootContext()->setContextProperty("taskController", &taskController);

    // Load sample data for demo
    taskController.loadSampleData();

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("TaskManager", "Main");

    if(engine.rootObjects().isEmpty()){
        return -1;
    }

    return app.exec();
}
