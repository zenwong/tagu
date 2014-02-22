#-------------------------------------------------
#
# Project created by QtCreator 2014-02-21T08:04:41
#
#-------------------------------------------------

CONFIG += c++11
QT       += core gui sql
LIBS += -lffmpegthumbnailer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tagu2
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    delegates/ThumbnailDelegate.cpp \
    Worker.cpp \
    SettingsDialog.cpp

HEADERS  += MainWindow.hpp \
    ImageDelegate.hpp \
    delegates/ThumbnailDelegate.hpp \
    Worker.hpp \
    SettingsDialog.hpp

FORMS    += MainWindow.ui \
    SettingsDialog.ui
