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
#include "Config.hpp"

static Config loadConfig() {
    string appdir = QCoreApplication::applicationDirPath().toStdString();
    string path = appdir + "/config.json";
    ifstream i_archive_stream( path );
    cereal::JSONInputArchive i_archive( i_archive_stream );
    Config config;
    i_archive( config );

    return config;
}

static void saveConfig(Config config) {
    string appdir = QCoreApplication::applicationDirPath().toStdString();
    string path = appdir + "/config.json";

    ofstream o_archive_stream(path);
    {
      cereal::JSONOutputArchive o_archive( o_archive_stream );
      o_archive( config );
    }

}

//struct Options {
//    int thumbWidth, rowCount, colCount, padding, margin;
//    QString email, password, session, imageDir, lastView;
//    QByteArray winState, winPosition;
//    QSet<QString> importDirs;
//    QSettings settings;

//    Options() : settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat) {
//        thumbWidth = settings.value("Images/thumbWidth").toInt();
//        rowCount = settings.value("Images/rowCount").toInt();
//        colCount = settings.value("Images/colCount").toInt();
//        padding = settings.value("Images/padding").toInt();
//        margin = settings.value("Images/margin").toInt();
//        imageDir = settings.value("Images/imageDirs").toString();

//        email = settings.value("User/email").toString();
//        password = settings.value("User/password").toString();
//        session = settings.value("User/session").toString();

//        lastView = settings.value("State/lastView").toString();
//        winState = settings.value("State/winState").toByteArray();
//        winPosition = settings.value("State/winPosition").toByteArray();

//        settings.beginGroup("Import");
//        int size = settings.beginReadArray("dirs");
//        for(int i=0; i < size; i++){
//            settings.setArrayIndex(i);
//            importDirs.insert(settings.value("dir").toString());
//        }
//        settings.endArray();
//        settings.endGroup();

//        //qDebug() << importDirs;
//    }

//    ~Options() {
//        settings.setValue("Images/thumbWidth", thumbWidth);
//        settings.setValue("Images/rowCount", rowCount);
//        settings.setValue("Images/colCount", colCount);
//        settings.setValue("Images/padding", padding);
//        settings.setValue("Images/margin", margin);
//        settings.setValue("Images/imageDir", imageDir);

//        settings.setValue("User/email", email);
//        settings.setValue("User/password", password);
//        settings.setValue("User/session", session);

//        settings.setValue("State/lastView", lastView);
//        settings.setValue("State/winState", winState);
//        settings.setValue("State/winPosition", winPosition);

//        settings.beginGroup("Import");
//        settings.beginWriteArray("dirs");

//        int i = 0;
//        foreach(const QString& dir, importDirs) {
//            settings.setArrayIndex(i);
//            settings.setValue("dir", dir);
//            i++;
//        }
//        settings.endArray();
//        settings.endGroup();
//    }
//};

static QList<int> getVids(QListView *list, QSqlTableModel *vidTable) {
    QItemSelection selected( list->selectionModel()->selection() );

    QList<int> vids;
    foreach(QModelIndex index, selected.indexes()) {
        vids.append(vidTable->data(vidTable->index(index.row(), 0)).toInt());
    }

    return vids;
}

#endif // UTILS_HPP
