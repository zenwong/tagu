#include "CoverDelegate.hpp"

CoverDelegate::CoverDelegate(QWidget *parent) : QStyledItemDelegate(parent)
{
    config = loadConfig();

    if(config.imageDir[config.imageDir.size()] == QDir::separator().toLatin1()) {
        coverDir = QString::fromStdString(config.imageDir) + "covers" + QDir::separator();
    } else {
        coverDir = QString::fromStdString(config.imageDir) + QDir::separator() + "covers" + QDir::separator();
    }
}

void CoverDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyledItemDelegate::paint(painter,option,index);
    painter->save();
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::TextAntialiasing, true);

    QPixmap pixmap("/mnt/seagate/pics/covers/" + index.data().toString() + ".jpg");

    painter->drawPixmap(option.rect, pixmap);

    painter->restore();
}

QSize CoverDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    Q_UNUSED(option);
    Q_UNUSED(index);
    return QSize(800, 537);
}
