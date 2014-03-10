#-------------------------------------------------
#
# Project created by QtCreator 2014-02-21T08:04:41
#
#-------------------------------------------------

CONFIG += c++11
QT       += core gui sql network
LIBS += -lavcodec -lavformat -lswscale -lavutil

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tagu
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    delegates/ThumbnailDelegate.cpp \
    Worker.cpp \
    dialogs/TagsDialog.cpp \
    dialogs/SignupDialog.cpp \
    dialogs/ConfigDialog.cpp \
    FFMpeg.cpp \
    Config.cpp \
    delegates/ActressDelegate.cpp \
    delegates/ScreenshotDelegate.cpp \
    delegates/CoverDelegate.cpp \
    models/VideoModel.cpp \
    models/VidsModel.cpp

HEADERS  += MainWindow.hpp \
    delegates/ThumbnailDelegate.hpp \
    Worker.hpp \
    dialogs/TagsDialog.hpp \
    dialogs/SignupDialog.hpp \
    dialogs/ConfigDialog.hpp \
    Utils.hpp \
    FFMpeg.hpp \
    Config.hpp \
    delegates/ActressDelegate.hpp \
    delegates/ScreenshotDelegate.hpp \
    delegates/CoverDelegate.hpp \
    delegates/Delegates.hpp \
    models/VideoModel.hpp \
    models/VidsModel.hpp

FORMS    += MainWindow.ui \
    SettingsDialog.ui \
    dialogs/TagsDialog.ui \
    dialogs/SignupDialog.ui \
    dialogs/ConfigDialog.ui
