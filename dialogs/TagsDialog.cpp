#include "TagsDialog.hpp"
#include "ui_TagsDialog.h"

TagsDialog::TagsDialog(QWidget *parent) :  QDialog(parent), ui(new Ui::TagsDialog){
    ui->setupUi(this);

    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/db");
    db.open();

    model = new QSqlTableModel(this, db);
    model->setTable("TagCategory");
    model->select();

    ui->treeView->setModel(model);
}

TagsDialog::~TagsDialog(){
    delete ui;
}
