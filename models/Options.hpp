#ifndef OPTIONS_HPP
#define OPTIONS_HPP
#include <QString>
#include <QByteArray>
#include <QSet>
#include <QSettings>
#include <QDebug>
#include <QCoreApplication>

struct Options {
    Options();
    ~Options();

    int thumbWidth, rowCount, colCount, padding, margin;
    QString email, password, session, imageDir, lastView;
    QByteArray winState, winPosition;
    QSet<QString> importDirs;
    QSettings settings;

    //void save();
};

#endif // OPTIONS_HPP
