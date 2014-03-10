#include "ScreenshotDelegate.hpp"
#include "models/VidsModel.hpp"
#include <QDebug>

ScreenshotDelegate::ScreenshotDelegate(QWidget *parent) : QStyledItemDelegate(parent) {
    this->config = loadConfig();

    if(config.imageDir[config.imageDir.size()] == QDir::separator().toLatin1()) {
        screenDir = QString::fromStdString(config.imageDir) + "screens" + QDir::separator();
    } else {
        screenDir = QString::fromStdString(config.imageDir) + QDir::separator() + "screens" + QDir::separator();
    }
}

void ScreenshotDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyledItemDelegate::paint(painter,option,index);
    painter->save();
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);

    QString path = screenDir + index.data(VidsModel::SCREEN).toString();
    qDebug() << "screen shot path: " << path;

    QPixmap pixmap(path);

    painter->drawPixmap(option.rect, pixmap);

    painter->restore();
}

QSize ScreenshotDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    return QSize(1600, 900);
}
