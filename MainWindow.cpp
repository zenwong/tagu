#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <NLDatabase.h>
#include <QDesktopServices>
#include <QStandardItemModel>
using namespace NL::DB;

Database db("/root/devel/cpp/qt/JavByTitle/db");

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    initDB();
}

void MainWindow::initDB() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/root/devel/cpp/qt/JavByTitle/db");
    db.open();

    vidTable = new QSqlTableModel(this, db);
    vidTable->setTable("vids");
    //vidTable->setEditStrategy(QSqlTableModel::OnFieldChange);
    vidTable->select();
    while (vidTable->canFetchMore())
    {
        vidTable->fetchMore();
    }

//    qDebug() << vidTable->rowCount();

//    ui->tableView->setModel(vidTable);
//    ui->tableView->hideColumn(0);
//    ui->tableView->hideColumn(2);
//    ui->tableView->hideColumn(3);
//    ui->tableView->hideColumn(4);

//    QList<QTreeWidgetItem *> items;
//    for(const auto &row: db.query("select name from acts").select()) {
//        items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(row.column_string(0)))));
//    }
//    ui->treeVids->insertTopLevelItems(0, items);

//    QStandardItemModel *model = new QStandardItemModel();
//    QStandardItem *parentItem = model->invisibleRootItem();
//    for(const auto &row: db.query("select title from vids").select()) {
//        QStandardItem *item = new QStandardItem(QString::fromStdString(row.column_string(0)));
//        parentItem->appendRow(item);
//        //parentItem = item;
//    }
//    ui->treeView->setModel(model);

//    for(const auto &row: db.query("select title,path from vids").select()) {
//        title.append(QString::fromStdString(row.column_string(0)));
//        path.append(QString::fromStdString(row.column_string(1)));
//    }
//    model = new QStringListModel(title);
    ui->listView->setModel(vidTable);
    ui->listView->setModelColumn(1);

//    totalVideos = new QLabel(this);
//    totalVideos->setText("Total Videos: " + QString::number(title.size()));
//    ui->statusBar->addPermanentWidget(totalVideos);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_listView_clicked(const QModelIndex &index) {
  //qDebug() << vidTable->data(index, 1);
  qDebug() << vidTable->itemData(index);
  //qDebug() << model->itemData(index);
    //qDebug() << path.at(index.row());
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    //qDebug() << vidTable->data(index, 0);
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    QDesktopServices::openUrl(QUrl("file://" + path.at(index.row()), QUrl::TolerantMode));
}

