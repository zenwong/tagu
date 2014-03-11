#ifndef SIGNUPDIALOG_HPP
#define SIGNUPDIALOG_HPP

#include <QDebug>
#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegExp>
#include <QRegExpValidator>
#include <QToolTip>
#include <QSettings>

namespace Ui {
class SignupDialog;
}

class SignupDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SignupDialog(QWidget *parent = 0, QNetworkAccessManager* nam = 0);
    ~SignupDialog();

private slots:
    void on_btnSignup_clicked();
    void on_btnLogin_clicked();
    void onSignupReply();
    void onLoginReply();
    void on_editEmail_editingFinished();

private:
    Ui::SignupDialog *ui;
    QNetworkAccessManager *nam;
    QNetworkRequest post;
    QNetworkReply *signupReply, *loginReply;
    QRegExpValidator emailValidater;
    QSettings settings;
};

#endif // SIGNUPDIALOG_HPP
