#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml/qqmlextensionplugin.h>
#include "DataController.h"

Q_IMPORT_QML_PLUGIN(RickMortyUIPlugin)

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    DataController dataController;
    engine.rootContext()->setContextProperty("dataController", &dataController);

    QObject::connect
    (
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection
    );

    engine.loadFromModule("RickMortyAPI", "Main");

    dataController.loadEpisodes();

    return app.exec();
}
