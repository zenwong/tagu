#include "ThumbnailDelegate.hpp"
#include <QDebug>

ThumbnailDelegate::ThumbnailDelegate(QWidget *parent) : QStyledItemDelegate(parent), fm(QApplication::font()), font("Times", 15, QFont::Bold){
    opts = new Options;

    if(opts->imageDir[opts->imageDir.size()] == QDir::separator().toLatin1()) {
        thumbDir = opts->imageDir + "thumbs" + QDir::separator();
    } else {
        thumbDir = opts->imageDir + QDir::separator() + "thumbs" + QDir::separator();
    }

    margin  = 10;
    padding = 0;
    thumbWidth = 400;
    thumbHeight = 225;

    frames = opts->rowCount * opts->colCount;
    //thumbHeight = opts.thumbWidth / 1.7777778;

    totalWidth = opts->colCount * opts->thumbWidth + padding * opts->colCount + margin * 2;
    totalHeight = opts->rowCount * thumbHeight + padding * opts->rowCount + margin * 2;

    //qDebug() << "total width: " << totalWidth << ", total height: " << totalHeight;
}

void ThumbnailDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyledItemDelegate::paint(painter,option,index);
    painter->save();
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);

    QPixmap pixmap(thumbDir + index.data().toString() + ".jpg");
    QRect rect(0, 0, thumbWidth, thumbHeight);
    painter->drawPixmap(rect, pixmap);

//    painter->setFont(font);
//    if (option.state & QStyle::State_Selected)
//        painter->setPen(QColor(Qt::white));
//    else
//        painter->setPen(QColor(Qt::black));

//    QRect bounding = option.rect;
//    bounding.adjust(-margin * 2,-margin - fm.height(), 0, 0);

//    painter->drawText(bounding, Qt::AlignBottom | Qt::AlignCenter, index.data().toString());


    painter->restore();
}

QSize ThumbnailDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    //return QSize(totalWidth + 5, totalHeight + 5);
    //return QSize(thumbWidth + margin * 2, thumbHeight + margin * 2 + fm.height() * 2);
    //return QSize(thumbWidth + margin * 2, thumbHeight + fm.height() * 2);
    return QSize(thumbWidth, thumbHeight);
}



