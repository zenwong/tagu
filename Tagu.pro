#-------------------------------------------------
#
# Project created by QtCreator 2014-02-11T14:24:55
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -std=c++11
QT       += core gui sql

LIBS += /usr/lib/libsqlite3.so /usr/lib/libboost_system.so /usr/lib/libboost_filesystem.so

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tagu
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    TableModel.cpp \
    HtmlDelegate.cpp \
    MultiCompleter.cpp \
    SettingsDialog.cpp

HEADERS  += MainWindow.hpp \
    TableModel.hpp \
    HtmlDelegate.hpp \
    MultiCompleter.hpp \
    SettingsDialog.hpp \
    Utils.hpp

FORMS    += MainWindow.ui \
    SettingsDialog.ui

include(QtAwesome/QtAwesome/QtAwesome.pri)
