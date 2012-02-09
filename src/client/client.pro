# -------------------------------------------------
# Project created by QtCreator 2012-01-16T13:39:54
# -------------------------------------------------
QT += core \
    gui \
    network
TARGET = client
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    plot.cpp \
    tcpclient.cpp \
    experimentdata.cpp \
    dataviewwindow.cpp \
    seriesdata.cpp \
    scalemagnifier.cpp \
    scaleplotmagnifier.cpp \
    setupcurvesdialog.cpp \
    ColorBox.cpp \
    scaleplotpanner.cpp \
    tableview.cpp
HEADERS += mainwindow.h \
    plot.h \
    tcpclient.h \
    experimentdata.h \
    dataviewwindow.h \
    seriesdata.h \
    scalemagnifier.h \
    scaleplotmagnifier.h \
    setupcurvesdialog.h \
    ColorBox.h \
    scaleplotpanner.h \
    tableview.h
FORMS += mainwindow.ui \
    dataviewwindow.ui \
    setupcurvesdialog.ui
INCLUDEPATH += ../qwt6

# INCLUDEPATH += ../../../qwt/src
# qwt 6.0.1 need to be compiled and installed to system path
LIBS += -L../../../qwt/lib

# release mode and unix
LIBS += -lqwt
