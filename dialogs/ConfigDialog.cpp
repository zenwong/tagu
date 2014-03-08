#include "ConfigDialog.hpp"
#include "ui_ConfigDialog.h"
#include "../Utils.hpp"

ConfigDialog::ConfigDialog(QWidget *parent) :  QDialog(parent),  ui(new Ui::ConfigDialog){
    ui->setupUi(this);

    config = loadConfig();

    ui->spinThumbWidth->setValue(config.thumbWidth);
    ui->spinColCount->setValue(config.colCount);
    ui->spinRowCount->setValue(config.rowCount);
    ui->editEmail->setText(QString::fromStdString(config.email));
    ui->editPassword->setText(QString::fromStdString(config.password));
    ui->editImageDir->setText(QString::fromStdString(config.imageDir));

    for(const auto& d: config.javDirs)
        dirs << QString::fromStdString(d);

    javModel = new QStringListModel(dirs);
    ui->listView->setModel(javModel);
}

ConfigDialog::~ConfigDialog()
{
    qDebug() << javModel->stringList();

    config.javDirs.clear();
   foreach(const QString& d, javModel->stringList()) {
       config.javDirs.emplace(d.toStdString());
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

    saveConfig(config);
    delete ui;
}

void ConfigDialog::on_spinThumbWidth_valueChanged(int thumbWidth)
{
    config.thumbWidth = thumbWidth;
}

void ConfigDialog::on_listView_doubleClicked(const QModelIndex &index){
    javModel->removeRow(index.row());
}

void ConfigDialog::on_pushButton_clicked(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), lastDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    config.javDirs.insert(dir.toStdString());

    dirs.clear();
    for(const auto& d: config.javDirs)
        dirs << QString::fromStdString(d);

    javModel->setStringList(dirs);
    lastDir = dir;
}

void ConfigDialog::on_spinRowCount_valueChanged(int rowCount)
{
    config.rowCount = rowCount;
}

void ConfigDialog::on_spinRowCount_valueChanged(const QString &rowCount)
{
    config.rowCount = rowCount.toInt();
}

void ConfigDialog::on_spinColCount_valueChanged(int colCount)
{
    config.colCount = colCount;
}

void ConfigDialog::on_spinColCount_valueChanged(const QString &colCount)
{
    config.colCount = colCount.toInt();
}

void ConfigDialog::on_editEmail_textChanged(const QString &email)
{
    config.email = email.toStdString();
}

void ConfigDialog::on_editPassword_textChanged(const QString &password)
{
    config.password = password.toStdString();
}

void ConfigDialog::on_toolButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), lastDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->editImageDir->setText(dir);
    config.imageDir = dir.toStdString();
}
