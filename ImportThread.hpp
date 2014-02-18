#ifndef IMPORTTHREAD_HPP
#define IMPORTTHREAD_HPP
#include <QThread>
#include <QSettings>
#include <QApplication>
#include <QDebug>
#include <QDirIterator>
#include <QSqlQuery>
#include <QCryptographicHash>

class ImportThread : public QThread
{
public:
    ImportThread();
    ~ImportThread();
private:
    void run();
};

#endif // IMPORTTHREAD_HPP
