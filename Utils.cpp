#include "Utils.hpp"

namespace Utils {
  QSet<QString> getDirs(QSettings& settings) {
      QSet<QString> dirs;
      settings.beginGroup("Import");
      int size = settings.beginReadArray("dirs");
      for(int i=0; i < size; i++){
          settings.setArrayIndex(i);
          dirs.insert(settings.value("dir").toString());
      }
      settings.endArray();
      settings.endGroup();
      return dirs;
  }

  void saveDirs(QSettings& settings, QSet<QString> dirs) {
      settings.beginGroup("Import");
      settings.beginWriteArray("dirs");

      int i = 0;
      foreach(const QString& dir, dirs) {
          settings.setArrayIndex(i);
          settings.setValue("dir", dir);
          i++;
      }
      settings.endArray();
      settings.endGroup();
  }
}


