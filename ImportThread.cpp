#include "ImportThread.hpp"

void ImportThread::run() {
    QSettings settings(QApplication::instance()->applicationDirPath() + "/settings.ini", QSettings::IniFormat);
    QList<QString> dirs;
    int len = settings.beginReadArray("ImportDirs");
    for (int i = 0; i < len; ++i) {
        settings.setArrayIndex(i);
        dirs.append(settings.value("dir").toString());
    }
    settings.endArray();

    qDebug() << dirs;
}
