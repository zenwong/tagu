#ifndef VIDSMODEL_HPP
#define VIDSMODEL_HPP
#include <QSqlTableModel>
#include "Config.hpp"

class VidsModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit VidsModel(Config config, QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    enum Roles
    {
        TAGS_ROLE = Qt::UserRole + 100,
        ACTS_ROLE = Qt::UserRole + 101,
        PATH_ROLE = Qt::UserRole + 102,
        SCREEN      = Qt::UserRole + 103,
        THUMB       = Qt::UserRole + 104
    };


};

#endif // VIDSMODEL_HPP
