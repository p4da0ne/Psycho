cmake_minimum_required(VERSION 3.16)
project(saturn VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS AxContainer Core5Compat Gui Network PrintSupport Qml Quick Sql Widgets OpenGLWidgets)

qt_standard_project_setup()

qt_add_executable(saturn WIN32 MACOSX_BUNDLE
    dbbackup.cpp dbbackup.h
    main.cpp
    main_form.ui
    mainform.cpp mainform.h
    mapsettingsdialog.cpp mapsettingsdialog.h
    mymdiarea.cpp mymdiarea.h
    settings.cpp settings.h
    main.qml
    LoginScreen.qml
    DBConnectScreen.qml
)
target_compile_definitions(saturn PRIVATE
    BUILD_DLL
)
target_compile_definitions(saturn PUBLIC
    NOMINMAX
)

target_link_directories(saturn PRIVATE
    ../build
)

target_link_libraries(saturn PRIVATE
    Qt::AxContainer
    Qt::Core
    Qt::Core5Compat
    Qt::Gui
    Qt::Network
    Qt::PrintSupport
    Qt::Qml
    Qt::Quick
    Qt::Sql
    Qt::Widgets
    Qt6::OpenGLWidgets
    dataaccess
    db_saturn
    event_manager
    mapview
    my_mapaccess
    object_manager
    output
    reports
    signs_edit
    supporting_tables
    users_manager
)


# Resources:
set(saturn_resource_files
    "Resources/01.ico"
    "Resources/RedStar.ico"
    "Resources/book.png"
    "Resources/change_user.png"
    "Resources/database.png"
    "Resources/db_settings.png"
    "Resources/enter.png"
    "Resources/exit.png"
    "Resources/mapwork.png"
    "Resources/metacontact.png"
    "Resources/planet.png"
    "Resources/saturn.png"
    "Resources/user_config.png"
)

qt_add_resources(saturn "saturn"
    PREFIX
        "/"
    FILES
        ${saturn_resource_files}
)

if(WIN32)
    target_include_directories(saturn PRIVATE
        .
        ../dataaccess
        ../db_saturn
        ../event_manager
        ../include
        ../mapview
        ../my_mapaccess
        ../object_manager
        ../output
        ../reports
        ../signs_edit
        ../supporting_tables
        ../users_manager
    )
endif()

install(TARGETS saturn
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET saturn
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
