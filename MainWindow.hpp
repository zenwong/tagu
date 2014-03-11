#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QDataWidgetMapper>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QCompleter>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>
#include <QLabel>
#include <QElapsedTimer>
#include "delegates/Delegates.hpp"
#include "Worker.hpp"
#include "FFMpeg.hpp"
#include "models/VidsModel.hpp"

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
    void onScreenshotView();
    void onImportVideos();
    void onOptions();
    void onLogin();
    void onResetDatabase();
    void onActressList();
    void onTagList();
    void onSync();
    void onRowChanged(QModelIndex,QModelIndex);
    void replyFinished(QNetworkReply*);

    void on_comboAct_currentIndexChanged(const QString &arg1);
    void on_listTags_doubleClicked(const QModelIndex &index);
    void on_listActs_doubleClicked(const QModelIndex &index);

    void refreshVids();
    void refreshData();
    void refreshSearch();
    void refreshImport();

    void on_editTitle_editingFinished();
    void on_editDesc_editingFinished();

    void on_comboRating_activated(int index);

private:
    void initDB();

    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlTableModel *tagTable, *actTable, *tagList, *actList;

    VidsModel *vidTable;

    QCompleter *tagComplete, *actComplete, *searchComplete;
    ThumbnailDelegate *thumbDel;
    //QSettings settings;
    int currentVid;
    QNetworkAccessManager *nam;
    QNetworkRequest post, get;

    QThread *thread;
    Worker *worker;
    QFutureWatcher<void> vidsWatcher, dataWatcher;
    QFutureWatcher<int> importWatcher;
    QFutureWatcher<QSqlQueryModel*> searchWatcher;
    QFuture<QSqlQueryModel*> searchFuture;
    QFuture<int> importFuture;
    QElapsedTimer timer;

    FFMpeg thumbnailer;

    QLabel *statusImport;
    QDataWidgetMapper *mapper;

    Options opts;
};

#endif // MAINWINDOW_HPP
