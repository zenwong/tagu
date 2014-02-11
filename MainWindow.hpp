#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QtSql>
#include <map>
#include <QLabel>
#include "HtmlDelegate.hpp"
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_listView_clicked(const QModelIndex &index);

    void on_tableView_clicked(const QModelIndex &index);

    void on_listView_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlTableModel *vidTable;

    void initDB();
    QStringListModel *model;
    QStringList title, path;
    QLabel *totalVideos;
};

#endif // MAINWINDOW_HPP
