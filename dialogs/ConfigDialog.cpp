#include "ConfigDialog.hpp"
#include "ui_ConfigDialog.h"
#include "../Utils.hpp"

ConfigDialog::ConfigDialog(QWidget *parent) :  QDialog(parent),  ui(new Ui::ConfigDialog){
    ui->setupUi(this);

    ui->spinThumbWidth->setValue(opts.thumbWidth);
    ui->spinColCount->setValue(opts.colCount);
    ui->spinRowCount->setValue(opts.rowCount);
    ui->editEmail->setText(opts.email);
    ui->editPassword->setText(opts.password);
    ui->editImageDir->setText(opts.imageDir);

    foreach(const auto& d, opts.importDirs)
        dirs << d;

    javModel = new QStringListModel(dirs);
    ui->listView->setModel(javModel);
}

ConfigDialog::~ConfigDialog()
{
    opts.importDirs.clear();
   foreach(const QString& d, javModel->stringList()) {
       opts.importDirs.insert(d);
   }

   QString dir = ui->editImageDir->text() + QDir::separator();
   QDir act(dir + "actress");
   QDir actSmall(act.absolutePath() + QDir::separator() + "small");
   QDir covers(dir + "covers");
   QDir screens(dir + "screens");
   QDir thumbs(dir + "thumbs");

   if(!act.exists()) QDir().mkdir(act.absolutePath());
   if(!actSmall.exists()) QDir().mkdir(actSmall.absolutePath());
   if(!covers.exists()) QDir().mkdir(covers.absolutePath());
   if(!screens.exists()) QDir().mkdir(screens.absolutePath());
   if(!thumbs.exists()) QDir().mkdir(thumbs.absolutePath());

    delete ui;
}

void ConfigDialog::on_spinThumbWidth_valueChanged(int thumbWidth)
{
    opts.thumbWidth = thumbWidth;
}

void ConfigDialog::on_listView_doubleClicked(const QModelIndex &index){
    javModel->removeRow(index.row());
}

void ConfigDialog::on_pushButton_clicked(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), lastDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    opts.importDirs.insert(dir);

    dirs.clear();
    foreach(const auto& d, opts.importDirs)
        dirs << d;

    javModel->setStringList(dirs);
    lastDir = dir;
}

void ConfigDialog::on_spinRowCount_valueChanged(int rowCount)
{
    opts.rowCount = rowCount;
}

void ConfigDialog::on_spinRowCount_valueChanged(const QString &rowCount)
{
    opts.rowCount = rowCount.toInt();
}

void ConfigDialog::on_spinColCount_valueChanged(int colCount)
{
    opts.colCount = colCount;
}

void ConfigDialog::on_spinColCount_valueChanged(const QString &colCount)
{
    opts.colCount = colCount.toInt();
}

void ConfigDialog::on_editEmail_textChanged(const QString &email)
{
    opts.email = email;
}

void ConfigDialog::on_editPassword_textChanged(const QString &password)
{
    opts.password = password;
}

void ConfigDialog::on_toolButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), lastDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->editImageDir->setText(dir);
    opts.imageDir = dir;
}
