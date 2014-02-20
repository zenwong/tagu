#include "Worker.hpp"

Worker::Worker(QObject *parent) : QObject(parent) {
    _abort = false;
    _interrupt = false;
}

void Worker::requestMethod(Worker::Task task){
    qDebug()<<"Request worker Method"<< task <<"in Thread "<<thread()->currentThreadId();
    QMutexLocker locker(&mutex);
    _interrupt = true;
    _task = task;
    condition.wakeOne();
}

void Worker::abort(){
    qDebug()<<"Request worker aborting in Thread "<<thread()->currentThreadId();
    QMutexLocker locker(&mutex);
    _abort = true;
    condition.wakeOne();
}

void Worker::doImport(){
    qDebug()<<"Starting Method1 in Thread "<<thread()->currentThreadId();

    QSettings settings(QApplication::instance()->applicationDirPath() + "/settings.ini", QSettings::IniFormat);
    QList<QString> dirs;
    int len = settings.beginReadArray("ImportDirs");
    for (int i = 0; i < len; ++i) {
        settings.setArrayIndex(i);
        dirs.append(settings.value("dir").toString());
    }
    settings.endArray();

    QCryptographicHash crypto(QCryptographicHash::Sha1);

    QStringList filters;
    filters << "*.avi" << "*.wmv" << "*.mp4" << "*.mkv" << "*.flv" << "*.mpg" << "*.mpeg" << "*.mov"
               << "*.asf" << "*.rmvb" << "*.ogm";

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/root/devel/cpp/qt/Tagu/db");
    db.open();


    if(db.transaction()) {
        QSqlQuery query(db);
        query.prepare("insert into vids(title,path,hash) values(?,?,?)");

        for(QString d: dirs) {
            QDir dir(d);
            QDirIterator iterator(dir.absolutePath(), filters,  QDir::AllDirs|QDir::Files, QDirIterator::Subdirectories);
            while (iterator.hasNext()) {
                iterator.next();
                if (!iterator.fileInfo().isDir()) {
                    //qDebug() << iterator.fileInfo().baseName();

                    // check for files created after last import date in qsettings
                    qDebug() << iterator.fileInfo().created();

                    QFile file(iterator.filePath());
                    file.open(QFile::ReadOnly);
                    crypto.addData(file.read(9999));
                    QByteArray hash = crypto.result();

                    //qDebug() << hash.toHex();

                    query.bindValue(0, iterator.fileInfo().baseName());
                    query.bindValue(1, iterator.filePath());
                    query.bindValue(2, hash.toHex());
                    query.exec();
                }
            }
        }
        db.commit();
    }

}

void Worker::doSync()
{
    qDebug()<<"Starting Method2 in Thread "<<thread()->currentThreadId();

    for (int i = 0; i < 20; i ++) {

        mutex.lock();
        bool abort = _abort;
        bool interrupt = _interrupt;
        mutex.unlock();

        if (abort || interrupt) {
            qDebug()<<"Interrupting Method2 in Thread "<<thread()->currentThreadId();
            break;
        }

        QEventLoop loop;
        QTimer::singleShot(1000, &loop, SLOT(quit()));
        loop.exec();

        emit valueChanged(QString::number(i));
    }
}

void Worker::doSearch()
{
    qDebug()<<"Starting Method3 in Thread "<<thread()->currentThreadId();

    for (int i = 0; i < 30; i ++) {

        mutex.lock();
        bool abort = _abort;
        bool interrupt = _interrupt;
        mutex.unlock();

        if (abort || interrupt) {
            qDebug()<<"Interrupting Method3 in Thread "<<thread()->currentThreadId();
            break;
        }

        QEventLoop loop;
        QTimer::singleShot(1000, &loop, SLOT(quit()));
        loop.exec();

        emit valueChanged(QString::number(i));
    }
}

void Worker::mainLoop(){
    qDebug()<<"Starting worker mainLoop in Thread "<<thread()->currentThreadId();

    forever {

        mutex.lock();
        if (!_interrupt && !_abort) {
            condition.wait(&mutex);
        }
        _interrupt = false;

        if (_abort) {
            qDebug()<<"Aborting worker mainLoop in Thread "<<thread()->currentThreadId();
            emit finished();
            return;
        }

        Task task = _task;
        mutex.unlock();

        switch(task) {
        case Import:
            doImport();
            break;
        case Sync:
            doSync();
            break;
        case Search:
            doSearch();
            break;
        }
    }
}
