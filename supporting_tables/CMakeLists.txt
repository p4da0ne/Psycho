cmake_minimum_required(VERSION 3.16)
project(supporting_tables VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core5Compat Gui Network Sql Widgets)

qt_standard_project_setup()

qt_add_library(supporting_tables
    config_supporting_tables.h
    supporting_tables.cpp supporting_tables.h
    supporting_tables_form.ui
)
target_compile_definitions(supporting_tables PRIVATE
    __USE_DLL_XXXXX
)

target_link_libraries(supporting_tables PUBLIC
    Qt::Core
    Qt::Core5Compat
    Qt::Gui
    Qt::Network
    Qt::Sql
    Qt::Widgets
    dataaccess
    db_saturn
    my_mapaccess
)

if(WIN32)
    target_include_directories(supporting_tables PUBLIC
        ../dataaccess
        ../db_saturn
        ../include
        ../my_mapaccess
    )
endif()

install(TARGETS supporting_tables
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
