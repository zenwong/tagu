#ifndef VIDSMODEL_HPP
#define VIDSMODEL_HPP
#include <QSqlTableModel>
#include <QSqlRecord>
#include "Config.hpp"

class VidsModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit VidsModel(Config config, QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    enum Roles
    {
        TAGS      = Qt::UserRole + 100,
        ACTS      = Qt::UserRole + 101,
        PATH      = Qt::UserRole + 102,
        SCREEN = Qt::UserRole + 103,
        THUMB  = Qt::UserRole + 104,
        COVER   = Qt::UserRole + 105,
        RATING  = Qt::UserRole + 106
    };

    void updateRating(const QModelIndex&,int);
};

#endif // VIDSMODEL_HPP
