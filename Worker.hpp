#ifndef WORKER_HPP
#define WORKER_HPP
#include <QThread>
#include <QSettings>
#include <QApplication>
#include <QDebug>
#include <QDirIterator>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QDateTime>
#include <QMutex>
#include <QWaitCondition>
#include <QTimer>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <libffmpegthumbnailer/videothumbnailer.h>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QListView>
#include <QSqlTableModel>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);
    enum Task {
        Import,
        Sync,
        Search
    };
    void requestMethod(Task method);
    void abort();

    void insertAct(QString, QSqlTableModel*, QListView*);


private:
    Task _task;
    bool _abort;
    bool _interrupt;
    QMutex mutex;
    QWaitCondition condition;
    QSqlDatabase db;
    int thumbWidth, thumbPercent;
    QSettings settings;
    QNetworkAccessManager *nam;
    QNetworkRequest get, post;

    void doImport();
    void doSync();
    void doSearch();

signals:
    void valueChanged(const QString &value);
    void finished();
    void importFinished();

public slots:
    void mainLoop();
    void replyFinished(QNetworkReply*);
    void gotData(QNetworkReply *trigger);
};

#endif // WORKER_HPP
