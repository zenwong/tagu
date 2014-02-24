#include "Worker.hpp"
#include "Globals.hpp"

Worker::Worker(QObject *parent) : QObject(parent), settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat) {
    _abort = false;
    _interrupt = false;

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/db");
    db.open();

//    nam = new QNetworkAccessManager(this);
//    post.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
//    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

void Worker::requestMethod(Worker::Task task){
    QMutexLocker locker(&mutex);
    _interrupt = true;
    _task = task;
    condition.wakeOne();
}

void Worker::abort(){
    QMutexLocker locker(&mutex);
    _abort = true;
    condition.wakeOne();
}

void Worker::doImport(){
    qDebug()<< "Start Importing Videos in Thread "<<thread()->currentThreadId();

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

    QSqlQuery insert(db);
    insert.prepare("insert into sync(tid,synced,json) values(?,?,?)");

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

                QFile file(iterator.filePath());
                file.open(QFile::ReadOnly);
                crypto.addData(file.read(9999));
                QByteArray hash = crypto.result();

                //qDebug() << hash.toHex();

                query.bindValue(0, iterator.fileInfo().baseName());
                query.bindValue(1, iterator.filePath());
                query.bindValue(2, hash.toHex());
                query.exec();

//                insert.bindValue(0, Table::VIDS);
//                insert.bindValue(1, 0);
//                QString json = QString(R"delimiter({"title":"%1","hash":"%2"})delimiter").arg(iterator.fileInfo().baseName()).arg(QString(hash.toHex()));
//                insert.bindValue(2, json);
//                insert.exec();
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
    QSqlQuery query(db);

    db.transaction();
    query.exec("select title,hash from vids where synced = 0");
    QJsonArray vids;
    while(query.next()) {
        QJsonObject obj;
        obj["title"] = query.value(0).toString();
        obj["hash"] = query.value(1).toString();
        vids.append(obj);
    }

    //qDebug() << vids;

    QJsonArray tags;
    query.exec("select * from SyncTags");
    while(query.next()) {
        QJsonObject obj;
        obj["title"] = query.value(0).toString();
        obj["tags"] = query.value(1).toString();
        tags.append(obj);
    }

    //qDebug() << tags;

    QJsonArray acts;
    query.exec("select * from SyncActs");
    while(query.next()) {
        QJsonObject obj;
        obj["title"] = query.value(0).toString();
        obj["tags"] = query.value(1).toString();
        acts.append(obj);
    }

    //qDebug() << acts;

    QJsonArray acttags;
    query.exec("select * from SyncActTags");
    while(query.next()) {
        QJsonObject obj;
        obj["act"] = query.value(0).toString();
        obj["tags"] = query.value(1).toString();
        acttags.append(obj);
    }
    db.commit();

    //qDebug() << acttags;

    QJsonObject json;
    if(vids.size() >= 1) {
        json["vids"] = vids;
    }
    if(tags.size() >= 1) {
        json["vidtags"] = tags;
    }
    if(acts.size() >= 1) {
        json["vidacts"] = acts;
    }
    if(acttags.size() >= 1) {
        json["acttags"] = acttags;
    }

    get.setUrl(QUrl("http://tagu.in"));
    nam->get(get);
    qDebug() << "netweork request";
}

void Worker::replyFinished(QNetworkReply *reply) {
  qDebug() << reply->readAll();
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
