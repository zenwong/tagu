#include "VideoModel.hpp"

VideoModel::VideoModel(QSqlDatabase db, QObject *parent): QAbstractItemModel(parent){

    for(int i = 0; i < 10; i++) {
//        VideoItem *item = new VideoItem("SDMT-517", "/mnt/seagate/favs/SDMT-819.avi", "rape,teacher,incest", "Kaho Kasumi,Riko Tachibana,Nao Kamiki", "/tmp/ffmpeg/screens/OKSN-184C.jpg");
//        items.append(item);
    }

}

QVariant VideoModel::data(const QModelIndex& index, int role) const {
    VideoItem* item = items.at(index.row());
    if (!item) return QVariant();

    switch(role)
    {
    // qt specific roles
    case  Qt::DisplayRole:
        return item->title;
        break;

    case Qt::DecorationRole:
        return item->path;
        break;

    case TAGS_ROLE:
        return item->tags;
        break;

    case ACTS_ROLE:
        return item->acts;
        break;

    case PATH_ROLE:
        return item->path;
        break;

    default:
        return QVariant();
    }
}

// return empty index for flat structures. (special behavior needed for tree views)
QModelIndex VideoModel::parent(const QModelIndex& index) const {
    Q_UNUSED(index);
    return QModelIndex();
}

// create and return a standard index. (special behavior needed for tree views)
QModelIndex VideoModel::index(int row, int column, const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return createIndex(row, column);
}

// return row count. (here size of the list)
int VideoModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return items.size();
}

// return columns. (here only 1)
int VideoModel::columnCount(const QModelIndex& index) const {
    Q_UNUSED(index);
    return 1;
}

