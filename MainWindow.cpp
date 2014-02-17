#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "SettingsDialog.hpp"
#include "Utils.hpp"
#include <QDesktopServices>
#include <QStandardItemModel>
#include <QPushButton>
#include <QListWidgetItem>
#include <QShortcut>
#include <QDirIterator>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), settings(qApp->applicationDirPath() + "/settings.ini", QSettings::IniFormat) {
    ui->setupUi(this);

    initDB();
    initToolBar();

    QSize size(420,240);

    qDebug() << qApp->applicationDirPath();

    QCoreApplication::setOrganizationName("Tagu");
    QCoreApplication::setOrganizationDomain("tagu.in");
    QCoreApplication::setApplicationName("Tagu");

//    settings.beginWriteArray("ImportDirs");
//    settings.setArrayIndex(0);
//    settings.setValue("dir", "/mnt/seagate/favs");
//    settings.setArrayIndex(1);
//    settings.setValue("dir", "/mnt/seagate/japanese");
//    settings.setArrayIndex(2);
//    settings.setValue("dir", "/mnt/seagate/downloads");
//    settings.endArray();

    int i = 0;
    QDir dir("/mnt/seagate/pics/thumbs");
    QDirIterator iterator(dir.absolutePath(), QDirIterator::Subdirectories);
    while (iterator.hasNext()) {
        iterator.next();
        if (!iterator.fileInfo().isDir()) {
            QString filename = iterator.fileName();

            QListWidgetItem *item = new QListWidgetItem(filename, ui->listWidget);
            item->setSizeHint(size);
            ui->listWidget->insertItem(i, item);

            QLabel *lbl = new QLabel(filename);
            lbl->setPixmap(pixmapWithText(QPixmap(iterator.filePath()), filename.toStdString()));
            lbl->setAlignment(Qt::AlignCenter);
            ui->listWidget->setItemWidget(item, lbl);
            i++;
        }
    }

    QShortcut *hot1 = new QShortcut(QKeySequence("Ctrl+1"), this);
    connect(hot1, SIGNAL(activated()), this, SLOT(onHot1()));
}

void MainWindow::initDB() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/root/devel/cpp/qt/Tagu/db");
    db.open();

    QSqlQuery vidsQuery("select title,path from vids", db);

    while(vidsQuery.next()) {
        title.append(vidsQuery.value(0).toString());
        path.append(vidsQuery.value(1).toString());
    }

    vidTable = new QSqlTableModel(this, db);
    vidTable->setTable("vids");
    vidTable->setEditStrategy(QSqlTableModel::OnFieldChange);
    vidTable->select();
    while (vidTable->canFetchMore()) {
        vidTable->fetchMore();
    }

//    for(const auto &row: sdb.query("select title,path from vids").select()) {
//        title.append(QString::fromStdString(row.column_string(0)));
//        path.append(QString::fromStdString(row.column_string(1)));
//    }
    vids = new QStringListModel(title);

    tagTable = new QSqlTableModel(this, db);
    tagTable->setTable("tags");
    tagTable->setEditStrategy(QSqlTableModel::OnFieldChange);
    tagTable->select();

    vidtags = new QSqlTableModel(this, db);
    vidtags->setTable("vidTags");
    vidtags->setEditStrategy(QSqlTableModel::OnFieldChange);
    vidtags->select();

    actTable = new QSqlTableModel(this, db);
    actTable->setTable("acts");
    actTable->setEditStrategy(QSqlTableModel::OnFieldChange);
    actTable->select();

    vidacts = new QSqlTableModel(this, db);
    vidacts->setTable("vidActs");
    vidacts->setEditStrategy(QSqlTableModel::OnFieldChange);
    vidacts->select();

    searchView = new QSqlTableModel(this, db);
    searchView->setTable("SearchView");
    searchView->setEditStrategy(QSqlTableModel::OnFieldChange);
    searchView->select();

    autoTags = new MultiCompleter();
    autoTags->setModel(tagTable);
    autoTags->setCompletionMode(QCompleter::PopupCompletion);
    autoTags->setCompletionColumn(1);

    autoActs = new MultiCompleter();
    autoActs->setModel(actTable);
    autoActs->setCompletionMode(QCompleter::PopupCompletion);
    autoActs->setCaseSensitivity(Qt::CaseInsensitive);
    autoActs->setCompletionColumn(1);

    autoSearch = new MultiCompleter();
    autoSearch->setModel(searchView);
    autoSearch->setCompletionMode(QCompleter::PopupCompletion);
    autoSearch->setCaseSensitivity(Qt::CaseInsensitive);
    autoSearch->setCompletionColumn(0);

    ui->listView->setModel(vids);
    ui->editTags->setCompleter(autoTags);
    ui->editActs->setCompleter(autoActs);
    ui->editSearch->setCompleter(autoSearch);
}

void MainWindow::initToolBar() {

}

void MainWindow::syncToServer() {

}

void MainWindow::options() {
    SettingsDialog dialog;
    dialog.exec();
}

void MainWindow::onGridView() {

}

void MainWindow::onRowView() {

}

void MainWindow::onHot1() {
    qDebug() << "shortcut 1";
}

MainWindow::~MainWindow(){
    delete ui;
}

// loads tags and acts that were previously saved
void MainWindow::on_listView_clicked(const QModelIndex &index) {
    QString title = vids->data(index,0).toString();
    qDebug()  << title;

    QSqlQuery actsForVidQuery("select act from VidActsView where title = ?", db);
    QSqlQuery tagsForVidQuery("select tag from VidTagsView where title = ?", db);

    tagsForVidQuery.bindValue(0, title);
    if(tagsForVidQuery.exec()) {
        tagsForVidQuery.first();
        ui->editTags->setText(tagsForVidQuery.value(0).toString());
    }

    actsForVidQuery.bindValue(0, title);
    if(actsForVidQuery.exec()) {
        actsForVidQuery.first();
        ui->editActs->setText(actsForVidQuery.value(0).toString());
    }

    //string acts = sdb.query("select act from VidActsView where title = ?").select_single(title.c_str()).column_string(0);

    //string tags = sdb.query("select tag from VidTagsView where title = ?").select_single(title.c_str()).column_string(0);
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    QDesktopServices::openUrl(QUrl("file://" + path.at(index.row()), QUrl::TolerantMode));
}

// add acts to a video
void MainWindow::on_editActs_returnPressed()
{
    QStringList acts = ui->editActs->text().split(",");

    QSqlQuery insert("insert into acts(name) values(?)", db);
    QSqlQuery select("select _id from acts where name = ?", db);
    QSqlQuery insertVidActs("insert into vidacts(title,aid) values(?,?)", db);

    foreach(QString act, acts) {
        select.bindValue(0, act);
        select.exec();

        int aid = 0;
        if(select.first()) {
            aid = select.value(0).toInt();
        } else {
            insert.bindValue(0, act);
            insert.exec();
            aid = insert.lastInsertId().toInt();
        }

        qDebug() << aid;
        QItemSelection selection( ui->listView->selectionModel()->selection() );
        qDebug() << selection.size();
        foreach( const QModelIndex & index, selection.indexes() ) {
            qDebug() << index.row();
            qDebug() << title.at(index.row());

            insertVidActs.bindValue(0, title.at(index.row()));
            insertVidActs.bindValue(1, aid);
            insertVidActs.exec();
        }
    }

//    sdb.begin();
//    foreach (QString act, acts) {
//        int aid = sdb.query("select _id from acts where name = ?").select_single(act.simplified().toStdString()).column_int(0);

//        if(aid == 0) {
//            sdb.query("insert into acts(name) values(?)").execute(act.simplified().toStdString());
//            aid = sdb.last_insert_rowid();
//        }

//        if(aid >= 1) {
//            QItemSelection selection( ui->listView->selectionModel()->selection() );
//            foreach( const QModelIndex & index, selection.indexes() ) {
//                sdb.query("insert into vidacts(title,aid) values(?,?)").execute(title.at(index.row()).toStdString(), aid);
//            }
//        }

//    }

//    sdb.commit();
    searchView->select();
    actTable->select();
}

// add tags to a video
void MainWindow::on_editTags_returnPressed()
{
    QStringList tags = ui->editTags->text().split(",");

//    sdb.begin();
//    foreach (QString tag, tags) {

//        int tid = sdb.query("select _id from tags where name = ?").select_single(tag.simplified().toStdString()).column_int(0);

//        if(tid == 0) {
//            sdb.query("insert into tags(name) values(?)").execute(tag.simplified().toStdString());
//            tid = sdb.last_insert_rowid();
//        }

//        if(tid >= 1) {
//            QItemSelection selection( ui->listView->selectionModel()->selection() );
//            foreach( const QModelIndex & index, selection.indexes() ) {
//                sdb.query("insert into vidtags(title,tid) values(?,?)").execute(title.at(index.row()).toStdString(), tid);
//            }
//        }

//    }
//    sdb.commit();
    searchView->select();
    tagTable->select();
}

void MainWindow::on_listWidget_clicked(const QModelIndex &index)
{

}

void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    QString txt = ui->listWidget->currentItem()->text();
    txt.remove(txt.size() - 4, 4);

    qDebug() << txt;
    QSqlQuery pathForVidQuery("select path from vids where title = ?", db);
    pathForVidQuery.bindValue(0, txt);
    pathForVidQuery.exec(); pathForVidQuery.first();

    //string path = sdb.query("select path from vids where title = ?").select_single(txt.toStdString()).column_string(0);

    QString url = "file://";
    url.append(pathForVidQuery.value(0).toString());
    QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode));
}

// TODO find a more elegant way to filter by multiple tags and acts
// with the restriction that tags and acts are mixed together in the same lineEdit
void MainWindow::on_editSearch_returnPressed()
{
    QString search = ui->editSearch->text();
    QStringList terms = search.split(",");

//    if(terms.size() == 1) {
//        string term = terms.at(0).toStdString();
//        QStringList resTitle, resPath;
//        for(const auto& row: sdb.query("select vids.title as title, vids.path as path from vids inner join vidtags on vidtags.title = vids.title inner join tags on tags._id = vidtags.tid where tags.name = ? union  select vids.title as title, vids.path as path from vids inner join vidacts on vidacts.title = vids.title inner join acts on acts._id = vidacts.aid where acts.name = ?").select(term,term)){
//            resTitle.append(QString::fromStdString(row.column_string(0)));
//            resPath.append(QString::fromStdString(row.column_string(1)));
//        }

//        QStringListModel *searchResults = new QStringListModel(resTitle);
//        ui->listView->setModel(searchResults);
//    } else {
//       QString sql;
//       sql.reserve(9999);
//       sql.append("select _id from tags where name = ");
//       sql.append("'");
//       sql.append(terms.at(0).simplified());
//       sql.append("'");
//       for(int i = 1; i < terms.size(); i++) {
//           sql.append(" or name = ");
//           sql.append("'");
//           sql.append(terms.at(i).simplified());
//           sql.append("'");
//       }

//       // we need to seperate the selects since i wanted the acts and tags
//       // being searched for in the same lineEdit.  First we get a list of
//       // aids and tids, then we intersect or union on those ids.
//       QList<int> tids;
//       for(const auto& row: sdb.query(sql.toStdString()).select()){
//           tids.append(row.column_int(0));
//       }

//       qDebug() << tids;

//       sql = "";
//       sql.append("select _id from acts where name = ");
//       sql.append("'");
//       sql.append(terms.at(0).simplified());
//       sql.append("'");
//       for(int i = 1; i < terms.size(); i++) {
//           sql.append(" or name = ");
//           sql.append("'");
//           sql.append(terms.at(i).simplified());
//           sql.append("'");
//       }

//       QList<int> aids;
//       for(const auto& row: sdb.query(sql.toStdString()).select()){
//           aids.append(row.column_int(0));
//       }

//       qDebug() << aids;

//       sql = "";
//       sql.append("select vids.title as title, vids.path from vids inner join vidtags on vidtags.title = vids.title and vidtags.tid in(");
//       sql.append(QString::number(tids.at(0)));
//       for(int i = 1; i < tids.size(); i++) {
//           sql.append(",");
//           sql.append(QString::number(tids.at(i)));
//       }
//       sql.append(") group by vids.title having count(vids.title) = ");
//       sql.append(QString::number(tids.size()));

//       // check if user specified any or all in filter
//       if(ui->radioAll->isChecked()) {
//           sql.append(" intersect ");
//       } else {
//           sql.append(" union ");
//       }

//       sql.append("select vids.title as title, vids.path from vids inner join vidacts on vidacts.title = vids.title and vidacts.aid in(");
//       sql.append(QString::number(aids.at(0)));
//       for(int i = 1; i < aids.size(); i++) {
//           sql.append(",");
//           sql.append(QString::number(aids.at(i)));
//       }
//       sql.append(") group by vids.title having count(vids.title) = ");
//       sql.append(QString::number(aids.size()));

//       qDebug() << sql;
//       QStringList resTitle, resPath;
//       for(const auto& row: sdb.query(sql.toStdString()).select()){
//           qDebug() << QString::fromStdString(row.column_string(0)) << " \ " << QString::fromStdString(row.column_string(1));
//           resTitle.append(QString::fromStdString(row.column_string(0)));
//           resPath.append(QString::fromStdString(row.column_string(1)));
//       }

//       QStringListModel *searchResults = new QStringListModel(resTitle);
//       ui->listView->setModel(searchResults);
//    }
}
