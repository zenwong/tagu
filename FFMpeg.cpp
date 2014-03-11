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

int FFMpeg::doImport(){
    width = 400;
    height = 225;
    QSettings opts(QCoreApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
    rowCount = opts.value(Key::Rows).toInt();
    colCount = opts.value(Key::Columns).toInt();

    QCryptographicHash crypto(QCryptographicHash::Sha1);
    QSqlQuery query(db);
    query.prepare("insert into vids(title,path,hash) values(?,?,?)");
    QSqlQuery insert(db);
    insert.prepare("insert into sync(tid,synced,json) values(?,?,?)");


    QString imgDir = opts.value(Key::ImageDir).toString();
    if(imgDir[imgDir.size()] == QDir::separator().toLatin1()) {
        screenDir = imgDir + "screens" + QDir::separator();
        thumbDir = imgDir + "thumbs" + QDir::separator();
    } else {
        screenDir = imgDir + QDir::separator() + "screens" + QDir::separator();
        thumbDir = imgDir + QDir::separator() + "thumbs" + QDir::separator();
    }

    auto dirs = Utils::getDirs(opts);
    qDebug() << dirs;

    int importedVidsCount = 0;
    for(const auto& d: dirs) {
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

                //qDebug() << importedVidsCount << ") " << iterator.filePath();
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

    fctx = avformat_alloc_context();

    avformat_open_input(&fctx, path.toStdString().c_str(), NULL, NULL);
    avformat_find_stream_info(fctx, NULL);
    frame = av_frame_alloc();
    frameRGB = av_frame_alloc();

    for(unsigned int i = 0; i < fctx->nb_streams; i++) {
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

    //qDebug() << "duration: " << duration << ", step: " << step;

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

    QSize size(1600, 900);
    QImage image(size, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&image);

    int x = 0, y = 0;
    int num = 0;
    int frames = rowCount * colCount;
    int _step = step;

    for(int i = 0; i < frames; i++) {
        int64_t timestamp = av_rescale(((uint64_t) _step) * 1000, fctx->streams[stream]->time_base.den, fctx->streams[stream]->time_base.num)/1000;
        avformat_seek_file(fctx, stream, 0, timestamp, 0x7fffffffffffffffLL, AVSEEK_FLAG_FRAME);
        avcodec_flush_buffers(cctx);

        auto image = getJpeg();
        painter.drawImage(x, y, image);

        // TODO select best thumbnail from all the images generated from screenshot
        if(i == 4) {
            QSize s(400,225);
            QImage thumb(s, QImage::Format_ARGB32_Premultiplied);
            QPainter p2(&thumb);
            QRect r(0,0,400,225);
            p2.drawImage(r, image);

            QImageWriter iw(thumbDir + name + ".jpg");
            iw.write(thumb);
        }

        num++;

        if(num != 0 && num % 4 == 0) {
            y += 225;
            x = 0;
        } else {
            x += 400;
        }

        _step += step;

        //qDebug() << num << ") x: " << x << ", y: " << y << ", step: " << _step;
    }

    QImageWriter writer(screenDir + name + ".jpg");
    if(!writer.write(image)) {
        qDebug() << writer.errorString();
    }
}

QImage FFMpeg::getJpeg() {
    AVPacket packet;
    int frameComplete = 0;

    QImage image(width, height, QImage::Format_ARGB32_Premultiplied);
    while((av_read_frame(fctx, &packet)>=0) && !frameComplete) {
        if(packet.stream_index== stream) {
            avcodec_decode_video2(cctx, frame, &frameComplete, &packet);
            if(frameComplete) {
                sws_scale(sctx, frame->data, frame->linesize, 0, cctx->height,  frameRGB->data, frameRGB->linesize);

                uint8_t *src = (uint8_t *)(frameRGB->data[0]);
                for (int y = 0; y < height; y++) {
                    QRgb *scanLine = (QRgb *) image.scanLine(y);
                    for (int x = 0; x < width; x=x+1) {
                        scanLine[x] = qRgb(src[3*x], src[3*x+1], src[3*x+2]);
                    }
                    src += frameRGB->linesize[0];
                }

            }
        }

        av_free_packet(&packet);
    }

    return image;
}

FFMpeg::~FFMpeg() {
    qDebug() << "ffmpeg dtor";
}
