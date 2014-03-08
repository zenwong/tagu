#ifndef UTILS_HPP
#define UTILS_HPP
#include <QStringList>
#include <QListView>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>

static QList<int> getVids(QListView *list, QSqlTableModel *vidTable) {
    QItemSelection selected( list->selectionModel()->selection() );
    qDebug() << selected;

    QList<int> vids;
    foreach(QModelIndex index, selected.indexes()) {
        vids.append(vidTable->data(vidTable->index(index.row(), 0)).toInt());
    }

    return vids;
}

enum Config {
    THUMB_WIDTH, ROW_COUNT, COL_COUNT, DIR
};

static void saveSettings(QSqlDatabase db, Config key, QString val) {
    QSqlQuery insert(db);
    insert.prepare( "insert into Settings(key,value) values(?,?)" );
    insert.bindValue(0, key);
    insert.bindValue(1, val);
    insert.exec();
}

#endif // UTILS_HPP
