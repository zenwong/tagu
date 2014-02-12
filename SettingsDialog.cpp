#include "SettingsDialog.hpp"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SettingsDialog){
    ui->setupUi(this);
}

void SettingsDialog::on_optGeneral_clicked(){
  ui->stackedWidget->setCurrentIndex(0);
}

void SettingsDialog::on_optView_clicked(){
    ui->stackedWidget->setCurrentIndex(1);
}

void SettingsDialog::on_optVideo_clicked(){
    ui->stackedWidget->setCurrentIndex(2);
}

SettingsDialog::~SettingsDialog(){
    delete ui;
}
