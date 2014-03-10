#ifndef COVERDELEGATE_HPP
#define COVERDELEGATE_HPP
#include <QStyledItemDelegate>
#include <QApplication>
#include <QPainter>
#include <QDir>
#include "../Config.hpp"
#include "../Utils.hpp"

class CoverDelegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    CoverDelegate(QWidget *parent = 0);
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const ;

    Config config;
    QString coverDir;
};
#endif // COVERDELEGATE_HPP
