#ifndef MULTICOMPLETER_HPP
#define MULTICOMPLETER_HPP

#include <QCompleter>
#include <QLineEdit>

class MultiCompleter : public QCompleter
{
   Q_OBJECT

   public:
       QString pathFromIndex( const QModelIndex& index ) const;
       QStringList splitPath( const QString& path ) const;

};

#endif // MULTICOMPLETER_HPP
