#ifndef UTILS_HPP
#define UTILS_HPP
#include <QStringList>
#include <QListView>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QSettings>
#include <fstream>
#include <QDebug>
#include <sstream>

namespace Key {
    static QString ThumbWidth = "Images/ThumbWidth";
    static QString ThumbHeight = "Images/ThumbHeight";
    static QString Margin   = "Images/Margin";
    static QString Padding = "Images/Padding";
    static QString Rows      = "Images/Rows";
    static QString Columns = "Images/Columns";
    static QString ImageDir = "Images/ImageDir";
    static QString Email = "User/Email";
    static QString Password = "User/Password";
    static QString Session = "User/Session";
    static QString WinState = "State/WinState";
    static QString WinPos    = "State/WinPos";
    static QString LastView = "State/LastView";
    static QString ImportDir = "Import";
};

namespace Utils {
  QSet<QString> getDirs(QSettings&);
  void saveDirs(QSettings& settings, QSet<QString> dirs);
}

#endif // UTILS_HPP
