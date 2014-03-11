#ifndef CONFIGDIALOG_HPP
#define CONFIGDIALOG_HPP

#include <QDialog>
#include <QStringListModel>
#include <QFileDialog>
#include <QDebug>
#include <QSettings>
#include "Utils.hpp"

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();

signals:
    void onClose();

private slots:

    void on_spinThumbWidth_valueChanged(int arg1);
    void on_listView_doubleClicked(const QModelIndex &index);
    void on_pushButton_clicked();
    void on_spinRowCount_valueChanged(int arg1);
    void on_spinRowCount_valueChanged(const QString &arg1);
    void on_spinColCount_valueChanged(int arg1);
    void on_spinColCount_valueChanged(const QString &arg1);
    void on_editEmail_textChanged(const QString &arg1);
    void on_editPassword_textChanged(const QString &arg1);
    void on_toolButton_clicked();

private:
    Ui::ConfigDialog *ui;
    //QStringList dirs;
    QStringListModel *javModel;
    QString lastDir;
    QSettings opts;
    QSet<QString> dirs;
};

#endif // CONFIGDIALOG_HPP
