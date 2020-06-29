#-------------------------------------------------
#
# Project created by QtCreator 2016-11-25T15:40:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Cassandra
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    BOP_category.cpp \
    BOP_database.cpp \
    BOP_main.cpp \
    BOP_manager.cpp \
    BOP_movie.cpp

HEADERS  += mainwindow.h \
    BOP_category.h \
    BOP_database.h \
    BOP_manager.h \
    BOP_movie.h

FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES += \
    resources.qrc

CONFIG += qt

QT += svg
