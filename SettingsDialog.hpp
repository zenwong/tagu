#ifndef SETTINGSDIALOG_HPP
#define SETTINGSDIALOG_HPP

#include <QDialog>
#include <QSettings>
#include <QStringListModel>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private slots:
    void on_btnJavAdd_clicked();
    void on_btnPornAdd_clicked();
    void on_btnHentaiAdd_clicked();
    void on_btnImageSave_clicked();
    void on_listJavImport_doubleClicked(const QModelIndex &index);
    void on_listPornImport_doubleClicked(const QModelIndex &index);
    void on_listHentaiImport_doubleClicked(const QModelIndex &index);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::SettingsDialog *ui;
    QSettings settings;
    QStringList javDirs, pornDirs, hentaiDirs;
    QStringListModel *javModel, *pornModel, *hentaiModel;
    QString lastDir;
};

static void saveArray(QSettings& settings, QString array, QStringList& list) {
  settings.beginWriteArray(array);
   for (int i = 0; i < list.size(); ++i) {
       settings.setArrayIndex(i);
       settings.setValue("dir", list.at(i));
   }
   settings.endArray();
}

static void updateArray(QSettings& settings, QString array, QStringList& list, QStringListModel *model, const QModelIndex& index) {
    QString dir = model->data(index, 0).toString();
    list.removeAll(dir);
    settings.remove(array);
    saveArray(settings, array, list);
    model->setStringList(list);
}

static bool exists(const QStringList& list, QString word) {
    bool exists = false;
    foreach(QString item, list) {
        if(item == word) {
            //qDebug() << item << " : " << word;
            exists = true;
            break;
        }
    }
    return exists;
}

#endif // SETTINGSDIALOG_HPP
