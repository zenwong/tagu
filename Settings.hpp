#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include <QSet>
#include <QFile>
#include <QDataStream>
#include <QCoreApplication>

class Settings
{
public:
    Settings();
    ~Settings();

    void clear();

    QByteArray windowGeometry, windowState;
    int thumbWidth, thumbPercent;
    QString email, password, session;
    QSet<QString> javDirs, pornDirs, hentaiDirs;
};

#endif // SETTINGS_HPP
