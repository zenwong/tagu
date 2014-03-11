#include "ScreenshotDelegate.hpp"
#include "models/VidsModel.hpp"
#include <QDebug>

ScreenshotDelegate::ScreenshotDelegate(QWidget *parent) : QStyledItemDelegate(parent), fm(QApplication::font()), font("Times", 15, QFont::Bold)
{
    opts = new Options;
    if(opts->imageDir[opts->imageDir.size()] == QDir::separator().toLatin1()) {
        screenDir = opts->imageDir + "screens" + QDir::separator();
    } else {
        screenDir = opts->imageDir + QDir::separator() + "screens" + QDir::separator();
    }

    margin  = 10;
    padding = 0;

    int thumbHeight = 225;

    totalWidth = opts->colCount * 400 + padding * opts->colCount + margin * 2;
    totalHeight = opts->rowCount * thumbHeight + padding * opts->rowCount + margin * 2;
}

void ScreenshotDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyledItemDelegate::paint(painter,option,index);
    painter->save();
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);

    QString path = screenDir + index.data().toString();
    //qDebug() << "screen shot path: " << path;

    QPixmap pixmap(path);
    QRect rect(margin,margin, 1600, 900);
    painter->drawPixmap(rect, pixmap);

    painter->setFont(font);
    //painter->setFont(option.font);
    if (option.state & QStyle::State_Selected)
        painter->setPen(QColor(Qt::white));
    else
        painter->setPen(QColor(Qt::black));

    QRect bounding = option.rect;
    bounding.adjust(-margin * 2,-margin - fm.height(), 0, 0);

    painter->drawText(bounding, Qt::AlignBottom | Qt::AlignCenter, index.data().toString());

    painter->restore();
}

QSize ScreenshotDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    return QSize(totalWidth, totalHeight + 2 * fm.height());
}
