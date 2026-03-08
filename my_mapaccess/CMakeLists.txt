cmake_minimum_required(VERSION 3.16)
project(my_mapaccess VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Gui)

qt_standard_project_setup()

qt_add_library(my_mapaccess
    config_my_mapaccess.h
    my_mapaccess.cpp my_mapaccess.h
)
target_include_directories(my_mapaccess PUBLIC
    ../include
)

target_compile_definitions(my_mapaccess PRIVATE
    BUILD_DLL
    __USE_DLL_MY_MAPACCESS
)

target_link_libraries(my_mapaccess PUBLIC
    Qt::Core
    Qt::Gui
)

install(TARGETS my_mapaccess
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
