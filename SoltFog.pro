#-------------------------------------------------
#
# Project created by QtCreator 2016-08-08T10:10:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts serialbus serialport

TARGET = SoltFog
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    dataminer.cpp \
    digitkeyboard.cpp \
    plotter.cpp \
    devicecontroller.cpp \
    logiccontroller.cpp \
    IODevices/mdvvmodel.cpp \
    IODevices/mva8model.cpp \
    IODevices/lumelp18.cpp

HEADERS  += mainwindow.h \
    dataminer.h \
    digitkeyboard.h \
    plotter.h \
    devicecontroller.h \
    logiccontroller.h \
    IODevices/mdvvmodel.h \
    IODevices/mva8model.h \
    IODevices/lumelp18.h

FORMS    += mainwindow.ui \
    digitkeyboard.ui \
    plotter.ui
