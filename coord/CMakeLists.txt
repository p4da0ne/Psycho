cmake_minimum_required(VERSION 3.16)
project(coord VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Gui Sql)

qt_standard_project_setup()

qt_add_library(coord
    config_coord.h
    coord.cpp coord.h
    coordmodel.cpp coordmodel.h
)
target_include_directories(coord PUBLIC
    ../include
    /
)

target_compile_definitions(coord PRIVATE
    __USE_DLL_COORD
)

target_link_libraries(coord PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Sql
)

install(TARGETS coord
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
