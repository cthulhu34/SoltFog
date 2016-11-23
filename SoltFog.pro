#-------------------------------------------------
#
# Project created by QtCreator 2016-08-08T10:10:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts serialbus serialport

TARGET = SoltFog
TEMPLATE = app

target.path = /home/pi/
INSTALLS = target


SOURCES += main.cpp\
    mainwindow.cpp \
    dataminer.cpp \
    digitkeyboard.cpp \
    plotter.cpp \
    devicecontroller.cpp \
    logiccontroller.cpp \
    IODevices/mdvvmodel.cpp \
    IODevices/mva8model.cpp \
    IODevices/lumelp18.cpp \
    plotoptions.cpp

HEADERS  += mainwindow.h \
    dataminer.h \
    digitkeyboard.h \
    plotter.h \
    devicecontroller.h \
    logiccontroller.h \
    IODevices/mdvvmodel.h \
    IODevices/mva8model.h \
    IODevices/lumelp18.h \
    wiringPi/wiringPi.h \
    plotoptions.h

FORMS    += mainwindow.ui \
    digitkeyboard.ui \
    plotter.ui \
    plotoptions.ui

INCLUDEPATH += wiringPi

QMAKE_LFLAGS += -lwiringPi -lpthread

RESOURCES += \
    res.qrc
