#ifndef IMPORTTHREAD_HPP
#define IMPORTTHREAD_HPP
#include <QThread>
#include <QSettings>
#include <QApplication>
#include <QDebug>
#include <QDirIterator>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QDateTime>

class ImportThread : public QThread
{
public:
    ImportThread();
    ~ImportThread();
private:
    void run();
    void genThumbnail(QString path, int percentage);
};

#endif // IMPORTTHREAD_HPP
