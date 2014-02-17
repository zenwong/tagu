#ifndef DBHELPER_HPP
#define DBHELPER_HPP
#include <QObject>

class DbHelper: QObject
{
    Q_OBJECT
public:
    DbHelper(QObject *parent);

public slots:
    void onEditTag_returnPressed(QString);
};

#endif // DBHELPER_HPP
