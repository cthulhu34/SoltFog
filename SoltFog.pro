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
    mdvvmodel.cpp \
    mva8model.cpp \
    dataminer.cpp \
    digitkeyboard.cpp \
    plotter.cpp

HEADERS  += mainwindow.h \
    mdvvmodel.h \
    mva8model.h \
    dataminer.h \
    digitkeyboard.h \
    plotter.h

FORMS    += mainwindow.ui \
    digitkeyboard.ui \
    plotter.ui
