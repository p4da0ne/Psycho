cmake_minimum_required(VERSION 3.16)
project(mapview VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Core5Compat Gui PrintSupport Sql Widgets)
find_package(QMapLibre REQUIRED COMPONENTS Core Location)

qt_standard_project_setup()

qt_add_library(mapview
    calculatingClasses/Calculate_K_omkrf.cpp calculatingClasses/Calculate_K_omkrf.h
    calculatingClasses/People_Losses.cpp calculatingClasses/People_Losses.h
    calculatingClasses/calculating_mps.cpp calculatingClasses/calculating_mps.h
    calculatingClasses/formationsMPS.cpp calculatingClasses/formationsMPS.h
    calculatingClasses/formationsPsiLooses.cpp calculatingClasses/formationsPsiLooses.h
    calculatingClasses/regions_mpos.cpp calculatingClasses/regions_mpos.h
    change_coord_dialog.cpp change_coord_dialog.h
    config_mapview.h
    events_map_model.cpp events_map_model.h
    mapscroll.cpp mapscroll.h
    mapview.cpp mapview.h
    move_model.cpp move_model.h
    signdata.cpp signdata.h
    view_manage.cpp view_manage.h
    MapViewNew.cpp MapViewNew.h
)

qt_add_qml_module(mapview
    URI mapview
    VERSION 1.0
    QML_FILES
        MapView.qml
)

target_include_directories(mapview PUBLIC
    ../coord
    ../event_manager
    ../eventsmodel
    ../include
    ../my_mapaccess
    ../reports
    ../searchengine
    /
    calculatingClasses
)

target_compile_definitions(mapview PRIVATE
    BUILD_DLL
    __USE_DLL_XXXXX
)

target_link_libraries(mapview PUBLIC
    Qt::Core
    Qt::Core5Compat
    Qt::Gui
    Qt::PrintSupport
    Qt::Sql
    Qt::Widgets
    Qt6::Qml
    Qt6::Quick
    Qt6::Location
    Qt6::Positioning
    QMapLibre::Core
    QMapLibre::Location
    coord
    event_manager
    eventsmodel
    my_mapaccess
    reports
    searchengine
)

qmaplibre_location_setup_plugins(mapview)

# Resources:
set(mapview_resource_files
    "Resources/contrast-down.png"
    "Resources/contrast-up.png"
    "Resources/down_bright.png"
    "Resources/edit_1.png"
    "Resources/event-search.png"
    "Resources/foto.png"
    "Resources/greate_scale.jpg"
    "Resources/less_scale.jpg"
    "Resources/map_close.png"
    "Resources/map_open.png"
    "Resources/map_search.png"
    "Resources/mapwork.png"
    "Resources/no_photo1.png"
    "Resources/print.png"
    "Resources/print_part.png"
    "Resources/search.png"
    "Resources/up_bright.png"
)

qt_add_resources(mapview "mapview"
    PREFIX
        "/"
    FILES
        ${mapview_resource_files}
)

install(TARGETS mapview
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
