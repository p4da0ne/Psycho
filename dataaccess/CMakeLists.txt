cmake_minimum_required(VERSION 3.16)
project(dataaccess VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Gui Sql Qml Quick)

qt_standard_project_setup()

qt_add_library(dataaccess
    authmanager.cpp authmanager.h
    config_dataaccess.h
    dataaccess.cpp dataaccess.h
    dbconnectionsettings.cpp dbconnectionsettings.h
    eventsrepository.cpp eventsrepository.h
    geometryrepository.cpp geometryrepository.h
    geojsonservice.cpp geojsonservice.h
    legacycalculationservice.cpp legacycalculationservice.h
    mapobjectsrepository.cpp mapobjectsrepository.h
    mapeditingservice.cpp mapeditingservice.h
    mapgeometryrolesservice.cpp mapgeometryrolesservice.h
    mapruntimeservice.cpp mapruntimeservice.h
    mapsnapshotservice.cpp mapsnapshotservice.h
    objectdetailsrepository.cpp objectdetailsrepository.h
    personnelrepository.cpp personnelrepository.h
    pollingservice.cpp pollingservice.h
)
target_compile_definitions(dataaccess PRIVATE
    DATAACCESS_BUILD_DLL
)

target_link_libraries(dataaccess PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
    Qt::Sql
)

install(TARGETS dataaccess
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
