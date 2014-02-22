#-------------------------------------------------
#
# Project created by QtCreator 2014-02-21T08:04:41
#
#-------------------------------------------------

CONFIG += c++11
QT       += core gui sql network
LIBS += -lffmpegthumbnailer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tagu
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    delegates/ThumbnailDelegate.cpp \
    Worker.cpp \
    SettingsDialog.cpp \
    QtWebsocket/functions.cpp \
    QtWebsocket/QWsFrame.cpp \
    QtWebsocket/QWsHandshake.cpp \
    QtWebsocket/QWsSocket.cpp

HEADERS  += MainWindow.hpp \
    delegates/ThumbnailDelegate.hpp \
    Worker.hpp \
    SettingsDialog.hpp \
    Globals.hpp \
    QtWebsocket/functions.h \
    QtWebsocket/QWsFrame.h \
    QtWebsocket/QWsHandshake.h \
    QtWebsocket/QWsSocket.h \
    QtWebsocket/WsEnums.h

FORMS    += MainWindow.ui \
    SettingsDialog.ui
