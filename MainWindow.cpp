#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "SettingsDialog.hpp"
#include "dialogs/TagsDialog.hpp"
#include "dialogs/SignupDialog.hpp"
#include "Utils.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat), thumbnailer(config) {
    ui->setupUi(this);
//    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
//    restoreState(settings.value("mainWindowState").toByteArray());

    restoreGeometry(config.windowGeometry);
    restoreState(config.windowState);

    QThreadPool::globalInstance()->setMaxThreadCount(1);
    initDB();

    statusImport = new QLabel;
    ui->statusBar->addPermanentWidget(statusImport);

    QString defaultView = settings.value("DefaultView").toString();
    QStyledItemDelegate *delegate;
    if(defaultView.isNull()) {
        delegate = new QStyledItemDelegate;
    } else {
        if(defaultView == "Compact") {
            delegate = new QStyledItemDelegate;
            ui->listView->setFlow(QListView::TopToBottom);
        } else if(defaultView == "Thumbnail") {
            delegate = new ThumbnailDelegate(this);
            ui->listView->setFlow(QListView::LeftToRight);
        } else if(defaultView == "Cover") {
            delegate = new CoverDelegate(this);
            ui->listView->setFlow(QListView::LeftToRight);
        }
    }

    //thumbDel = new ThumbnailDelegate(this);
    ui->listView->setItemDelegate(delegate);

    connect(&vidsWatcher, SIGNAL(finished()), this, SLOT(refreshVids()));
    connect(&dataWatcher, SIGNAL(finished()), this, SLOT(refreshData()));
    connect(&searchWatcher, SIGNAL(finished()), this, SLOT(refreshSearch()));
    connect(&importWatcher, SIGNAL(finished()), this, SLOT(refreshImport()));

    connect(ui->comboTag->lineEdit(), SIGNAL(returnPressed()), this, SLOT(on_editTags_returnPressed()));
    connect(ui->comboAct->lineEdit(), SIGNAL(returnPressed()), this, SLOT(on_editActs_returnPressed()));

    connect(ui->actionThumbnail, SIGNAL(triggered()), this, SLOT(onThumbnailView()));
    connect(ui->actionCompact, SIGNAL(triggered()), this, SLOT(onCompactView()));
    connect(ui->actionCover, SIGNAL(triggered()), this, SLOT(onCoverView()));
    connect(ui->actionActressList, SIGNAL(triggered()), this, SLOT(onActressList()));
    connect(ui->actionTags, SIGNAL(triggered()), this, SLOT(onTagList()));

    connect(ui->actionVideos, SIGNAL(triggered()), this, SLOT(onImportVideos()));
    connect(ui->actionSync_Database, SIGNAL(triggered()), this, SLOT(onSync()));
    connect(ui->actionOptions, SIGNAL(triggered()), this, SLOT(onOptions()));
    connect(ui->actionReset_Database, SIGNAL(triggered()), this, SLOT(onResetDatabase()));
    connect(ui->actionLogin, SIGNAL(triggered()), this, SLOT(onLogin()));

    ui->comboAct->lineEdit()->setText("");
    ui->comboTag->lineEdit()->setText("");

    nam = new QNetworkAccessManager(this);
    post.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // TODO get session from settings
    post.setRawHeader("Cookie", "session=$2y$05$tDIxAIPo9I9s5fURHfN8.epFzM5Civu2InhlRLGJ5US4kiCNZ/o9m");
    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

void MainWindow::refreshVids() {
    qDebug() << "refresh vids list";
    vidTable->select();
}

void MainWindow::refreshData() {
    qDebug() << "refresh data";

    tagTable->select();
    actTable->select();
    tagList->select();
    actList->select();

    ui->comboTag->lineEdit()->setText("");
    ui->comboAct->lineEdit()->setText("");
}

void MainWindow::refreshSearch() {
  ui->listView->setModel(searchFuture.result());
}

void MainWindow::refreshImport() {
    QString status = "Imported: " + QString::number(importFuture.result()) + " vids, in " + QString::number(timer.elapsed() / 1000.0) + " seconds";
    statusImport->setText(status);
//    QLabel *stat = new QLabel("Imported " + QString::number(importFuture.result()) + " vids");
//    stat->setAlignment(Qt::AlignRight);
//    ui->statusBar->addWidget(stat, 1);
    //ui->statusBar->showMessage("Imported " + QString::number(importFuture.result()) + " vids");

}

void MainWindow::replyFinished(QNetworkReply *reply) {
    qDebug() << reply->errorString();
    qDebug() << reply->readAll();

    if(reply->error() == QNetworkReply::NoError) {
        QtConcurrent::run(worker, &Worker::updateSyncedVids, db, reply->readAll());
    }
}

void MainWindow::initDB() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/db");
    db.open();

    vidTable = new QSqlTableModel(this, db);
    vidTable->setTable("LibraryView");
    vidTable->setEditStrategy(QSqlTableModel::OnFieldChange);
    vidTable->select();
    while (vidTable->canFetchMore()) {
        vidTable->fetchMore();
    }

    tagTable = new QSqlTableModel(this, db);
    tagTable->setTable("TagFilter");
    tagTable->setEditStrategy(QSqlTableModel::OnFieldChange);
    tagTable->select();

    actTable = new QSqlTableModel(this, db);
    actTable->setTable("ActFilter");
    actTable->setEditStrategy(QSqlTableModel::OnFieldChange);
    actTable->select();

    ui->listView->setModel(vidTable);
    ui->listView->setModelColumn(1);

    tagList = new QSqlTableModel(this, db);
    tagList->setTable("TagList");
    tagList->setEditStrategy(QSqlTableModel::OnFieldChange);
    tagList->select();

    actList = new QSqlTableModel(this, db);
    actList->setTable("ActList");
    actList->setEditStrategy(QSqlTableModel::OnFieldChange);
    actList->select();

    tagComplete = new QCompleter(tagTable);
    tagComplete->setCompletionColumn(1);
    tagComplete->setCompletionMode(QCompleter::PopupCompletion);
    tagComplete->setCaseSensitivity(Qt::CaseInsensitive);

    actComplete = new QCompleter(actTable);
    actComplete->setCompletionColumn(1);
    actComplete->setCompletionMode(QCompleter::PopupCompletion);
    actComplete->setCaseSensitivity(Qt::CaseInsensitive);

    searchComplete = new QCompleter(actTable);
    searchComplete->setCompletionColumn(1);
    searchComplete->setCompletionMode(QCompleter::PopupCompletion);
    searchComplete->setMaxVisibleItems(1);
    searchComplete->setCaseSensitivity(Qt::CaseInsensitive);

//    ui->listTags->setModel(tagList);
//    ui->listActs->setModel(actList);

    ui->editSearch->setCompleter(searchComplete);

    ui->comboTag->setModel(tagTable);
    ui->comboAct->setModel(actTable);

    ui->comboTag->setModelColumn(1);
    ui->comboTag->setCompleter(tagComplete);

    ui->comboAct->setModelColumn(1);
    ui->comboAct->setCompleter(actComplete);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_listView_clicked(const QModelIndex &index){
    QString tags = vidTable->data(vidTable->index(index.row(), 3)).toString();
    QString acts = vidTable->data(vidTable->index(index.row(), 4)).toString();
    currentVid = vidTable->data(vidTable->index(index.row(), 0)).toInt();

    //qDebug() << "current vid = " << currentVid;

    ui->listTags->setModel(tagList);
    ui->listActs->setModel(actList);
    ui->listTags->setModelColumn(2);
    ui->listActs->setModelColumn(2);

    tagList->setFilter("vid=" + QString::number(currentVid));
    actList->setFilter("vid=" + QString::number(currentVid));

}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index){
    QString url = vidTable->data(vidTable->index(index.row(), 5)).toString();
    QDesktopServices::openUrl(QUrl::fromLocalFile(url));
}

void MainWindow::onThumbnailView() {
  ui->listView->setItemDelegate(new ThumbnailDelegate);
  ui->listView->setFlow(QListView::LeftToRight);
  ui->listView->reset();
  settings.setValue("DefaultView", "Thumbnail");
}

void MainWindow::onCompactView() {
  ui->listView->setItemDelegate(new QStyledItemDelegate);
  ui->listView->setFlow(QListView::TopToBottom);
  ui->listView->reset();
  settings.setValue("DefaultView", "Compact");
}

void MainWindow::onCoverView() {
  ui->listView->setItemDelegate(new CoverDelegate);
  ui->listView->setFlow(QListView::LeftToRight);
  ui->listView->reset();
  settings.setValue("DefaultView", "Cover");
}

void MainWindow::onLogin() {
    SignupDialog dialog(this, nam);
    dialog.exec();
}

void MainWindow::onActressList() {

}

void MainWindow::onTagList() {
    TagsDialog dialog;
    dialog.exec();
}

void MainWindow::on_editSearch_textEdited(const QString &txt){
    if(txt.size() < 1) {
        ui->listView->setModel(vidTable);
        ui->listView->setModelColumn(1);
    } else {
        searchFuture = QtConcurrent::run(worker, &Worker::doSearch, db, txt);
        searchWatcher.setFuture(searchFuture);
    }
}

void MainWindow::on_editActs_returnPressed(){
    QString act = ui->comboAct->currentText().simplified();
    if(act.size() > 2) {
        QFuture<void> future = QtConcurrent::run(worker, &Worker::insertAct, db, act, vidTable, ui->listView);
        dataWatcher.setFuture(future);
    }
}

void MainWindow::on_editTags_returnPressed(){
    QString tag = ui->comboTag->currentText().simplified();
    if(tag.size() > 2) {
        QFuture<void> future = QtConcurrent::run(worker, &Worker::insertTag, db, tag, vidTable, ui->listView);
        dataWatcher.setFuture(future);
    }
}

void MainWindow::on_comboTag_currentIndexChanged(const QString &tag){
    if(ui->comboTag->isActiveWindow()) {
        QFuture<void> future = QtConcurrent::run(worker, &Worker::insertTag, db, tag, vidTable, ui->listView);
        dataWatcher.setFuture(future);
    }
}

void MainWindow::on_comboAct_currentIndexChanged(const QString &act){
    if(ui->comboAct->isActiveWindow()) {
        QFuture<void> future = QtConcurrent::run(worker, &Worker::insertAct, db, act, vidTable, ui->listView);
        dataWatcher.setFuture(future);
    }
}

void MainWindow::on_listTags_doubleClicked(const QModelIndex &index){
  int vid =  tagList->data(tagList->index(index.row(), 0)).toInt();
  int tid  = tagList->data(tagList->index(index.row(), 1)).toInt();

  QSqlQuery remove("delete from vidtags where vid = ? and tid = ?", db);
  remove.bindValue(0, vid);
  remove.bindValue(1, tid);
  remove.exec();
  tagList->select();
}

void MainWindow::on_listActs_doubleClicked(const QModelIndex &index){
    int vid =  actList->data(actList->index(index.row(), 0)).toInt();
    int aid  = actList->data(actList->index(index.row(), 1)).toInt();

    QSqlQuery remove("delete from vidacts where vid = ? and aid = ?", db);
    remove.bindValue(0, vid);
    remove.bindValue(1, aid);
    remove.exec();
    actList->select();
}

void MainWindow::onImportVideos() {
//     importFuture = QtConcurrent::run(worker, &Worker::doImport, db, config);
//     importWatcher.setFuture(importFuture);

    timer.start();
    importFuture = QtConcurrent::run(thumbnailer, &FFMpeg::run);
    importWatcher.setFuture(importFuture);
}

void MainWindow::onSync() {
    // TODO find a way to move this to worker thread, need to start event loop in Worker
    //worker->requestMethod(Worker::Sync);

    QSqlQuery query(db);

    db.transaction();
    query.exec("select title,hash from vids where synced = 0");
    QJsonArray vids;
    while(query.next()) {
        QJsonObject obj;
        obj["title"] = query.value(0).toString();
        obj["hash"] = query.value(1).toString();
        vids.append(obj);
    }

    //qDebug() << vids;

    QJsonArray tags;
    query.exec("select * from SyncTags");
    while(query.next()) {
        QJsonObject obj;
        obj["title"] = query.value(0).toString();
        obj["tag"] = query.value(1).toString();
        tags.append(obj);
    }

    //qDebug() << tags;

    QJsonArray acts;
    query.exec("select * from SyncActs");
    while(query.next()) {
        QJsonObject obj;
        obj["title"] = query.value(0).toString();
        obj["act"] = query.value(1).toString();
        acts.append(obj);
    }

    //qDebug() << acts;

    QJsonArray acttags;
    query.exec("select * from SyncActTags");
    while(query.next()) {
        QJsonObject obj;
        obj["act"] = query.value(0).toString();
        obj["tags"] = query.value(1).toString();
        acttags.append(obj);
    }
    db.commit();

    //qDebug() << acttags;

    QJsonObject json;
    if(vids.size() >= 1) {
        json["vids"] = vids;
    }
    if(tags.size() >= 1) {
        json["vidtags"] = tags;
    }
    if(acts.size() >= 1) {
        json["vidacts"] = acts;
    }
    if(acttags.size() >= 1) {
        json["acttags"] = acttags;
    }

    QJsonDocument doc(json);

    qDebug() << doc.toJson();

    post.setUrl(QUrl("http://api.tagu.in/sync"));
    nam->post(post, doc.toJson());
}

void MainWindow::onOptions() {
    SettingsDialog dialog;
    dialog.setConfig(config);
    dialog.exec();
}

void MainWindow::onResetDatabase() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Reset Database", "Are you sure you want to reset the database?", QMessageBox::Yes| QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QString sql = "delete from category; delete from tags; delete from acts; delete from vids;"
                "delete from vidacts; delete from vidtags; delete from acttags; delete from search;"
                "delete from sqlite_sequence;";
        QStringList scriptQueries = sql.split(';');
        QSqlQuery query(db);

        db.transaction();
        foreach (QString queryTxt, scriptQueries){
            if(queryTxt.size() > 1) {
                query.exec(queryTxt);
                query.finish();
            }
        }
        db.commit();
        vidTable->select();
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    Q_UNUSED(event);
    config.windowGeometry = saveGeometry();
    config.windowState = saveState();
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());
}

