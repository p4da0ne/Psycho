TEMPLATE = lib
TARGET = users_manager

QT	+= gui sql

DESTDIR = ../build

INCLUDEPATH += . \

RESOURCES       += users_manager.qrc

DEFINES += __USE_DLL_USERS_MANAGER

include(users_manager.pri)