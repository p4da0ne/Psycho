cmake_minimum_required(VERSION 3.16)
project(searchengine VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Gui Sql)

qt_standard_project_setup()

qt_add_library(searchengine
    config_searchengine.h
    searchengine.cpp searchengine.h
    searchresultrmodel.cpp searchresultrmodel.h
)
target_include_directories(searchengine PUBLIC
    ../include
    /
)

target_compile_definitions(searchengine PRIVATE
    __USE_DLL_SEARCHENGINE
)

target_link_libraries(searchengine PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Sql
)

install(TARGETS searchengine
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
