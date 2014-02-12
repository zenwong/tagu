#ifndef SETTINGSDIALOG_HPP
#define SETTINGSDIALOG_HPP

#include <QDialog>

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
    void on_optGeneral_clicked();

    void on_optView_clicked();

    void on_optVideo_clicked();

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_HPP
