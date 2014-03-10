#include "VidsModel.hpp"
#include <QDebug>

VidsModel::VidsModel(QObject *parent) : QSqlTableModel(parent)
{
    config = loadConfig();
}

//QVariant VidsModel::data(const QModelIndex &index, int role) const
//{
//    if (role < Qt::UserRole)
//            return QSqlQueryModel::data(index, role);

//    QVariant result;
//    //    if (role == VidsModel::SCREEN) {
//    //        result = index.data(Qt::DisplayRole).toString() + ".jpg";
//    //    }

//    switch(role) {
//    case SCREEN:
//        result = index.data(Qt::DisplayRole).toString() + ".jpg";
//        break;

//    case THUMB:
//        result = index.data(Qt::DisplayRole).toString() + ".jpg";
//        break;

//    case COVER:
//        result = index.data(Qt::DisplayRole).toString() + ".jpg";
//        break;

//    case TAGS:
//        result = index.data(Qt::DisplayRole).toString() + ".jpg";
//        break;

//    case ACTS:
//        result = index.data(Qt::DisplayRole).toString() + ".jpg";
//        break;

//    case PATH:
//        result = index.data(PATH).toString();
//        break;

//    case RATING: {
//        QModelIndex modelIndex = createIndex(index.row(), fieldIndex("rating"));
//        result = QSqlTableModel::data(modelIndex, Qt::DisplayRole);
//        break;
//    }

//    default:
//        result = QSqlTableModel::data(index, role);
//        break;
//    }

//    //    else
//    //        result = QSqlTableModel::data(index, role);

//    return result;
//}

void VidsModel::updateRating(const QModelIndex &index, int rating) {
    auto rec = record(index.row());
    qDebug() << rec;
    rec.setValue("rating", rating);
    setRecord(index.row(), rec);
    emit dataChanged(index,index);
}
