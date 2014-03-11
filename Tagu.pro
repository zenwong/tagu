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
    delegates/ActressDelegate.cpp \
    delegates/ScreenshotDelegate.cpp \
    delegates/CoverDelegate.cpp \
    models/VidsModel.cpp \
    models/Options.cpp

HEADERS  += MainWindow.hpp \
    delegates/ThumbnailDelegate.hpp \
    Worker.hpp \
    dialogs/TagsDialog.hpp \
    dialogs/SignupDialog.hpp \
    dialogs/ConfigDialog.hpp \
    Utils.hpp \
    FFMpeg.hpp \
    delegates/ActressDelegate.hpp \
    delegates/ScreenshotDelegate.hpp \
    delegates/CoverDelegate.hpp \
    delegates/Delegates.hpp \
    models/VidsModel.hpp \
    models/Options.hpp

FORMS    += MainWindow.ui \
    dialogs/TagsDialog.ui \
    dialogs/SignupDialog.ui \
    dialogs/ConfigDialog.ui
