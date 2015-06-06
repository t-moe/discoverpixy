#-------------------------------------------------
#
# Project created by QtCreator 2015-04-02T22:09:26
#
#-------------------------------------------------

QT       += widgets gui

TARGET = emulatorqt
TEMPLATE = lib
CONFIG += staticlib debug

SOURCES += \
    mainwindow.cpp \
    main.cpp \
    ll_tft.cpp \
    ll_system.cpp \
    ll_touch.cpp \
    ll_filesystem.cpp
HEADERS += \
    mainwindow.h \


INCLUDEPATH+= ../../common/lowlevel/ \
              ../../common/touch/ \
              ../../common/tft/ \
              ../../common/filesystem


FORMS += \
    mainwindow.ui

DESTDIR = $$_PRO_FILE_PWD_ #force windows to not create subfolders

#QMAKE_CXXFLAGS+= -v
