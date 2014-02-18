#include "ImportThread.hpp"

ImportThread::ImportThread() {

}

ImportThread::~ImportThread() {
    quit();
    wait();
}

// possible optimization spawn 1 thread per physical disk
// single threaded if files are on a single disk

void ImportThread::run() {
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

