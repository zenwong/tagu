#ifndef FILESLISTVIEW_HPP
#define FILESLISTVIEW_HPP
#include <QListView>

class FilesListView : public QListView
{
public:
    FilesListView();

    void thumbnailView();
    void rowView();
    void compactView();
    void actressView();
};

#endif // FILESLISTVIEW_HPP
