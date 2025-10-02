cmake_minimum_required(VERSION 3.16)
project(event_manager VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Core5Compat Gui Sql Widgets)

qt_standard_project_setup()

qt_add_library(event_manager
    config_event_manager.h
    event_manager.cpp event_manager.h
    progressthread.cpp progressthread.h
)
target_include_directories(event_manager PUBLIC
    .
    ../coord
    ../eventsmodel
    ../include
)

target_compile_definitions(event_manager PRIVATE
    BUILD_DLL
    __USE_DLL_EVENT_MANAGER
)

target_link_libraries(event_manager PUBLIC
    Qt::Core
    Qt::Core5Compat
    Qt::Gui
    Qt::Sql
    Qt::Widgets
    coord
    eventsmodel
)


# Resources:
set(icons_resource_files
    "icons/add_but.png"
    "icons/close.png"
    "icons/grey.ico"
    "icons/magenta.ico"
    "icons/no_photo.png"
    "icons/red.ico"
    "icons/sync.ico"
    "icons/yellow.ico"
)

qt_add_resources(event_manager "icons"
    PREFIX
        "/icons"
    FILES
        ${icons_resource_files}
)

install(TARGETS event_manager
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
