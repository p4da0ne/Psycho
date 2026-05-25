#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QQuickWindow>
#include <QSGRendererInterface>
#include <QSurfaceFormat>
#include <QTextStream>
#include <QCoreApplication>
#include <QtGlobal>

#include <authmanager.h>
#include <dataaccess.h>
#include <dbconnectionsettings.h>
#include <eventsrepository.h>
#include <geometryrepository.h>
#include <geojsonservice.h>
#include <geojsontilefeedservice.h>
#include <mapobjectsrepository.h>
#include <mapeditingservice.h>
#include <mapgeometryrolesservice.h>
#include <mapruntimeservice.h>
#include <mapsnapshotservice.h>
#include <localgeojsonrepository.h>
#include <objectdetailsrepository.h>
#include <personnelrepository.h>
#include <pollingservice.h>
#include <uidatarepository.h>

namespace {
QString startupLogPath()
{
    return QDir(QCoreApplication::applicationDirPath()).filePath("saturn_startup.log");
}

void appendStartupLog(const QString &line)
{
    QFile file(startupLogPath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        return;
    }
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << QDateTime::currentDateTimeUtc().toString(Qt::ISODate) << " " << line << "\n";
}

void qtMessageLogger(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    QString level = "DEBUG";
    switch (type) {
    case QtDebugMsg:
        level = "DEBUG";
        break;
    case QtInfoMsg:
        level = "INFO";
        break;
    case QtWarningMsg:
        level = "WARN";
        break;
    case QtCriticalMsg:
        level = "CRIT";
        break;
    case QtFatalMsg:
        level = "FATAL";
        break;
    }
    appendStartupLog(QStringLiteral("qt %1: %2").arg(level, msg));
}
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QSurfaceFormat surfaceFormat;
    surfaceFormat.setRenderableType(QSurfaceFormat::OpenGL);
    surfaceFormat.setProfile(QSurfaceFormat::NoProfile);
    surfaceFormat.setVersion(2, 1);
    surfaceFormat.setDepthBufferSize(24);
    surfaceFormat.setStencilBufferSize(8);
    QSurfaceFormat::setDefaultFormat(surfaceFormat);

    qputenv("QSG_RHI_BACKEND", "opengl");
    qputenv("QT_OPENGL", "desktop");
    qputenv("QT_QUICK_CONTROLS_STYLE", "Basic");
    qputenv("QML_DISABLE_DISK_CACHE", "1");
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QGuiApplication app(argc, argv);
    qInstallMessageHandler(qtMessageLogger);
    QGuiApplication::setOrganizationName("vka");
    QGuiApplication::setApplicationName("saturn");

    const QString appDir = QCoreApplication::applicationDirPath();
    QCoreApplication::addLibraryPath(appDir);
    QCoreApplication::addLibraryPath(QDir(appDir).filePath("plugins"));

    qmlRegisterSingletonType<DataAccess>("Saturn.Backend", 1, 0, "Database", DataAccess::qmlSingleton);
    qmlRegisterSingletonType<AuthManager>("Saturn.Backend", 1, 0, "Auth", AuthManager::qmlSingleton);
    qmlRegisterSingletonType<DbConnectionSettings>("Saturn.Backend", 1, 0, "DbSettings", DbConnectionSettings::qmlSingleton);
    qmlRegisterSingletonType<EventsRepository>("Saturn.Backend", 1, 0, "EventsRepo", EventsRepository::qmlSingleton);
    qmlRegisterSingletonType<GeometryRepository>("Saturn.Backend", 1, 0, "GeometryRepo", GeometryRepository::qmlSingleton);
    qmlRegisterSingletonType<GeoJsonService>("Saturn.Backend", 1, 0, "GeoJson", GeoJsonService::qmlSingleton);
    qmlRegisterSingletonType<GeoJsonTileFeedService>("Saturn.Backend", 1, 0, "TileFeed", GeoJsonTileFeedService::qmlSingleton);
    qmlRegisterSingletonType<MapObjectsRepository>("Saturn.Backend", 1, 0, "MapObjectsRepo", MapObjectsRepository::qmlSingleton);
    qmlRegisterSingletonType<MapEditingService>("Saturn.Backend", 1, 0, "MapEditing", MapEditingService::qmlSingleton);
    qmlRegisterSingletonType<MapGeometryRolesService>("Saturn.Backend", 1, 0, "MapGeometryRoles", MapGeometryRolesService::qmlSingleton);
    qmlRegisterSingletonType<MapRuntimeService>("Saturn.Backend", 1, 0, "MapRuntime", MapRuntimeService::qmlSingleton);
    qmlRegisterSingletonType<MapSnapshotService>("Saturn.Backend", 1, 0, "MapSnapshot", MapSnapshotService::qmlSingleton);
    qmlRegisterSingletonType<LocalGeoJsonRepository>("Saturn.Backend", 1, 0, "LocalGeoJsonRepo", LocalGeoJsonRepository::qmlSingleton);
    qmlRegisterSingletonType<ObjectDetailsRepository>("Saturn.Backend", 1, 0, "ObjectDetailsRepo", ObjectDetailsRepository::qmlSingleton);
    qmlRegisterSingletonType<PersonnelRepository>("Saturn.Backend", 1, 0, "PersonnelRepo", PersonnelRepository::qmlSingleton);
    qmlRegisterSingletonType<PollingService>("Saturn.Backend", 1, 0, "Polling", PollingService::qmlSingleton);
    qmlRegisterSingletonType<UiDataRepository>("Saturn.Backend", 1, 0, "UiDataRepo", UiDataRepository::qmlSingleton);
    GeoJsonTileFeedService::instance();

    // Keep legacy behavior: all backend services use persisted DB settings immediately on startup.
    DbConnectionSettings *dbSettings = DbConnectionSettings::instance();
    dbSettings->load();
    DataAccess *dbAccess = DataAccess::instance();
    dbAccess->setDriver(dbSettings->driver());
    dbAccess->setHost(dbSettings->host());
    dbAccess->setPort(dbSettings->port());
    dbAccess->setDbName(dbSettings->dbName());
    dbAccess->setUser(dbSettings->user());
    dbAccess->setPassword(dbSettings->password());

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/AppShell.qml"));
    QFile::remove(startupLogPath());
    appendStartupLog("startup begin");
    appendStartupLog("load url: " + url.toString());
    appendStartupLog("qt library paths: " + QCoreApplication::libraryPaths().join(";"));

    QObject::connect(&app, &QCoreApplication::aboutToQuit, []() {
        appendStartupLog("aboutToQuit emitted");
    });
    QObject::connect(&app, &QGuiApplication::lastWindowClosed, []() {
        appendStartupLog("lastWindowClosed emitted");
    });

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::warnings,
        &app,
        [](const QList<QQmlError> &warnings) {
            for (const QQmlError &warning : warnings) {
                appendStartupLog("qml warning: " + warning.toString());
            }
        },
        Qt::DirectConnection);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) {
                appendStartupLog("object creation failed for: " + objUrl.toString());
                QCoreApplication::exit(-1);
                return;
            }
            appendStartupLog("root object created: " + objUrl.toString());
        },
        Qt::QueuedConnection);

    engine.load(url);
    if (engine.rootObjects().isEmpty()) {
        appendStartupLog("engine rootObjects is empty after load");
    } else {
        appendStartupLog("engine rootObjects count: " + QString::number(engine.rootObjects().size()));
    }
    const int execResult = app.exec();
    appendStartupLog("app.exec() returned: " + QString::number(execResult));
    return execResult;
}
