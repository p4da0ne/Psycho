cmake_minimum_required(VERSION 3.16)
project(eventsmodel VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Gui Sql Widgets)

qt_standard_project_setup()

qt_add_library(eventsmodel
    config_eventsmodel.h
    event.cpp event.h
    event_object.cpp event_object.h
    eventsmodel.cpp eventsmodel.h
)
target_include_directories(eventsmodel PUBLIC
    ../coord
    ../include
)

target_compile_definitions(eventsmodel PRIVATE
    BUILD_DLL
    __USE_DLL_EVENTSMODEL
)

target_link_libraries(eventsmodel PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Sql
    Qt::Widgets
    coord
)


# Resources:
set(icons_resource_files
    "icons/grey.ico"
    "icons/magenta.ico"
    "icons/red.ico"
    "icons/yellow.ico"
)

qt_add_resources(eventsmodel "icons"
    PREFIX
        "/icons"
    FILES
        ${icons_resource_files}
)

install(TARGETS eventsmodel
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
