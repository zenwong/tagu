#include "VidsModel.hpp"
#include <QDebug>

VidsModel::VidsModel(Config config, QObject *parent) : QSqlTableModel(parent)
{
}

QVariant VidsModel::data(const QModelIndex &index, int role) const
{
    QVariant result;
    if (role == VidsModel::SCREEN) {
        result = index.data(Qt::DisplayRole).toString() + ".jpg";
    }

    else
        result = QSqlTableModel::data(index, role);

    return result;
}

