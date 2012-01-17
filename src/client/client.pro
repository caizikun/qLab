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
    experimentdata.cpp

# SOURCES += ../qwt6/qwt_plot.cpp
HEADERS += mainwindow.h \
    plot.h \
    tcpclient.h \
    experimentdata.h

# HEADERS += ../qwt6/qwt_plot.h
FORMS += mainwindow.ui
INCLUDEPATH += ../qwt6/

# qwt 6.0.1 need to be compiled and installed to system path
LIBS += -lqwt
