cmake_minimum_required(VERSION 3.16)
project(saturn VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS AxContainer Core5Compat Gui Network PrintSupport Qml Quick Sql Widgets OpenGLWidgets)

qt_standard_project_setup()

file(GLOB MAINFORM_COMPONENT_QML RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" CONFIGURE_DEPENDS
    "components/*.qml"
)

set(MAINFORM_QML_FILES
    AppShell.qml
    main.qml
    LoginScreen.qml
    DBConnectScreen.qml
    ${MAINFORM_COMPONENT_QML}
)

qt_add_executable(saturn WIN32 MACOSX_BUNDLE
    main.cpp
    ${MAINFORM_QML_FILES}
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
    mapview
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

qt_add_resources(saturn "saturn_qml"
    BASE
        "${CMAKE_CURRENT_SOURCE_DIR}"
    PREFIX
        "/"
    FILES
        ${MAINFORM_QML_FILES}
)

if(WIN32)
    set(POSTGRESQL_RUNTIME_DIR "" CACHE PATH "Optional PostgreSQL bin directory containing libpq.dll")

    target_include_directories(saturn PRIVATE
        .
        ../dataaccess
        ../include
        ../mapview
    )

    # Post-build runtime deployment so saturn.exe starts directly from build dir.
    get_target_property(_qt_qmake_executable Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qt_qmake_executable}" DIRECTORY)
    set(_windeployqt_executable "${_qt_bin_dir}/windeployqt.exe")

    if(EXISTS "${_windeployqt_executable}")
        set(_deploy_mode_arg "$<$<CONFIG:Debug>:--debug>$<$<NOT:$<CONFIG:Debug>>:--release>")

        add_custom_command(TARGET saturn POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E make_directory "$<TARGET_FILE_DIR:saturn>/mapview"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "$<TARGET_FILE:mapviewplugin>"
                "$<TARGET_FILE_DIR:saturn>/mapview/"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "${CMAKE_SOURCE_DIR}/mapview/qmldir.runtime"
                "$<TARGET_FILE_DIR:saturn>/mapview/qmldir"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "${CMAKE_BINARY_DIR}/mapview/MyMapView.qml"
                "$<TARGET_FILE_DIR:saturn>/mapview/MyMapView.qml"
            COMMAND "${_windeployqt_executable}"
                --force
                --compiler-runtime
                --dir "$<TARGET_FILE_DIR:saturn>"
                --qmldir "${CMAKE_CURRENT_SOURCE_DIR}"
                --qmldir "${CMAKE_SOURCE_DIR}/mapview"
                ${_deploy_mode_arg}
                "$<TARGET_FILE:saturn>"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "$<$<CONFIG:Debug>:${_qt_bin_dir}/QMapLibred.dll>$<$<NOT:$<CONFIG:Debug>>:${_qt_bin_dir}/QMapLibre.dll>"
                "$<TARGET_FILE_DIR:saturn>/"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "$<$<CONFIG:Debug>:${_qt_bin_dir}/QMapLibreLocationd.dll>$<$<NOT:$<CONFIG:Debug>>:${_qt_bin_dir}/QMapLibreLocation.dll>"
                "$<TARGET_FILE_DIR:saturn>/"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "$<$<CONFIG:Debug>:${_qt_bin_dir}/QMapLibreWidgetsd.dll>$<$<NOT:$<CONFIG:Debug>>:${_qt_bin_dir}/QMapLibreWidgets.dll>"
                "$<TARGET_FILE_DIR:saturn>/"
            COMMAND "${CMAKE_COMMAND}" -E env
                "APP_DIR=$<TARGET_FILE_DIR:saturn>"
                "POSTGRESQL_RUNTIME_DIR=${POSTGRESQL_RUNTIME_DIR}"
                "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_SOURCE_DIR}/copy_postgresql_runtime.cmake"
            VERBATIM
            COMMAND_EXPAND_LISTS
            COMMENT "Deploying Qt/QML runtime, plugins, and module binaries for saturn.exe"
        )
    else()
        message(WARNING "windeployqt.exe not found near Qt6::qmake; post-build deployment disabled")
    endif()
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
