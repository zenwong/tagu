#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDebug>
#include <QCompleter>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "delegates/ThumbnailDelegate.hpp"
#include "Worker.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *);

private slots:
    void on_listView_clicked(const QModelIndex &index);
    void on_listView_doubleClicked(const QModelIndex &index);
    void on_editActs_returnPressed();
    void on_editTags_returnPressed();
    void on_editSearch_textEdited(const QString &arg1);
    void on_comboTag_currentIndexChanged(const QString &arg1);

    void onThumbnailView();
    void onCompactView();
    void onCoverView();
    void onImportVideos();
    void onOptions();
    void onLogin();
    void onResetDatabase();
    void onImportFinished();
    void onActressList();
    void onTagList();
    void onSync();
    void replyFinished(QNetworkReply*);

    void on_comboAct_currentIndexChanged(const QString &arg1);
    void on_listTags_doubleClicked(const QModelIndex &index);
    void on_listActs_doubleClicked(const QModelIndex &index);

private:
    void initDB();

    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlTableModel *vidTable, *tagTable, *actTable, *tagList, *actList;
    QCompleter *tagComplete, *actComplete, *searchComplete;
    ThumbnailDelegate *thumbDel;
    QSettings settings;
    int currentVid;
    QNetworkAccessManager *nam;
    QNetworkRequest post, get;

    QThread *thread;
    Worker *worker;
};

#endif // MAINWINDOW_HPP
