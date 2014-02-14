#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QtSql>
#include <map>
#include <QLabel>
#include <QStringListModel>
#include "HtmlDelegate.hpp"
#include "MultiCompleter.hpp"
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
    void on_listView_doubleClicked(const QModelIndex &index);
    void on_listWidget_clicked(const QModelIndex &index);
    void on_listWidget_doubleClicked(const QModelIndex &index);
    void on_editActs_returnPressed();
    void on_editTags_returnPressed();
    void on_editSearch_returnPressed();

    void options();
    void onGridView();
    void onRowView();
    void onHot1();


private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlTableModel *vidTable, *actTable, *tagTable, *vidtags, *vidacts, *searchView;
    MultiCompleter *autoTags, *autoActs, *autoSearch;
    QStringListModel *vids;
    QStringList title, path;
    QLabel *totalVideos;

    void initDB();
    void initToolBar();
};

#endif // MAINWINDOW_HPP
