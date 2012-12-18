#-------------------------------------------------
#
# Project created by QtCreator 2012-12-11T23:14:02
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = weatherProj_vol1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    plots.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    plots.h

FORMS    += mainwindow.ui \
    plots.ui
