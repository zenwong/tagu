#include "Settings.hpp"

Settings::Settings(){
    QFile file(QCoreApplication::applicationDirPath() + "/settings.dat");
    if(file.exists()) {
        file.open(QIODevice::ReadOnly);
        QDataStream in(&file);
        in >> this->email >> this->password >> this->session >> this->thumbWidth >> this->thumbPercent
            >> this->windowGeometry >> this->windowState >> this->javDirs >> this->pornDirs >> this->hentaiDirs;
    }
}

Settings::~Settings() {
    QFile file(QCoreApplication::applicationDirPath() + "/settings.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << this->email;
    out << this->password;
    out << this->session;
    out << this->thumbWidth;
    out << this->thumbPercent;
    out << this->windowGeometry;
    out << this->windowState;
    out << this->javDirs;
    out << this->pornDirs;
    out << this->hentaiDirs;
}

void Settings::clear() {
  javDirs.clear();
  pornDirs.clear();
  hentaiDirs.clear();
}
