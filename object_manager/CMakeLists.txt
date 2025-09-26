cmake_minimum_required(VERSION 3.16)
project(object_manager VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS AxContainer Core5Compat Gui PrintSupport Sql Widgets)

qt_standard_project_setup()

qt_add_library(object_manager
    Calculate_K_omkrf.cpp Calculate_K_omkrf.h
    People_Losses.cpp People_Losses.h
    add_elements_dialog.cpp add_elements_dialog.h
    add_object.cpp add_object.h add_object.ui
    calculating_mps.cpp calculating_mps.h
    comboboxdelegat.cpp comboboxdelegat.h
    config_object_manager.h
    mylabel.cpp mylabel.h
    mysqlrelationaldelegate.cpp mysqlrelationaldelegate.h
    object_manager.cpp object_manager.h
    object_manager_form.ui
    persones_info.cpp persones_info.h
    persones_info_form.ui
    personesdata.cpp personesdata.h
    personesdata_form.ui
    simledelegate.cpp simledelegate.h
    textprinter.cpp textprinter.h
)
target_include_directories(object_manager PUBLIC
    .
    ../include
    ../my_mapaccess
    ../output
    ../reports
)

target_compile_definitions(object_manager PRIVATE
    BUILD_DLL
    __USE_DLL_XXXXX
)

target_link_libraries(object_manager PUBLIC
    Qt::AxContainer
    Qt::Core
    Qt::Core5Compat
    Qt::Gui
    Qt::PrintSupport
    Qt::Sql
    Qt::Widgets
    my_mapaccess
    output
    reports
)


# Resources:
set(object_manager_resource_files
    "Resources/0.png"
    "Resources/01.png"
    "Resources/02.png"
    "Resources/03.png"
    "Resources/Stop2.png"
    "Resources/add.png"
    "Resources/add_but.png"
    "Resources/book.png"
    "Resources/change_user.png"
    "Resources/close.png"
    "Resources/connect_saturn.png"
    "Resources/delete_but.png"
    "Resources/edit_1.png"
    "Resources/edit_but.png"
    "Resources/group.png"
    "Resources/open.png"
    "Resources/planet.png"
    "Resources/printer.png"
    "Resources/saturn_delete.png"
    "Resources/user_config.png"
    "Resources/weapon.png"
)

qt_add_resources(object_manager "object_manager"
    PREFIX
        "/"
    FILES
        ${object_manager_resource_files}
)

install(TARGETS object_manager
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
