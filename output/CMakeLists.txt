cmake_minimum_required(VERSION 3.16)
project(output VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Gui Sql)

qt_standard_project_setup()

qt_add_library(output
    config_output.h
    output.cpp output.h
)
target_include_directories(output PUBLIC
    .
)

target_compile_definitions(output PRIVATE
    __USE_DLL_XXXXX
)

target_link_libraries(output PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Sql
)

install(TARGETS output
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
