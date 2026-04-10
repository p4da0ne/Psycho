cmake_minimum_required(VERSION 3.16)
project(saturn VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS AxContainer Core5Compat Gui Network PrintSupport Sql Widgets)
find_package(QMapLibre REQUIRED COMPONENTS Core Location)

qt_standard_project_setup()

add_subdirectory(my_mapaccess)
add_subdirectory(coord)
add_subdirectory(users_manager)
add_subdirectory(output)
add_subdirectory(textedit)
add_subdirectory(reports)
add_subdirectory(dataaccess)
add_subdirectory(db_saturn)
add_subdirectory(supporting_tables)
add_subdirectory(object_manager)
add_subdirectory(signs_edit)
add_subdirectory(eventsmodel)
add_subdirectory(searchengine)
add_subdirectory(mapview)
add_subdirectory(event_manager)
add_subdirectory(mainform)

find_program(PWSH_EXECUTABLE NAMES pwsh powershell REQUIRED)
add_custom_target(quality-check
    COMMAND "${PWSH_EXECUTABLE}" -NoProfile -ExecutionPolicy Bypass
        -File "${CMAKE_SOURCE_DIR}/tools/repo_quality_gate.ps1"
        -RepoRoot "${CMAKE_SOURCE_DIR}"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    COMMENT "Running repository quality gate"
)
