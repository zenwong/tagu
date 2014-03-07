#include "SettingsDialog.hpp"
#include "ui_SettingsDialog.h"
#include <QFileDialog>
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent),
    settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat),
    ui(new Ui::SettingsDialog){
    ui->setupUi(this);
}

void SettingsDialog::setConfig(Settings conf) {
    this->config = conf;
    javModel = new QStringListModel(config.javDirs.toList());
    pornModel = new QStringListModel(config.pornDirs.toList());
    hentaiModel = new QStringListModel(config.hentaiDirs.toList());

    ui->listJavImport->setModel(javModel);
    ui->listPornImport->setModel(pornModel);
    ui->listHentaiImport->setModel(hentaiModel);

    ui->imageDir->setText(config.imageDir);
    ui->thumbWidth->setText(QString::number(config.thumbWidth));
    ui->thumbPercentage->setText(QString::number(config.thumbPercent));
}

SettingsDialog::~SettingsDialog(){
    delete ui;
}

void SettingsDialog::on_btnJavAdd_clicked(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), lastDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!exists(javDirs, dir) && dir.size() > 1) {
        config.javDirs.insert(dir);
        javModel->setStringList(config.javDirs.toList());
        lastDir = dir;
    }
}

void SettingsDialog::on_btnPornAdd_clicked(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), lastDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!exists(pornDirs, dir) && dir.size() > 1) {
        config.pornDirs.insert(dir);
        pornModel->setStringList(config.pornDirs.toList());
        lastDir = dir;
    }
}

void SettingsDialog::on_btnHentaiAdd_clicked(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), lastDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!exists(hentaiDirs, dir) && dir.size() > 1) {
        config.hentaiDirs.insert(dir);
        hentaiModel->setStringList(config.hentaiDirs.toList());
        lastDir = dir;
    }
}

void SettingsDialog::on_btnImageSave_clicked(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), lastDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->imageDir->setText(dir);
    settings.setValue("ImagesDir", dir);
    config.imageDir = dir;
}

void SettingsDialog::on_listJavImport_doubleClicked(const QModelIndex &index){
    QString dir = javModel->data(index, 0).toString();
    config.javDirs.remove(dir);
    javModel->setStringList(config.javDirs.toList());
}

void SettingsDialog::on_listPornImport_doubleClicked(const QModelIndex &index){
    QString dir = pornModel->data(index, 0).toString();
    config.pornDirs.remove(dir);
    pornModel->setStringList(config.pornDirs.toList());
}

void SettingsDialog::on_listHentaiImport_doubleClicked(const QModelIndex &index){
    QString dir = hentaiModel->data(index, 0).toString();
    config.hentaiDirs.remove(dir);
    hentaiModel->setStringList(config.hentaiDirs.toList());
}

void SettingsDialog::on_buttonBox_accepted(){
    settings.setValue("ThumbWidth", ui->thumbWidth->text());
    settings.setValue("ThumbPercentage", ui->thumbPercentage->text());

    config.thumbWidth = ui->thumbWidth->text().toInt();
    config.thumbPercent = ui->thumbPercentage->text().toInt();

    QString dir = ui->imageDir->text() + QDir::separator();
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

    config.save();
}

void SettingsDialog::on_buttonBox_rejected()
{
    qDebug() << "cancelled";
}
