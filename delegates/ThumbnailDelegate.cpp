#include "ThumbnailDelegate.hpp"
#include <QDebug>

void ThumbnailDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyledItemDelegate::paint(painter,option,index);
    painter->save();
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);

    //qDebug() << index.data().toString();
    QPixmap pixmap("/mnt/seagate/pics/thumbs/" + index.data().toString());

    //QRect rect(0,0,400,222);
    painter->drawPixmap(option.rect, pixmap);

    //    QFont font("Times", 15, QFont::Bold);
    //    painter->setFont(font);
    //    painter->drawText(option.rect, Qt::BottomSection, index.data().toString());


    painter->restore();
}

QSize ThumbnailDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const {
  return QSize(420, 240);
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

void CoverDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
//    QStyledItemDelegate::paint(painter,option,index);
//    painter->save();
//    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
//    painter->setRenderHint(QPainter::Antialiasing, true);
//    painter->setRenderHint(QPainter::TextAntialiasing, true);
//    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);

//    qDebug() << index.data().toString();
    QPixmap pixmap("/mnt/seagate/pics/covers/" + index.data().toString() + ".jpg");

    //QRect rect(0,0,800,537);

    painter->drawPixmap(option.rect, pixmap);
    //painter->drawPixmap(10,10,800,537, pixmap);

//    QFont font("Times", 15, QFont::Bold);
//    painter->setFont(font);
//    painter->drawText(rect2, Qt::AlignVCenter, index.data().toString());

    //painter->restore();
}

QSize CoverDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(800, 537);
}
