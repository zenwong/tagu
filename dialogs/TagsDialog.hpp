#ifndef TAGSDIALOG_HPP
#define TAGSDIALOG_HPP

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>

namespace Ui {
class TagsDialog;
}

class TagsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagsDialog(QWidget *parent = 0);
    ~TagsDialog();

private:
    Ui::TagsDialog *ui;
    QSqlTableModel *model;
    QSqlDatabase db;
};

#endif // TAGSDIALOG_HPP
