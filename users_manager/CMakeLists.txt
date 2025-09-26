cmake_minimum_required(VERSION 3.16)
project(users_manager VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Gui Sql Widgets)

qt_standard_project_setup()

qt_add_library(users_manager
    config_users_manager.h
    password_dialog.cpp password_dialog.h
    user_data_dialog.cpp user_data_dialog.h
    users_manager.cpp users_manager.h
)
target_include_directories(users_manager PUBLIC
    .
)

target_compile_definitions(users_manager PRIVATE
    __USE_DLL_USERS_MANAGER
)

target_link_libraries(users_manager PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Sql
    Qt::Widgets
)


# Resources:
set(users_manager_resource_files
    "Resources/add_but.png"
    "Resources/delete_but.png"
    "Resources/edit_but.png"
    "Resources/stock_lock.png"
)

qt_add_resources(users_manager "users_manager"
    PREFIX
        "/"
    FILES
        ${users_manager_resource_files}
)

install(TARGETS users_manager
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
