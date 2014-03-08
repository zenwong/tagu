#-------------------------------------------------
#
# Project created by QtCreator 2014-02-21T08:04:41
#
#-------------------------------------------------

CONFIG += c++11
QT       += core gui sql network
LIBS += -lffmpegthumbnailer -lavcodec -lavformat -lswscale -lavutil

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tagu
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    delegates/ThumbnailDelegate.cpp \
    Worker.cpp \
    SettingsDialog.cpp \
    dialogs/TagsDialog.cpp \
    dialogs/SignupDialog.cpp \
    Settings.cpp \
    Thumbnailer.cpp \
    FFMpeg.cpp

HEADERS  += MainWindow.hpp \
    delegates/ThumbnailDelegate.hpp \
    Worker.hpp \
    SettingsDialog.hpp \
    Globals.hpp \
    dialogs/TagsDialog.hpp \
    dialogs/SignupDialog.hpp \
    Utils.hpp \
    Settings.hpp \
    Thumbnailer.hpp \
    FFMpeg.hpp

FORMS    += MainWindow.ui \
    SettingsDialog.ui \
    dialogs/TagsDialog.ui \
    dialogs/SignupDialog.ui
