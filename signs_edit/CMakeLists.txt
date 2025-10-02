cmake_minimum_required(VERSION 3.16)
project(signs_edit VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Gui Sql Widgets)

qt_standard_project_setup()

qt_add_library(signs_edit
    config_signs_edit.h
    signs_edit.cpp signs_edit.h signs_edit.ui
)
target_include_directories(signs_edit PUBLIC
    .
    ../include
    ../my_mapaccess
)

target_compile_definitions(signs_edit PRIVATE
    __USE_DLL_SIGNS_EDIT
)

target_link_libraries(signs_edit PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Sql
    Qt::Widgets
    my_mapaccess
)

install(TARGETS signs_edit
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
