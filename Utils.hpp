#ifndef UTILS_HPP
#define UTILS_HPP
#include <QStringList>
#include <QListView>
#include <QSqlTableModel>

static QList<int> getVids(QListView *list, QSqlTableModel *vidTable) {
    QItemSelection selected( list->selectionModel()->selection() );
    qDebug() << selected;

    QList<int> vids;
    foreach(QModelIndex index, selected.indexes()) {
        vids.append(vidTable->data(vidTable->index(index.row(), 0)).toInt());
    }

    return vids;
}

#endif // UTILS_HPP
