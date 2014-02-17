#include "FilesListView.hpp"

FilesListView::FilesListView()
{
    int i = 0;
    QDir dir("/mnt/seagate/pics/thumbs");
    QDirIterator iterator(dir.absolutePath(), QDirIterator::Subdirectories);
    while (iterator.hasNext()) {
        iterator.next();
        if (!iterator.fileInfo().isDir()) {
            QString filename = iterator.fileName();

            QListWidgetItem *item = new QListWidgetItem(filename, ui->listWidget);
            item->setSizeHint(size);
            ui->listWidget->insertItem(i, item);

            QLabel *lbl = new QLabel(filename);
            lbl->setPixmap(pixmapWithText(QPixmap(iterator.filePath()), filename.toStdString()));
            lbl->setAlignment(Qt::AlignCenter);
            ui->listWidget->setItemWidget(item, lbl);
            i++;
        }
    }
}
