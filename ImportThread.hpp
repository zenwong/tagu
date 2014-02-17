#ifndef IMPORTTHREAD_HPP
#define IMPORTTHREAD_HPP
#include <QThread>
#include <QSettings>
#include <QApplication>
#include <QDebug>

class ImportThread : public QThread
{
private:
    void run();
};

#endif // IMPORTTHREAD_HPP
