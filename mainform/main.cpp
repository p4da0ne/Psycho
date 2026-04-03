#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <authmanager.h>
#include <dataaccess.h>
#include <dbconnectionsettings.h>
#include <eventsrepository.h>
#include <geometryrepository.h>
#include <geojsonservice.h>
#include <mapobjectsrepository.h>
#include <mapeditingservice.h>
#include <mapgeometryrolesservice.h>
#include <mapruntimeservice.h>
#include <mapsnapshotservice.h>
#include <objectdetailsrepository.h>
#include <personnelrepository.h>
#include <pollingservice.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QGuiApplication::setOrganizationName("vka");
    QGuiApplication::setApplicationName("saturn");

    qmlRegisterSingletonType<DataAccess>("Saturn.Backend", 1, 0, "Database", DataAccess::qmlSingleton);
    qmlRegisterSingletonType<AuthManager>("Saturn.Backend", 1, 0, "Auth", AuthManager::qmlSingleton);
    qmlRegisterSingletonType<DbConnectionSettings>("Saturn.Backend", 1, 0, "DbSettings", DbConnectionSettings::qmlSingleton);
    qmlRegisterSingletonType<EventsRepository>("Saturn.Backend", 1, 0, "EventsRepo", EventsRepository::qmlSingleton);
    qmlRegisterSingletonType<GeometryRepository>("Saturn.Backend", 1, 0, "GeometryRepo", GeometryRepository::qmlSingleton);
    qmlRegisterSingletonType<GeoJsonService>("Saturn.Backend", 1, 0, "GeoJson", GeoJsonService::qmlSingleton);
    qmlRegisterSingletonType<MapObjectsRepository>("Saturn.Backend", 1, 0, "MapObjectsRepo", MapObjectsRepository::qmlSingleton);
    qmlRegisterSingletonType<MapEditingService>("Saturn.Backend", 1, 0, "MapEditing", MapEditingService::qmlSingleton);
    qmlRegisterSingletonType<MapGeometryRolesService>("Saturn.Backend", 1, 0, "MapGeometryRoles", MapGeometryRolesService::qmlSingleton);
    qmlRegisterSingletonType<MapRuntimeService>("Saturn.Backend", 1, 0, "MapRuntime", MapRuntimeService::qmlSingleton);
    qmlRegisterSingletonType<MapSnapshotService>("Saturn.Backend", 1, 0, "MapSnapshot", MapSnapshotService::qmlSingleton);
    qmlRegisterSingletonType<ObjectDetailsRepository>("Saturn.Backend", 1, 0, "ObjectDetailsRepo", ObjectDetailsRepository::qmlSingleton);
    qmlRegisterSingletonType<PersonnelRepository>("Saturn.Backend", 1, 0, "PersonnelRepo", PersonnelRepository::qmlSingleton);
    qmlRegisterSingletonType<PollingService>("Saturn.Backend", 1, 0, "Polling", PollingService::qmlSingleton);

    QQmlApplicationEngine engine;
    // TEMPORARY(TEST): main.qml currently acts as technical backend validation shell.
    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) {
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);

    engine.load(url);
    return app.exec();
}
