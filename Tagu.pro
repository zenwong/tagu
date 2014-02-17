#-------------------------------------------------
#
# Project created by QtCreator 2014-02-11T14:24:55
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -std=c++11
QT       += core gui sql

LIBS += -lsqlite3

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tagu
TEMPLATE = app

SOURCES += main.cpp\
        MainWindow.cpp \
    MultiCompleter.cpp \
    SettingsDialog.cpp \
    ImportThread.cpp \
    DbHelper.cpp \
    FilesListView.cpp

HEADERS  += MainWindow.hpp \
    MultiCompleter.hpp \
    SettingsDialog.hpp \
    Utils.hpp \
    ImportThread.hpp \
    DbHelper.hpp \
    FilesListView.hpp

FORMS    += MainWindow.ui \
    SettingsDialog.ui

include(QtAwesome/QtAwesome/QtAwesome.pri)
