#-------------------------------------------------
#
# Project created by QtCreator 2014-02-11T14:24:55
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -std=c++11
QT       += core gui sql network

LIBS += -lsqlite3

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tagu
TEMPLATE = app

SOURCES += main.cpp\
        MainWindow.cpp \
    MultiCompleter.cpp \
    SettingsDialog.cpp \
    ImportThread.cpp \
    Worker.cpp \
    QtWebsocket/functions.cpp \
    QtWebsocket/QTlsServer.cpp \
    QtWebsocket/QWsFrame.cpp \
    QtWebsocket/QWsHandshake.cpp \
    QtWebsocket/QWsServer.cpp \
    QtWebsocket/QWsSocket.cpp

HEADERS  += MainWindow.hpp \
    MultiCompleter.hpp \
    SettingsDialog.hpp \
    Utils.hpp \
    ImportThread.hpp \
    Worker.hpp \
    QtWebsocket/functions.h \
    QtWebsocket/QTlsServer.h \
    QtWebsocket/QWsFrame.h \
    QtWebsocket/QWsHandshake.h \
    QtWebsocket/QWsServer.h \
    QtWebsocket/QWsSocket.h \
    QtWebsocket/WsEnums.h

FORMS    += MainWindow.ui \
    SettingsDialog.ui

include(QtAwesome/QtAwesome/QtAwesome.pri)

RESOURCES += \
    resources.qrc

OTHER_FILES +=

SUBDIRS += \
    QtWebsocket/QtWebsocket.pro
