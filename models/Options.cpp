#include "Options.hpp"

Options::Options() : settings(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat) {
    thumbWidth = settings.value("Images/thumbWidth").toInt();
    rowCount     = settings.value("Images/rowCount").toInt();
    colCount      = settings.value("Images/colCount").toInt();
    padding       = settings.value("Images/padding").toInt();
    margin         = settings.value("Images/margin").toInt();
    imageDir      = settings.value("Images/imageDir").toString();

    email = settings.value("User/email").toString();
    password = settings.value("User/password").toString();
    session = settings.value("User/session").toString();

    lastView = settings.value("State/lastView").toString();
    winState = settings.value("State/winState").toByteArray();
    winPosition = settings.value("State/winPosition").toByteArray();

    settings.beginGroup("Import");
    int size = settings.beginReadArray("dirs");
    for(int i=0; i < size; i++){
        settings.setArrayIndex(i);
        importDirs.insert(settings.value("dir").toString());
    }
    settings.endArray();
    settings.endGroup();

    //qDebug() << "Options ctor imageDir: " << imageDir;
}

Options::~Options() {
    settings.setValue("Images/thumbWidth", thumbWidth);
    settings.setValue("Images/rowCount", rowCount);
    settings.setValue("Images/colCount", colCount);
    settings.setValue("Images/padding", padding);
    settings.setValue("Images/margin", margin);
    settings.setValue("Images/imageDir", imageDir);

    //qDebug() << "Options dtor imageDir: " << imageDir;

    settings.setValue("User/email", email);
    settings.setValue("User/password", password);
    settings.setValue("User/session", session);

    settings.setValue("State/lastView", lastView);
    settings.setValue("State/winState", winState);
    settings.setValue("State/winPosition", winPosition);

    settings.beginGroup("Import");
    settings.beginWriteArray("dirs");

    int i = 0;
    foreach(const QString& dir, importDirs) {
        settings.setArrayIndex(i);
        settings.setValue("dir", dir);
        i++;
    }
    settings.endArray();
    settings.endGroup();

    settings.sync();
}
