#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QIcon>

using namespace Qt::StringLiterals;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setApplicationName("TaskManager");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Kinuy GmbH");

    // Set the Qt Quick Controls style BEFORE creating QQmlApplicationEngine
    QQuickStyle::setStyle("Material");  // or "Basic", "Fusion", "Universal"

    QQmlApplicationEngine engine;

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
