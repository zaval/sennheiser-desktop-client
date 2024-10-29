#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>


#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define PROJECT_VERSION_MAJOR_STR TOSTRING(PROJECT_VERSION_MAJOR)
#define PROJECT_VERSION_MINOR_STR TOSTRING(PROJECT_VERSION_MINOR)
#define PROJECT_VERSION_PATCH_STR TOSTRING(PROJECT_VERSION_PATCH)
#define PROJECT_DESCRIPTION_STR TOSTRING(PROJECT_DESCRIPTION)

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(
            "projectVersionMajor",
            PROJECT_VERSION_MAJOR_STR
            );
    engine.rootContext()->setContextProperty(
            "projectVersionMinor",
            PROJECT_VERSION_MINOR_STR
            );
    engine.rootContext()->setContextProperty(
            "projectVersionPatch",
            PROJECT_VERSION_PATCH_STR
            );
    engine.rootContext()->setContextProperty(
            "projectDescription",
            PROJECT_DESCRIPTION_STR
            );
    engine.rootContext()->setContextProperty(
            "projectUrl",
            PROJECT_URL
            );

//    qDebug() << engine.offlineStoragePath();

    QObject::connect(
            &engine,
            &QQmlApplicationEngine::objectCreationFailed,
            &app,
            []() { QCoreApplication::exit(-1); },
            Qt::QueuedConnection);
    engine.loadFromModule("btui", "Main");

    return app.exec();
}
