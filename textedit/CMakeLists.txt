cmake_minimum_required(VERSION 3.16)
project(textedit VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Core5Compat Gui PrintSupport Widgets)

qt_standard_project_setup()

qt_add_library(textedit
    config_textedit.h
    textedit.cpp textedit.h
)
target_compile_definitions(textedit PRIVATE
    BUILD_DLL
    __USE_DLL_TEXTEDIT
)

target_link_libraries(textedit PUBLIC
    Qt::Core
    Qt::Core5Compat
    Qt::Gui
    Qt::PrintSupport
    Qt::Widgets
)


# Resources:
set(textedit_resource_files
    "images/win/editcopy.png"
    "images/win/editcut.png"
    "images/win/editpaste.png"
    "images/win/editredo.png"
    "images/win/editundo.png"
    "images/win/exportpdf.png"
    "images/win/filenew.png"
    "images/win/fileopen.png"
    "images/win/fileprint.png"
    "images/win/filesave.png"
    "images/win/textbold.png"
    "images/win/textcenter.png"
    "images/win/textitalic.png"
    "images/win/textjustify.png"
    "images/win/textleft.png"
    "images/win/textright.png"
    "images/win/textunder.png"
    "images/win/word_document.png"
    "images/win/zoomin.png"
    "images/win/zoomout.png"
)

qt_add_resources(textedit "textedit"
    PREFIX
        "/"
    FILES
        ${textedit_resource_files}
)

install(TARGETS textedit
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
