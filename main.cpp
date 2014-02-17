#include "MainWindow.hpp"
#include <QApplication>
#include "ImportThread.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    ImportThread t;
    //QObject::connect(&t, SIGNAL(finished()), &a, SLOT(quit()));
    t.start();

    return a.exec();
}
