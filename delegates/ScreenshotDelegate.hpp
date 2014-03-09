#ifndef SCREENSHOTDELEGATE_HPP
#define SCREENSHOTDELEGATE_HPP
#include <QStyledItemDelegate>
#include <QApplication>
#include <QPainter>
#include <QDir>
#include "../Config.hpp"
#include "../Utils.hpp"

class ScreenshotDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ScreenshotDelegate(QWidget *parent = 0);

    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const ;

private:
    Config config;
    QString screenDir;
};

#endif // SCREENSHOTDELEGATE_HPP
