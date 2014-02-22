#include "SettingsDialog.hpp"
#include "ui_SettingsDialog.h"
#include <QFileDialog>
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent),
    settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat),
    ui(new Ui::SettingsDialog){
    ui->setupUi(this);

    int len = settings.beginReadArray("ImportJavDirs");
    for (int i = 0; i < len; ++i) {
        settings.setArrayIndex(i);
        javDirs.append(settings.value("dir").toString());
    }
    settings.endArray();

    len = settings.beginReadArray("ImportPornDirs");
    for (int i = 0; i < len; ++i) {
        settings.setArrayIndex(i);
        pornDirs.append(settings.value("dir").toString());
    }
    settings.endArray();

    len = settings.beginReadArray("ImportHentaiDirs");
    for (int i = 0; i < len; ++i) {
        settings.setArrayIndex(i);
        hentaiDirs.append(settings.value("dir").toString());
    }
    settings.endArray();

    javModel = new QStringListModel(javDirs);
    ui->listJavImport->setModel(javModel);

    pornModel = new QStringListModel(pornDirs);
    ui->listPornImport->setModel(pornModel);

    hentaiModel = new QStringListModel(hentaiDirs);
    ui->listHentaiImport->setModel(hentaiModel);

    ui->imageDir->setText(settings.value("ImagesDir").toString());
    ui->thumbWidth->setText(settings.value("ThumbWidth").toString());
    ui->thumbPercentage->setText(settings.value("ThumbPercentage").toString());
}


SettingsDialog::~SettingsDialog(){
    delete ui;
}

void SettingsDialog::on_btnJavAdd_clicked(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), lastDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!exists(javDirs, dir) && dir.size() > 1) {
        javDirs.append(dir);
        javModel->setStringList(javDirs);
        saveArray(settings, "ImportJavDirs", javDirs);
        lastDir = dir;
    }
}

void SettingsDialog::on_btnPornAdd_clicked(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), lastDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!exists(pornDirs, dir) && dir.size() > 1) {
        pornDirs.append(dir);
        pornModel->setStringList(pornDirs);
        saveArray(settings, "ImportPornDirs", pornDirs);
        lastDir = dir;
    }
}

void SettingsDialog::on_btnHentaiAdd_clicked(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), lastDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!exists(hentaiDirs, dir) && dir.size() > 1) {
        hentaiDirs.append(dir);
        hentaiModel->setStringList(hentaiDirs);
        saveArray(settings, "ImportHentaiDirs", hentaiDirs);
        lastDir = dir;
    }
}

void SettingsDialog::on_btnImageSave_clicked(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), lastDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->imageDir->setText(dir);
    settings.setValue("ImagesDir", dir);

}

void SettingsDialog::on_listJavImport_doubleClicked(const QModelIndex &index)
{
    QString dir = javModel->data(index, 0).toString();
    javDirs.removeAll(dir);
    settings.remove("ImportJavDirs");
    saveArray(settings, "ImportJavDirs", javDirs);
    javModel->setStringList(javDirs);
}

void SettingsDialog::on_listPornImport_doubleClicked(const QModelIndex &index){
    updateArray(settings, "ImportPornDirs", pornDirs, pornModel, index);
}


void SettingsDialog::on_buttonBox_accepted()
{
    settings.setValue("ThumbWidth", ui->thumbWidth->text());
    settings.setValue("ThumbPercentage", ui->thumbPercentage->text());

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

}

void SettingsDialog::on_buttonBox_rejected()
{
    qDebug() << "cancelled";
}

void SettingsDialog::on_listHentaiImport_doubleClicked(const QModelIndex &index){
    updateArray(settings, "ImportHentaiDirs", hentaiDirs, hentaiModel, index);
}
