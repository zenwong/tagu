#ifndef IMAGEDELEGATE_HPP
#define IMAGEDELEGATE_HPP
#include <QStyledItemDelegate>
#include <QApplication>
#include <QPainter>
#include <QDir>
#include <QDebug>
#include "Config.hpp"
#include "Utils.hpp"

class ThumbnailDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ThumbnailDelegate(QWidget *parent = 0);

    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const ;

private:
    Config config;
    QString thumbDir;
    int margin, padding, totalWidth, totalHeight, frames, thumbHeight;
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
