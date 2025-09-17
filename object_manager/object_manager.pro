TEMPLATE = lib
TARGET = object_manager

QT += core sql widgets gui axcontainer core5compat printsupport
QMAKE_LIBDIR = "C:/Qt/6.7.3/llvm-mingw_64/lib"

DESTDIR = ../build

RESOURCES       += object_manager.qrc

INCLUDEPATH += . \
                ../include \
                ../my_mapaccess \
                ../reports \
                ../output


DEPENDPATH +=  . \
                ../my_mapaccess \
                ../output \
                ../reports


LIBS += -L../build -lreports -loutput -lmy_mapaccess

DEFINES += __USE_DLL_XXXXX
DEFINES += BUILD_DLL

include(object_manager.pri)

HEADERS +=

SOURCES +=

FORMS +=

