#ifndef IMAGEDELEGATE_HPP
#define IMAGEDELEGATE_HPP
#include <QStyledItemDelegate>
#include <QApplication>
#include <QPainter>
#include <QDir>
#include <QDebug>
#include "models/Options.hpp"
#include "Utils.hpp"

class ThumbnailDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ThumbnailDelegate(QWidget *parent = 0);

    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const ;

private:
    Options *opts;
    QString thumbDir;
    int margin, padding, totalWidth, totalHeight, frames, thumbHeight;
};

#endif // IMAGEDELEGATE_HPP
