#include "Worker.hpp"
#include "Globals.hpp"
using namespace ffmpegthumbnailer;

Worker::Worker(QObject *parent) : QObject(parent), settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat) {
    _abort = false;
    _interrupt = false;

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QCoreApplication::applicationDirPath() + "/db");
    db.open();

    nam = new QNetworkAccessManager(this);
    post.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    post.setRawHeader("Cookie", "session=$2y$05$tDIxAIPo9I9s5fURHfN8.epFzM5Civu2InhlRLGJ5US4kiCNZ/o9m");

    //connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

void Worker::insertAct(QString act, QSqlTableModel *vidTable, QListView *list) {
    QSqlQuery insert(db);
    QSqlQuery insert2(db);
    QSqlQuery query(db);

    insert.prepare("insert into acts(name) values(?)");
    insert2.prepare("insert into vidacts(vid,aid) values(?,?)");
    query.prepare("select _id from acts where name = ?");

    int aid = 0, vid = 0;
    QItemSelection selected( list->selectionModel()->selection() );

    db.transaction();
    foreach(QModelIndex index, selected.indexes()) {
        vid = vidTable->data(vidTable->index(index.row(), 0)).toInt();
        //qDebug() << "vid: " << vid;

        insert.bindValue(0, act);
        insert.exec();

        query.bindValue(0, act);
        query.exec();
        query.first();
        aid = query.value(0).toInt();
        //qDebug() << "aid: " << aid << ", name: '" << act << "'";

        insert2.bindValue(0, vid);
        insert2.bindValue(1, aid);
        insert2.exec();

    }
    db.commit();
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

                QString savePath = thumbDir + iterator.fileInfo().baseName() + ".jpg";
                QFile f(savePath);
                if(!f.exists()) {
                    // TODO mkdir /tmp/thumbs then generate thumbs in tmp directory
                    // when finished move thumbs to real image directory and delete tmp dir

                    // TOGO allow importing of all kinds of files
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

    QJsonDocument doc(json);
    qDebug() << doc.toJson();

    post.setUrl(QUrl("http://tagu.in/sync"));
    nam->post(post, doc.toJson());
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

void Worker::gotData(QNetworkReply *trigger) {
    qDebug() << "got data";
}
