#include "ActressDelegate.hpp"

ActressDelegate::ActressDelegate(QWidget *parent) : QStyledItemDelegate(parent)
{
}

void ActressDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyledItemDelegate::paint(painter,option,index);
    painter->save();
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);

    QPixmap pixmap("/mnt/seagate/pics/actress/small/" + index.data().toString());

    QRect rect(0,0,120,120);
    QRect rect2(0,130,270,120);
    painter->drawPixmap(rect, pixmap);

    QFont font("Times", 15, QFont::Bold);
    painter->setFont(font);
    painter->drawText(rect2, Qt::AlignVCenter, index.data().toString());

    painter->restore();
}

QSize ActressDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    return QSize(400, 120);
}
