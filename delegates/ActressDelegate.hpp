#ifndef ACTRESSDELEGATE_HPP
#define ACTRESSDELEGATE_HPP
#include <QStyledItemDelegate>
#include <QApplication>
#include <QPainter>
#include <QDir>
#include "../Config.hpp"
#include "../Utils.hpp"

class ActressDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    ActressDelegate(QWidget *parent = 0);
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const ;
};


#endif // ACTRESSDELEGATE_HPP
