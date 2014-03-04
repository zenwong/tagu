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

    void insertAct(QSqlDatabase, QString, QSqlTableModel*, QListView*);
    void insertTag(QSqlDatabase, QString, QSqlTableModel*, QListView*);
    void updateSyncedVids(QSqlDatabase, QByteArray);

    void doImport(QSqlDatabase);
    void doSearch();

private:
    int thumbWidth, thumbPercent;
    QNetworkAccessManager *nam;
    QNetworkRequest get, post;

public slots:
    void refreshVidList();

signals:
    void valueChanged(const QString &value);
    void finished();
    void importFinished();

};

#endif // WORKER_HPP
