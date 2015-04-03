#-------------------------------------------------
#
# Project created by QtCreator 2015-04-02T22:09:26
#
#-------------------------------------------------

QT       += widgets gui

TARGET = emulatorqt
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    mainwindow.cpp \
    main.cpp \
    ll_tft.cpp
HEADERS += \
    mainwindow.h \


INCLUDEPATH+= ../../common/lowlevel/


FORMS += \
    mainwindow.ui

