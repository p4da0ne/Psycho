cmake_minimum_required(VERSION 3.16)
project(dataaccess VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Gui Sql)

qt_standard_project_setup()

qt_add_library(dataaccess
    config_dataaccess.h
    dataaccess.cpp dataaccess.h
)
target_compile_definitions(dataaccess PRIVATE
    BUILD_DLL
    __USE_DLL_XXXXX
)

target_link_libraries(dataaccess PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Sql
    Qt::Quick
)

install(TARGETS dataaccess
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
