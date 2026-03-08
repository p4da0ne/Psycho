#include <QtGui>
#include <QtQuickWidgets>
#include "mainform.h"
#include "qapplication.h"
#include "qstylefactory.h"

int main(int argc, char *argv[])
{

    // qputenv("QSG_INFO", "1");
    // qputenv("GL_DEBUG", "1");
    // qputenv("QT_OPENGL","desktop");
    // qputenv("QSG_RENDER_LOOP","windows");

    // QLoggingCategory::setFilterRules(
    //     "qt.qml.debug=true\n"
    //     "qt.qml.import.debug=false\n"
    //     "qt.qml.plugin.debug=true\n"
    //     "qt.core.plugin.debug=true\n"
    //     "qt.qpa.plugin.debug=true\n"
    //     "qt.scenegraph.general=true\n"
    //     "qt.opengl.backend=true\n"
    //     );

    // QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

    // QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QGuiApplication app(argc, argv);
    QGuiApplication::setOrganizationName("YourCompany");
    QGuiApplication::setApplicationName("SaturnGIS");

    qmlRegisterSingletonType<DataAccess>("Saturn.Backend", 1, 0, "Database", DataAccess::qmlSingleton);
    qmlRegisterSingletonType<AuthManager>    ("Saturn.Backend", 1, 0, "Auth",     AuthManager::qmlSingleton);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/main.qml"_qs);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl)
                     {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}

