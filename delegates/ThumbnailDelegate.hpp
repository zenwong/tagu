#ifndef IMAGEDELEGATE_HPP
#define IMAGEDELEGATE_HPP
#include <QStyledItemDelegate>
#include <QApplication>
#include <QPainter>

class ThumbnailDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ThumbnailDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const ;
};

class ActressDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    ActressDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const ;
};

class CoverDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    CoverDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const ;
};

#endif // IMAGEDELEGATE_HPP
