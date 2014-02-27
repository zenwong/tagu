#include "SignupDialog.hpp"
#include "ui_SignupDialog.h"
#include <QMessageBox>

SignupDialog::SignupDialog(QWidget *parent, QNetworkAccessManager *nam) : QDialog(parent), ui(new Ui::SignupDialog),
    settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat){
    ui->setupUi(this);
    this->nam = nam;
    post.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
}

void SignupDialog::on_btnSignup_clicked(){
    post.setUrl(QUrl("http://tagu.in/app/signup"));
    QString data = "email=" + ui->editEmail->text().simplified() + "&password=" + ui->editPassword->text().simplified();
    signupReply = nam->post(post, data.toLatin1());
    connect(signupReply, SIGNAL(readyRead()), this, SLOT(onSignupReply()));
}

void SignupDialog::onSignupReply() {
    QByteArray resp = signupReply->readAll();

    if(resp == "signup ok") {
        qDebug() << "client signup ok";
        settings.setValue("email", ui->editEmail->text());
        settings.setValue("password", ui->editPassword->text());
    }

    if(resp == "username taken") {
        qDebug() << "client username taken";
    }
}

void SignupDialog::onLoginReply() {

}

void SignupDialog::on_btnLogin_clicked(){

}

SignupDialog::~SignupDialog(){
    delete ui;
}

void SignupDialog::on_editEmail_editingFinished(){
    QRegExp mailREX("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b");
    mailREX.setCaseSensitivity(Qt::CaseInsensitive);
    mailREX.setPatternSyntax(QRegExp::RegExp);

    if(!mailREX.exactMatch(ui->editEmail->text())) {
        QMessageBox::warning(this, tr("Invalid Email"), tr("Please Enter a Valid Email Address"), QMessageBox::Ok);
        ui->editEmail->setFocus();
    } else {
    }

}
