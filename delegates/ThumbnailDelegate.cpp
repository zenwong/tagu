#include "ThumbnailDelegate.hpp"
#include <QDebug>

ThumbnailDelegate::ThumbnailDelegate(QWidget *parent) : QStyledItemDelegate(parent) {
    this->config = loadConfig();

    if(config.imageDir[config.imageDir.size()] == QDir::separator().toLatin1()) {
        thumbDir = QString::fromStdString(config.imageDir) + "thumbs" + QDir::separator();
    } else {
        thumbDir = QString::fromStdString(config.imageDir) + QDir::separator() + "thumbs" + QDir::separator();
    }

    margin  = 0;
    padding = 0;
    config.thumbWidth = 400;
    frames = config.rowCount * config.colCount;
    //thumbHeight = config.thumbWidth / 1.7777778;
    thumbHeight = 225;

    totalWidth = config.colCount * config.thumbWidth + padding * config.colCount + margin * 2;
    totalHeight = config.rowCount * thumbHeight + padding * config.rowCount + margin * 2;

    //qDebug() << "total width: " << totalWidth << ", total height: " << totalHeight;
}

void ThumbnailDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyledItemDelegate::paint(painter,option,index);
    painter->save();
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->setRenderHint(QPainter::Antialiasing, true);
    //painter->setRenderHint(QPainter::TextAntialiasing, true);
    //painter->setRenderHint(QPainter::HighQualityAntialiasing, true);

    //qDebug() << index.data().toString();
    //QPixmap pixmap(thumbDir + index.data().toString());

    QRect rect(margin, margin, config.thumbWidth, thumbHeight);

    //    for(int i = 0; i < frames; i++) {
    //        if(i != 0 &&  i % 4 == 0) {
    //            rect.adjust(0, thumbHeight + padding, 0, 0);
    //            rect.moveLeft(margin);
    //            qDebug() << i << ") " << rect;
    //        }

    //        QPixmap pixmap(thumbDir + index.data().toString() + "-" + QString::number(i) + ".jpg");
    //        painter->drawPixmap()
    //    }


    for(int i = 0; i < frames; i++) {

        if(i != 0 &&  i % 4 == 0) {
            //qDebug() << "moduls = " << i % 4;
            rect.adjust(0, thumbHeight + padding, 0, 0);
            rect.moveLeft(margin);
            //qDebug() << i << ") " << rect;
        }

        QPixmap pixmap(thumbDir + index.data().toString() + "-" + QString::number(i) + ".jpg");
        painter->drawPixmap(rect, pixmap);
        //rect.moveLeft(config.thumbWidth + padding);

        rect.adjust(config.thumbWidth + padding, 0, 0, 0);

        //qDebug() << i << ") " <<  rect;
    }

    //    int i = 0;
    //    for(int y = 0; y < totalHeight; y += thumbHeight + padding) {

    //        qDebug() << "row: " << rect;
    //        rect.moveBottom(y);

    //        QFont font("Times", 15, QFont::Bold);
    //        painter->setFont(font);
    //        painter->setBrush(Qt::white);
    //        painter->drawText(rect, Qt::BottomSection, QString::number(i));

    //        for(int x = 0; x < totalWidth; x += config.thumbWidth + padding) {

    //            qDebug() << "col : " << rect;
    //            rect.moveLeft(config.thumbWidth + padding);

    //            if(x > totalWidth) {
    //                rect.moveLeft(margin);
    //            }

    //            QFont font("Times", 15, QFont::Bold);
    //            painter->setFont(font);
    //            painter->drawText(rect, Qt::BottomSection, QString::number(i));

    //            i++;
    //        }
    //    }


    //QRect rect(0,0,400,222);
    //painter->drawPixmap(option.rect, pixmap);

    //        QFont font("Times", 15, QFont::Bold);
    //        painter->setFont(font);
    //        painter->drawText(option.rect, Qt::BottomSection, index.data().toString());


    painter->restore();
}

QSize ThumbnailDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    return QSize(totalWidth + 5, totalHeight + 5);
}



