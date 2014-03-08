#ifndef UTILS_HPP
#define UTILS_HPP
#include <QStringList>
#include <QListView>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <fstream>
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

static QList<int> getVids(QListView *list, QSqlTableModel *vidTable) {
    QItemSelection selected( list->selectionModel()->selection() );

    QList<int> vids;
    foreach(QModelIndex index, selected.indexes()) {
        vids.append(vidTable->data(vidTable->index(index.row(), 0)).toInt());
    }

    return vids;
}

#endif // UTILS_HPP
