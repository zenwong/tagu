#ifndef VIDEOMODEL_HPP
#define VIDEOMODEL_HPP
#include <QAbstractItemModel>
#include <QSqlDatabase>

struct VideoItem {
    VideoItem(int id, int rating, QString Title, QString Path, QString Filename, QString Tags, QString Acts) : title(Title)
      , path(Path), filename(Filename), tags(Tags), acts(Acts) {}

    QString title, path, filename, tags, acts;
    int id, rating;
};

class VideoModel : public QAbstractItemModel
{
    Q_OBJECT
   public:
       VideoModel(QSqlDatabase db, QObject *parent = 0);

       enum DataRole
       {
           TAGS_ROLE = Qt::UserRole + 100,
           ACTS_ROLE = Qt::UserRole + 101,
           PATH_ROLE = Qt::UserRole + 102
       };

       QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
       virtual QModelIndex parent(const QModelIndex& index) const override;
       virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
       virtual int rowCount(const QModelIndex& parent) const override;
       virtual int columnCount(const QModelIndex& index) const override;

       virtual bool setData (const QModelIndex& index, const QVariant& value, int role = Qt::EditRole)
       {
           if (role == Qt::EditRole)
           {
               VideoItem* item = items.at(index.row());
               item->title = value.toString();
               return true;
           }
           return false;
       }

       virtual Qt::ItemFlags flags (const QModelIndex& index) const
       {
           Q_UNUSED(index);
           return Qt::ItemIsSelectable    |
                   Qt::ItemIsEditable      |
                   Qt::ItemIsEnabled;
       }

   private:
       QList<VideoItem*> items;
};

#endif // VIDEOMODEL_HPP
