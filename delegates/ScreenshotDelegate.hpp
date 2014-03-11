#ifndef SCREENSHOTDELEGATE_HPP
#define SCREENSHOTDELEGATE_HPP
#include <QStyledItemDelegate>
#include <QApplication>
#include <QPainter>
#include <QDir>
#include "../Utils.hpp"

class ScreenshotDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ScreenshotDelegate(QWidget *parent = 0);

    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const ;

private:
    QString screenDir;
    int padding, margin, totalWidth, totalHeight, rows, cols, thumbWidth, thumbHeight;
    QFontMetrics fm;
    QFont font;
    QSettings opts;
};

#endif // SCREENSHOTDELEGATE_HPP
