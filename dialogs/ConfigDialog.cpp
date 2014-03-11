#include "ConfigDialog.hpp"
#include "ui_ConfigDialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) :  QDialog(parent),  ui(new Ui::ConfigDialog), opts(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat){
    ui->setupUi(this);

    ui->spinThumbWidth->setValue(opts.value(Key::ThumbWidth).toInt());
    ui->spinColCount->setValue(opts.value(Key::Columns).toInt());
    ui->spinRowCount->setValue(opts.value(Key::Rows).toInt());
    ui->editEmail->setText(opts.value(Key::Email).toString());
    ui->editPassword->setText(opts.value(Key::Password).toString());
    ui->editImageDir->setText(opts.value(Key::ImageDir).toString());

    dirs = Utils::getDirs(opts);

    javModel = new QStringListModel(dirs.toList());
    ui->listView->setModel(javModel);
}

ConfigDialog::~ConfigDialog()
{
//    opts.importDirs.clear();
//   foreach(const QString& d, javModel->stringList()) {
//       opts.importDirs.insert(d);
//   }

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

   emit onClose();
}

void ConfigDialog::on_spinThumbWidth_valueChanged(int thumbWidth)
{
    opts.setValue(Key::ThumbWidth, thumbWidth);
}

void ConfigDialog::on_listView_doubleClicked(const QModelIndex &index){
    dirs.remove(index.data().toString());
    Utils::saveDirs(opts,dirs);
    javModel->removeRow(index.row());
}

void ConfigDialog::on_pushButton_clicked(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), lastDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    dirs.insert(dir);
    Utils::saveDirs(opts, dirs);

    javModel->setStringList(dirs.toList());
    lastDir = dir;
}

void ConfigDialog::on_spinRowCount_valueChanged(int rowCount)
{
    opts.setValue(Key::Rows, rowCount);
}

void ConfigDialog::on_spinRowCount_valueChanged(const QString &rowCount)
{
    opts.setValue(Key::Rows, rowCount.toInt());
}

void ConfigDialog::on_spinColCount_valueChanged(int colCount)
{
    opts.setValue(Key::Columns, colCount);
}

void ConfigDialog::on_spinColCount_valueChanged(const QString &colCount)
{
    opts.setValue(Key::Columns, colCount.toInt());
}

void ConfigDialog::on_editEmail_textChanged(const QString &email)
{
    opts.setValue(Key::Email, email);
}

void ConfigDialog::on_editPassword_textChanged(const QString &password)
{
    opts.setValue(Key::Password, password);
}

void ConfigDialog::on_toolButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), lastDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->editImageDir->setText(dir);
    opts.setValue(Key::ImageDir, dir);
}
