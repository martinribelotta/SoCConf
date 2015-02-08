#-------------------------------------------------
#
# Project created by QtCreator 2015-02-05T20:01:10
#
#-------------------------------------------------

QT       += core gui svg script scripttools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SoCConf
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    scenemanager.cpp \
    diagramelements.cpp \
    sceneview.cpp \
    dialogpinconfig.cpp \
    socinfo.cpp

HEADERS  += \
    mainwindow.h \
    scenemanager.h \
    diagramelements.h \
    sceneview.h \
    dialogpinconfig.h \
    socinfo.h

FORMS    += \
    mainwindow.ui \
    dialogpinconfig.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    test.chip
