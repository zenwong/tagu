#include "Worker.hpp"
using namespace ffmpegthumbnailer;

Worker::Worker(QObject *parent) : QObject(parent) {
    _abort = false;
    _interrupt = false;

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/db");
    db.open();
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
    qDebug()<< "Start Importing Videos in Thread "<<thread()->currentThreadId();

    QSettings settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
    QList<QString> dirs;
    int len = settings.beginReadArray("ImportJavDirs");
    for (int i = 0; i < len; ++i) {
        settings.setArrayIndex(i);
        dirs.append(settings.value("dir").toString());
    }
    settings.endArray();

    len = settings.beginReadArray("ImportPornDirs");
    for (int i = 0; i < len; ++i) {
        settings.setArrayIndex(i);
        dirs.append(settings.value("dir").toString());
    }
    settings.endArray();

    len = settings.beginReadArray("ImportHentaiDirs");
    for (int i = 0; i < len; ++i) {
        settings.setArrayIndex(i);
        dirs.append(settings.value("dir").toString());
    }
    settings.endArray();


//    thumbWidth = settings.value("ThumbWidth").toInt();
//    thumbPercent = settings.value("ThumbPercentage").toInt();

    QString thumbDir = settings.value("ImagesDir").toString() + QDir::separator() + "thumbs" + QDir::separator();

    //qDebug() << dirs;

    QCryptographicHash crypto(QCryptographicHash::Sha1);

    QStringList filters;
    filters << "*.avi" << "*.wmv" << "*.mp4" << "*.mkv" << "*.flv" << "*.mpg" << "*.mpeg" << "*.mov"
               << "*.asf" << "*.rmvb" << "*.ogm";

    QSqlQuery query(db);
    query.prepare("insert into vids(title,path,hash) values(?,?,?)");

    foreach(QString d, dirs) {
        QDir dir(d);
        QDirIterator iterator(dir.absolutePath(), filters,  QDir::AllDirs|QDir::Files, QDirIterator::Subdirectories);

        db.transaction();
        while (iterator.hasNext()) {
            iterator.next();
            if (!iterator.fileInfo().isDir()) {
                //qDebug() << iterator.fileInfo().baseName();

                // check for files created after last import date in qsettings
                // qDebug() << iterator.fileInfo().created();

                QString savePath = thumbDir + iterator.fileInfo().baseName() + ".jpg";
                QFile f(savePath);
                if(!f.exists()) {
                    VideoThumbnailer thumb(400, false, true, 6, true);
                    thumb.setSeekPercentage(30);
                    thumb.generateThumbnail(iterator.filePath().toStdString(), Jpeg, savePath.toStdString());
                }

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
        db.commit();
        emit importFinished();
    }

    db.transaction();
    query.exec("delete from search");
    query.exec("insert into search(docid,title,tags,acts) select vid,title,tags,acts from LibraryView");
    db.commit();
}

void Worker::doSync(){

}

void Worker::doSearch(){

}

void Worker::mainLoop(){

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
