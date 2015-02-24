#-------------------------------------------------
#
# Project created by QtCreator 2015-02-05T20:01:10
#
#-------------------------------------------------

QT       += core gui svg

QT += script scripttools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SoCConf
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    scenemanager.cpp \
    diagramelements.cpp \
    sceneview.cpp \
    dialogpinconfig.cpp \
    socinfo.cpp \
    aboutdialog.cpp

HEADERS  += \
    mainwindow.h \
    scenemanager.h \
    diagramelements.h \
    sceneview.h \
    dialogpinconfig.h \
    socinfo.h \
    aboutdialog.h

FORMS    += \
    mainwindow.ui \
    aboutdialog.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    examples/lpc4337fbd144.chip \
    examples/rz-a1l.chip \
    examples/stm32f407Zx.chip
