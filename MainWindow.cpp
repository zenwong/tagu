#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "SettingsDialog.hpp"
#include "NLDatabase/NLDatabase.h"
#include <QDesktopServices>
#include <QStandardItemModel>
#include <QtAwesome.h>
#include <QPushButton>
using namespace NL::DB;

Database sdb("/root/devel/cpp/qt/JavByTitle/db");

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    initDB();
    initToolBar();
}

void MainWindow::initDB() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/root/devel/cpp/qt/JavByTitle/db");
    db.open();

    vidTable = new QSqlTableModel(this, db);
    vidTable->setTable("vids");
    vidTable->setEditStrategy(QSqlTableModel::OnFieldChange);
    vidTable->select();
    while (vidTable->canFetchMore()) {
        vidTable->fetchMore();
    }

    for(const auto &row: sdb.query("select title,path from vids").select()) {
        title.append(QString::fromStdString(row.column_string(0)));
        path.append(QString::fromStdString(row.column_string(1)));
    }
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
    QtAwesome* awesome = new QtAwesome( this );
    awesome->initFontAwesome();

    QPushButton *options = new QPushButton();
    options->setText(QChar( icon_cog ) );
    options->setFont( awesome->font(40) );
    ui->mainToolBar->addWidget(options);

    connect(options, SIGNAL(clicked()), this, SLOT(options()));
}

void MainWindow::options() {
    SettingsDialog dialog;
    dialog.exec();
}

MainWindow::~MainWindow(){
    delete ui;
}

// loads tags and acts that were previously saved
void MainWindow::on_listView_clicked(const QModelIndex &index) {
    string title = vids->data(index,0).toString().toStdString();
    string acts = sdb.query("select act from VidActsView where title = ?").select_single(title.c_str()).column_string(0);
    ui->editActs->setText(QString::fromStdString(acts));

    string tags = sdb.query("select tag from VidTagsView where title = ?").select_single(title.c_str()).column_string(0);
    ui->editTags->setText(QString::fromStdString(tags));
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    QDesktopServices::openUrl(QUrl("file://" + path.at(index.row()), QUrl::TolerantMode));
}

// add acts to a video
void MainWindow::on_editActs_returnPressed()
{
    QStringList acts = ui->editActs->text().split(",");

    sdb.begin();
    foreach (QString act, acts) {
        int aid = sdb.query("select _id from acts where name = ?").select_single(act.simplified().toStdString()).column_int(0);

        if(aid == 0) {
            sdb.query("insert into acts(name) values(?)").execute(act.simplified().toStdString());
            aid = sdb.last_insert_rowid();
        }

        if(aid >= 1) {
            QItemSelection selection( ui->listView->selectionModel()->selection() );
            foreach( const QModelIndex & index, selection.indexes() ) {
                sdb.query("insert into vidacts(title,aid) values(?,?)").execute(title.at(index.row()).toStdString(), aid);
            }
        }

    }

    sdb.commit();
    searchView->select();
    actTable->select();
}

// add tags to a video
void MainWindow::on_editTags_returnPressed()
{
    QStringList tags = ui->editTags->text().split(",");

    sdb.begin();
    foreach (QString tag, tags) {

        int tid = sdb.query("select _id from tags where name = ?").select_single(tag.simplified().toStdString()).column_int(0);

        if(tid == 0) {
            sdb.query("insert into tags(name) values(?)").execute(tag.simplified().toStdString());
            tid = sdb.last_insert_rowid();
        }

        if(tid >= 1) {
            QItemSelection selection( ui->listView->selectionModel()->selection() );
            foreach( const QModelIndex & index, selection.indexes() ) {
                sdb.query("insert into vidtags(title,tid) values(?,?)").execute(title.at(index.row()).toStdString(), tid);
            }
        }

    }
    sdb.commit();
    searchView->select();
    tagTable->select();
}

// TODO find a more elegant way to filter by multiple tags and acts
// with the restriction that tags and acts are mixed together in the same lineEdit
void MainWindow::on_editSearch_returnPressed()
{
    QString search = ui->editSearch->text();
    QStringList terms = search.split(",");

    if(terms.size() == 1) {
        string term = terms.at(0).toStdString();
        QStringList resTitle, resPath;
        for(const auto& row: sdb.query("select vids.title as title, vids.path as path from vids inner join vidtags on vidtags.title = vids.title inner join tags on tags._id = vidtags.tid where tags.name = ? union  select vids.title as title, vids.path as path from vids inner join vidacts on vidacts.title = vids.title inner join acts on acts._id = vidacts.aid where acts.name = ?").select(term,term)){
            resTitle.append(QString::fromStdString(row.column_string(0)));
            resPath.append(QString::fromStdString(row.column_string(1)));
        }

        QStringListModel *searchResults = new QStringListModel(resTitle);
        ui->listView->setModel(searchResults);
    } else {
       QString sql;
       sql.reserve(9999);
       sql.append("select _id from tags where name = ");
       sql.append("'");
       sql.append(terms.at(0).simplified());
       sql.append("'");
       for(int i = 1; i < terms.size(); i++) {
           sql.append(" or name = ");
           sql.append("'");
           sql.append(terms.at(i).simplified());
           sql.append("'");
       }

       // we need to seperate the selects since i wanted the acts and tags
       // being searched for in the same lineEdit.  First we get a list of
       // aids and tids, then we intersect or union on those ids.
       QList<int> tids;
       for(const auto& row: sdb.query(sql.toStdString()).select()){
           tids.append(row.column_int(0));
       }

       qDebug() << tids;

       sql = "";
       sql.append("select _id from acts where name = ");
       sql.append("'");
       sql.append(terms.at(0).simplified());
       sql.append("'");
       for(int i = 1; i < terms.size(); i++) {
           sql.append(" or name = ");
           sql.append("'");
           sql.append(terms.at(i).simplified());
           sql.append("'");
       }

       QList<int> aids;
       for(const auto& row: sdb.query(sql.toStdString()).select()){
           aids.append(row.column_int(0));
       }

       qDebug() << aids;

       sql = "";
       sql.append("select vids.title as title, vids.path from vids inner join vidtags on vidtags.title = vids.title and vidtags.tid in(");
       sql.append(QString::number(tids.at(0)));
       for(int i = 1; i < tids.size(); i++) {
           sql.append(",");
           sql.append(QString::number(tids.at(i)));
       }
       sql.append(") group by vids.title having count(vids.title) = ");
       sql.append(QString::number(tids.size()));

       // check if user specified any or all in filter
       if(ui->radioAll->isChecked()) {
           sql.append(" intersect ");
       } else {
           sql.append(" union ");
       }

       sql.append("select vids.title as title, vids.path from vids inner join vidacts on vidacts.title = vids.title and vidacts.aid in(");
       sql.append(QString::number(aids.at(0)));
       for(int i = 1; i < aids.size(); i++) {
           sql.append(",");
           sql.append(QString::number(aids.at(i)));
       }
       sql.append(") group by vids.title having count(vids.title) = ");
       sql.append(QString::number(aids.size()));

       qDebug() << sql;
       QStringList resTitle, resPath;
       for(const auto& row: sdb.query(sql.toStdString()).select()){
           qDebug() << QString::fromStdString(row.column_string(0)) << " \ " << QString::fromStdString(row.column_string(1));
           resTitle.append(QString::fromStdString(row.column_string(0)));
           resPath.append(QString::fromStdString(row.column_string(1)));
       }

       QStringListModel *searchResults = new QStringListModel(resTitle);
       ui->listView->setModel(searchResults);
    }
}
