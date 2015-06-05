TEMPLATE = lib
TARGET = users_manager

QT	+= gui sql

DESTDIR = ../build

INCLUDEPATH += . \

DEFINES += __USE_DLL_USERS_MANAGER

include(users_manager.pri)