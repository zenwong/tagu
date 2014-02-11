#ifndef HTMLDELEGATE_HPP
#define HTMLDELEGATE_HPP
#include <QStyledItemDelegate>
#include <QApplication>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QPainter>

class HtmlDelegate : public QStyledItemDelegate
{
protected:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

#endif

