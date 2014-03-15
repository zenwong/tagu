#include "ThumbnailDelegate.hpp"
#include <QDebug>

ThumbnailDelegate::ThumbnailDelegate(QWidget *parent) : QStyledItemDelegate(parent), fm(QApplication::font()), font("Times", 15, QFont::Bold), opts(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat)
{
    QString imgDir = opts.value(Key::ImageDir).toString();
    if(imgDir[imgDir.size()] == QDir::separator().toLatin1()) {
        thumbDir = imgDir + "thumbs" + QDir::separator();
    } else {
        thumbDir = imgDir + QDir::separator() + "thumbs" + QDir::separator();
    }

    margin  = 10;
    padding = 0;
    thumbWidth = 400;
    thumbHeight = 225;

    //    totalWidth = opts->colCount * opts->thumbWidth + padding * opts->colCount + margin * 2;
    //    totalHeight = opts->rowCount * thumbHeight + padding * opts->rowCount + margin * 2;

    //qDebug() << "total width: " << totalWidth << ", total height: " << totalHeight;
}

void ThumbnailDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->save();
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);

    QString path = thumbDir + index.data().toString() + ".jpg";
    //qDebug() << path;

    QPixmap pixmap(path);
    QRect r = option.rect;

    r.adjust(10,10,-10,-10);
    painter->drawRoundedRect(r, 3.0, 3.0);

    r.adjust(10,10,-10,-60);
    //qDebug() << r;
    painter->drawPixmap(r, pixmap);

    r.adjust(0,10,0,40);

    painter->setFont(font);
    if (option.state & QStyle::State_Selected)
        painter->setPen(QColor(Qt::red));
    else
        painter->setPen(QColor(Qt::black));

    painter->drawText(r, Qt::AlignBottom | Qt::AlignCenter, index.data().toString());

    painter->restore();
}

QSize ThumbnailDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    //return QSize(totalWidth + 5, totalHeight + 5);
    //return QSize(thumbWidth + margin * 2, thumbHeight + margin * 2 + fm.height() * 2);
    //return QSize(thumbWidth + margin * 2, thumbHeight + fm.height() * 2);
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(500,340);
}



