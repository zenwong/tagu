#include "Worker.hpp"
#include "Globals.hpp"
using namespace ffmpegthumbnailer;

Worker::Worker(QObject *parent) : QObject(parent) {
    nam = new QNetworkAccessManager(this);
    post.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    post.setRawHeader("Cookie", "session=$2y$05$tDIxAIPo9I9s5fURHfN8.epFzM5Civu2InhlRLGJ5US4kiCNZ/o9m");
}

void Worker::insertAct(QSqlDatabase db, QString act, QSqlTableModel *vidTable, QListView *list) {
   if(act.size() > 2) {
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
}

void Worker::insertTag(QSqlDatabase db, QString tag, QSqlTableModel *vidTable, QListView *list) {
    if(tag.size() > 1) {
        QSqlQuery insert(db);
        QSqlQuery insert2(db);
        QSqlQuery query(db);

        insert.prepare("insert into tags(name) values(?)");
        insert2.prepare("insert into vidtags(vid,tid) values(?,?)");
        query.prepare("select _id from tags where name = ?");

        int tid = 0, vid = 0;
        QItemSelection selected( list->selectionModel()->selection() );
        qDebug() << selected;

        db.transaction();
        foreach(QModelIndex index, selected.indexes()) {
            vid = vidTable->data(vidTable->index(index.row(), 0)).toInt();

            insert.bindValue(0, tag);
            insert.exec();

            query.bindValue(0, tag);
            query.exec();
            query.first();
            tid = query.value(0).toInt();

            qDebug() << "vid: " << vid  << ", tid: " << tid;

            insert2.bindValue(0, vid);
            insert2.bindValue(1, tid);
            insert2.exec();

        }
        db.commit();
    }
}

void Worker::updateSyncedVids(QSqlDatabase db, QByteArray json) {
  //qDebug() << json;
  QSqlQuery query(db);
  db.transaction();
  query.exec("update vids set synced = 1");
  query.exec("update vidtags set synced = 1");
  query.exec("update vidacts set synced = 1");
  db.commit();

  QJsonParseError err;
  QJsonDocument doc = QJsonDocument::fromJson(json, &err);

  QJsonObject root = doc.object();

  QJsonValue vidtags = root.value("vidtags");
  QJsonArray vt = vidtags.toArray();

  QSqlQuery select(db);
  QSqlQuery select2(db);
  QSqlQuery select3(db);
  QSqlQuery insert(db);
  QSqlQuery insert2(db);
  QSqlQuery insert3(db);
  QSqlQuery insert4(db);

  select.prepare("select _id from vids where title = ?");
  select2.prepare("select _id from tags where name = ?");
  select3.prepare("select _id from acts where name = ?");

  insert.prepare("insert into vidtags(vid,tid) values(?,?)");
  insert2.prepare("insert into vidacts(vid,aid) values(?,?)");
  insert3.prepare("insert into tags(name) values(?)");
  insert4.prepare("insert into acts(name) values(?)");

  int vid, tid, aid;
  db.transaction();
  foreach(QJsonValue v, vt) {
      QJsonObject obj = v.toObject();
      QJsonValue title = obj.value("title");
      QJsonValue tag  = obj.value("tag");

      insert3.bindValue(0, tag.toString());
      insert3.exec();

      select.bindValue(0, title.toString());
      select.exec();
      select.first();
      vid = select.value(0).toInt();

      select2.bindValue(0, tag.toString());
      select2.exec();
      select2.first();
      tid = select2.value(0).toInt();

      insert.bindValue(0, vid);
      insert.bindValue(1, tid);
      insert.exec();
  }

  QJsonValue vidacts = root.value("vidacts");
  QJsonArray va = vidacts.toArray();
  foreach(QJsonValue v, va) {
      QJsonObject obj = v.toObject();
      QJsonValue title = obj.value("title");
      QJsonValue act  = obj.value("act");

      insert4.bindValue(0, act.toString());
      insert4.exec();

      select.bindValue(0, title.toString());
      select.exec();
      select.first();
      vid = select.value(0).toInt();

      select3.bindValue(0, act.toString());
      select3.exec();
      select3.first();
      aid = select3.value(0).toInt();

      insert2.bindValue(0, vid);
      insert2.bindValue(1, aid);
      insert2.exec();
  }

  db.commit();
}

void Worker::doImport(QSqlDatabase db, Settings config){
    QCryptographicHash crypto(QCryptographicHash::Sha1);
    QStringList filters;
    filters << "*.avi" << "*.wmv" << "*.mp4" << "*.mkv" << "*.flv" << "*.mpg" << "*.mpeg" << "*.mov"  << "*.asf" << "*.rmvb" << "*.ogm";

    QSqlQuery query(db);
    query.prepare("insert into vids(title,path,hash) values(?,?,?)");

    QSqlQuery insert(db);
    insert.prepare("insert into sync(tid,synced,json) values(?,?,?)");

    QStringList dirs;
    foreach(const QString& dir, config.javDirs) dirs << dir;
    foreach(const QString& dir, config.pornDirs) dirs << dir;
    foreach(const QString& dir, config.hentaiDirs) dirs << dir;

    QString thumbDir;
    if(config.imageDir.endsWith('/')) {
        thumbDir = config.imageDir + "thumbs" + QDir::separator();
    } else {
        thumbDir = config.imageDir + QDir::separator() + "thumbs" + QDir::separator();
    }

    foreach(const QString& d, dirs) {
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
                    VideoThumbnailer thumb(config.thumbWidth, false, true, 6, true);
                    thumb.setSeekPercentage(config.thumbPercent);
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
    }

    db.transaction();
    query.exec("delete from search");
    query.exec("insert into search(docid,title,tags,acts) select vid,title,tags,acts from LibraryView");
    db.commit();
}

QSqlQueryModel* Worker::doSearch(QSqlDatabase db, const QString& txt){
    QStringList terms = txt.split(" ");

    QString sql = "select * from search where search MATCH '" + txt.simplified() + "*'";
    QSqlQueryModel *model = new QSqlQueryModel;

    if(terms.size() == 1) {
        if(txt.size() > 1) {
            QSqlQuery q(sql, db);
            model->setQuery(q);
        }
    }

    if(terms.size() > 1) {
        for(int i = 1; i < terms.size(); i++) {
            if(terms.at(i).simplified().size() > 1) {
                sql += " intersect select * from search where search MATCH '" + terms.at(i).simplified() + "*'";
            }
            QSqlQuery q(sql, db);
            model->setQuery(q);
        }
    }

    return model;
}
