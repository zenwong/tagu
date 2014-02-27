#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "SettingsDialog.hpp"
#include "dialogs/TagsDialog.hpp"
#include "dialogs/SignupDialog.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat) {
    ui->setupUi(this);
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    restoreState(settings.value("mainWindowState").toByteArray());
    initDB();

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

    thread = new QThread();
    worker = new Worker();

    worker->moveToThread(thread);
    connect(thread, SIGNAL(started()), worker, SLOT(mainLoop()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
    connect(worker, SIGNAL(importFinished()), this, SLOT(onImportFinished()));

    thread->start();

    nam = new QNetworkAccessManager(this);
    post.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    post.setRawHeader("Cookie", "session=$2y$05$tDIxAIPo9I9s5fURHfN8.epFzM5Civu2InhlRLGJ5US4kiCNZ/o9m");
    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

void MainWindow::replyFinished(QNetworkReply *reply) {
    qDebug() << reply->readAll();

    if(reply->error() == QNetworkReply::NoError) {
        QSqlQuery query(db);
        db.transaction();
        query.exec("update vids set synced = 1");
        query.exec("update vidtags set synced = 1");
        query.exec("update vidacts set synced = 1");
        db.commit();
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

//    QString stylesheet = "";
//    stylesheet += "QListView::item {border-bottom: 1px solid #6a6ea9;padding: 2px;color: black}";
//    stylesheet += "QListView::item:alternate {background-color: rgb(249, 249, 249);}";
//    stylesheet += "QListView::item:selected {background-color: rgb(49, 106, 197);padding: 0px;color: white;}";
//    ui->listView->setStyleSheet(stylesheet);
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

//    ui->editTags->setText(tags);
//    ui->editActs->setText(acts);
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index){
    //qDebug() << vidTable->columnCount();
    //qDebug() << "row: " <<  index.row() << ", column: " << index.column();
    QString url = vidTable->data(vidTable->index(index.row(), 5)).toString();
    QDesktopServices::openUrl(QUrl::fromLocalFile(url));
}

void MainWindow::on_editActs_returnPressed(){
    QString act = ui->comboAct->currentText().simplified();

    if(act.size() > 2) {
        QSqlQuery insert(db);
        QSqlQuery insert2(db);
        QSqlQuery query(db);

        insert.prepare("insert into acts(name) values(?)");
        insert2.prepare("insert into vidacts(vid,aid) values(?,?)");
        query.prepare("select _id from acts where name = ?");

        int aid = 0, vid = 0;
        QItemSelection selected( ui->listView->selectionModel()->selection() );

        db.transaction();
        foreach(QModelIndex index, selected.indexes()) {
            vid = vidTable->data(vidTable->index(index.row(), 0)).toInt();
            //qDebug() << "vid: " << vid;

            insert.bindValue(0, act);
            insert.exec();

            query.bindValue(0, act);
            query.exec();
            query.first();
            aid = query.value(0).toInt();
            //qDebug() << "aid: " << aid << ", name: '" << act << "'";

            insert2.bindValue(0, vid);
            insert2.bindValue(1, aid);
            insert2.exec();

        }
        db.commit();
        actTable->select();
        actList->select();

        ui->comboAct->lineEdit()->setText("");
    }
}

void MainWindow::on_editTags_returnPressed(){
    QString tag = ui->comboTag->currentText().simplified();

    if(tag.size() > 2) {
        QSqlQuery insert(db);
        QSqlQuery insert2(db);
        QSqlQuery query(db);

        insert.prepare("insert into tags(name) values(?)");
        insert2.prepare("insert into vidtags(vid,tid) values(?,?)");
        query.prepare("select _id from tags where name = ?");

        int tid = 0, vid = 0;
        QItemSelection selected( ui->listView->selectionModel()->selection() );

        db.transaction();
        foreach(QModelIndex index, selected.indexes()) {
            vid = vidTable->data(vidTable->index(index.row(), 0)).toInt();
            //qDebug() << "vid: " << vid;

            insert.bindValue(0, tag);
            insert.exec();

            query.bindValue(0, tag);
            query.exec();
            query.first();
            tid = query.value(0).toInt();
            //qDebug() << "tid: " << tid << ", name: '" << tag << "'";

            insert2.bindValue(0, vid);
            insert2.bindValue(1, tid);
            insert2.exec();

        }
        db.commit();
        tagTable->select();
        tagList->select();

        ui->comboTag->lineEdit()->setText("");
    }
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
    qDebug() << txt;
    QStringList terms = txt.split(" ");

    QString sql = "select * from search where search MATCH '" + txt.simplified() + "*'";
    QSqlQueryModel *model = new QSqlQueryModel(this);

    if(terms.size() > 1) {
        for(int i = 1; i < terms.size(); i++) {
            if(terms.at(i).simplified().size() > 1) {
                sql += " intersect select * from search where search MATCH '" + terms.at(i).simplified() + "*'";
            }
            QSqlQuery q(sql, db);
            model->setQuery(q);
            ui->listView->setModel(model);
            ui->listView->setModelColumn(0);
        }
    } else {
        if(txt.size() > 1) {
            QSqlQuery q(sql, db);
            model->setQuery(q);
            ui->listView->setModel(model);
            ui->listView->setModelColumn(0);
        }
    }

    if(txt.size() == 0) {
        ui->listView->setModel(vidTable);
        ui->listView->setModelColumn(1);
    }
}

void MainWindow::on_comboTag_currentIndexChanged(const QString &tag){
    QItemSelection selected( ui->listView->selectionModel()->selection() );
    QSqlQuery select(db);
    QSqlQuery insert(db);
    select.prepare("select _id from tags where name = ?");
    insert.prepare("insert into vidtags (vid,tid) values(?,?)");

    int vid;
    db.transaction();
    foreach(QModelIndex index, selected.indexes()) {
        vid = vidTable->data(vidTable->index(index.row(), 0)).toInt();
        select.bindValue(0, tag);
        select.exec();
        select.first();
        int tid = select.value(0).toInt();

        insert.bindValue(0, vid);
        insert.bindValue(1, tid);
        insert.exec();
    }
    db.commit();

    tagList->select();
}

void MainWindow::on_comboAct_currentIndexChanged(const QString &act){
    QItemSelection selected( ui->listView->selectionModel()->selection() );
    QSqlQuery select(db);
    QSqlQuery insert(db);
    select.prepare("select _id from acts where name = ?");
    insert.prepare("insert into vidacts (vid,aid) values(?,?)");

    int vid, aid;
    db.transaction();
    foreach(QModelIndex index, selected.indexes()) {
        vid = vidTable->data(vidTable->index(index.row(), 0)).toInt();
        select.bindValue(0, act);
        select.exec();
        select.first();
        aid = select.value(0).toInt();

        insert.bindValue(0, vid);
        insert.bindValue(1, aid);
        insert.exec();
    }
    db.commit();

    actList->select();
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
     worker->requestMethod(Worker::Import);
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

    //qDebug() << doc.toJson();

    post.setUrl(QUrl("http://tagu.in/sync"));
    nam->post(post, doc.toJson());
}

void MainWindow::onOptions() {
    SettingsDialog dialog;
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

void MainWindow::onImportFinished() {
    qDebug() << "MainWindow onImportFinished()";
    vidTable->select();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    Q_UNUSED(event);
//    if(db.isOpen()) {
//        db.close();
//    }
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());
}

