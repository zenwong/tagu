#include "FFMpeg.hpp"
#include <QDebug>
#include <QThread>
#include <QImage>
#include <QPainter>
#include <QImageWriter>
#include <climits>

FFMpeg::FFMpeg(QSqlDatabase db){
    this->db = db;
    av_register_all();
    avcodec_register_all();
    av_log_set_level(AV_LOG_QUIET);
    filters << "*.avi" << "*.wmv" << "*.mp4" << "*.mkv" << "*.flv" << "*.mpg" << "*.mpeg" << "*.mov"  << "*.asf" << "*.rmvb" << "*.ogm";
}

int FFMpeg::run() {
//    imported = 0;
//    foreach(const QString& d, config.javDirs) {
//        QDir dir(d);
//        QDirIterator iterator(dir.absolutePath(), filters,  QDir::AllDirs|QDir::Files, QDirIterator::Subdirectories);

//        while (iterator.hasNext()) {
//            iterator.next();
//            if (iterator.fileInfo().isFile()) {
//                qDebug() << imported << ") " <<  iterator.fileName();
//                //qDebug() << "before passing to parse() " << QThread::currentThreadId();
//                parse(iterator.filePath(), iterator.fileInfo().baseName());
//                imported++;
//            }
//        }
//    }

//    return imported;
}

int FFMpeg::doImport(Settings config){
    config.reload();
    qDebug() << config.javDirs;
    QCryptographicHash crypto(QCryptographicHash::Sha1);
    QSqlQuery query(db);
    query.prepare("insert into vids(title,path,hash) values(?,?,?)");
    QSqlQuery insert(db);
    insert.prepare("insert into sync(tid,synced,json) values(?,?,?)");

    QStringList dirs;
    foreach(const QString& dir, config.javDirs) dirs << dir;
    foreach(const QString& dir, config.pornDirs) dirs << dir;
    foreach(const QString& dir, config.hentaiDirs) dirs << dir;

    QString thumbDir;
    if(config.imageDir.endsWith('/')) {
        thumbDir = config.imageDir + "thumbs" + QDir::separator();
    } else {
        thumbDir = config.imageDir + QDir::separator() + "thumbs" + QDir::separator();
    }

    int importedVidsCount = 0;
    foreach(const QString& d, dirs) {
        QDir dir(d);
        QDirIterator iterator(dir.absolutePath(), filters,  QDir::AllDirs|QDir::Files, QDirIterator::Subdirectories);

        db.transaction();
        while (iterator.hasNext()) {
            iterator.next();
            if (!iterator.fileInfo().isDir()) {
                parse(iterator.filePath(), iterator.fileInfo().baseName());

                QFile file(iterator.filePath());
                file.open(QFile::ReadOnly);
                crypto.addData(file.read(9999));
                QByteArray hash = crypto.result();

                query.bindValue(0, iterator.fileInfo().baseName());
                query.bindValue(1, iterator.filePath());
                query.bindValue(2, hash.toHex());
                query.exec();

                importedVidsCount++;

                qDebug() << importedVidsCount << ") " << iterator.filePath();
            }
        }
        db.commit();
    }

    db.transaction();
    query.exec("delete from search");
    query.exec("insert into search(docid,title,tags,acts) select vid,title,tags,acts from LibraryView");
    db.commit();

    return importedVidsCount;
}

void FFMpeg::parse(QString path, QString name) {
    this->path = path; this->name = name;
    //qDebug() << "inside parse() " << QThread::currentThreadId();

    fctx = avformat_alloc_context();

    avformat_open_input(&fctx, path.toStdString().c_str(), NULL, NULL);
    avformat_find_stream_info(fctx, NULL);
    frame = av_frame_alloc();
    frameRGB = av_frame_alloc();

    for(int i = 0; i < fctx->nb_streams; i++) {
        if(fctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            stream = i;
            break;
        }
    }

    cctx = fctx->streams[stream]->codec;
    codec = avcodec_find_decoder(cctx->codec_id);
    avcodec_open2(cctx, codec, NULL);

    duration = fctx->duration / AV_TIME_BASE;
    step = duration / (rowCount * colCount + 2);

    numBytes = avpicture_get_size(pixfmt, width, height);
    buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
    avpicture_fill((AVPicture *)frameRGB, buffer, pixfmt, width, height);

    //sctx = sws_getCachedContext(sctx, width, height, cctx->pix_fmt, width, height, pixfmt, SWS_BILINEAR, NULL, NULL, NULL);
    sctx = sws_getCachedContext(sctx, cctx->width, cctx->height, cctx->pix_fmt, width, height, pixfmt, SWS_BICUBIC, NULL, NULL, NULL);

    seek();

    av_free(buffer);
    av_free(frameRGB);
    av_free(frame);
    avcodec_close(cctx);
    avformat_close_input(&fctx);
}

void FFMpeg::seek() {

    for(int i = step; i < duration; i += step) {
        //qDebug() << i;
        int64_t timestamp = av_rescale(((uint64_t)i)*1000, fctx->streams[stream]->time_base.den, fctx->streams[stream]->time_base.num)/1000;
        //avformat_seek_file(fctx, stream, 0, timestamp, INT64_MAX, AVSEEK_FLAG_FRAME);
        avformat_seek_file(fctx, stream, 0, timestamp, 0x7fffffffffffffffLL, AVSEEK_FLAG_FRAME);
        avcodec_flush_buffers(cctx);

        saveJpeg(i);
    }
}

void FFMpeg::saveJpeg(int seconds) {
    AVPacket packet;
    int frameComplete = 0;

    while((av_read_frame(fctx, &packet)>=0) && !frameComplete) {
        if(packet.stream_index== stream) {
            avcodec_decode_video2(cctx, frame, &frameComplete, &packet);
            if(frameComplete) {
                //QImage myImage(width, height, QImage::Format_RGB32);
                QImage myImage(width, height, QImage::Format_ARGB32_Premultiplied);

                sws_scale(sctx, frame->data, frame->linesize, 0, cctx->height,  frameRGB->data, frameRGB->linesize);

                uint8_t *src = (uint8_t *)(frameRGB->data[0]);
                for (int y = 0; y < height; y++) {
                    QRgb *scanLine = (QRgb *) myImage.scanLine(y);
                    for (int x = 0; x < width; x=x+1) {
                        scanLine[x] = qRgb(src[3*x], src[3*x+1], src[3*x+2]);
                    }
                    src += frameRGB->linesize[0];
                }

                QString tmp = "/tmp/ffmpeg/";
                tmp.append(name);
                tmp.append("-");
                tmp.append(QString::number(seconds));
                tmp.append(".jpg");

                QImageWriter writer(tmp);
                if(!writer.write(myImage)) {
                    qDebug() << writer.errorString();
                }

            }
        }

        av_free_packet(&packet);
    }
}
