cmake_minimum_required(VERSION 3.16)
project(db_saturn VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Gui Sql)

qt_standard_project_setup()

qt_add_library(db_saturn
    config_db_saturn.h
    db_saturn.cpp db_saturn.h
    db_struct.cpp db_struct.h
)
target_include_directories(db_saturn PUBLIC
    ../dataaccess
)

target_compile_definitions(db_saturn PRIVATE
    __USE_DLL_XXXXX
)

target_link_libraries(db_saturn PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Sql
    dataaccess
)

install(TARGETS db_saturn
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
