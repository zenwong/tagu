#include "CoverDelegate.hpp"

CoverDelegate::CoverDelegate(QWidget *parent) : QStyledItemDelegate(parent)
{
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
