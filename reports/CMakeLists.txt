cmake_minimum_required(VERSION 3.16)
project(reports VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Core5Compat Gui PrintSupport Sql Widgets)

qt_standard_project_setup()

qt_add_library(reports
    config_reports.h
    report_data.cpp report_data.h
    report_view.cpp report_view.h
    reports.cpp reports.h
    textprinter.cpp textprinter.h
)
target_include_directories(reports PUBLIC
    .
    ../output
    ../textedit
)

target_compile_definitions(reports PRIVATE
    __USE_DLL_REPORTS
)

target_link_libraries(reports PUBLIC
    Qt::Core
    Qt::Core5Compat
    Qt::Gui
    Qt::PrintSupport
    Qt::Sql
    Qt::Widgets
    output
    textedit
)

install(TARGETS reports
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
