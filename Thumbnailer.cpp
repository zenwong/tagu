#include "Thumbnailer.hpp"
#include <QDebug>
#include <QDirIterator>
#include <QImageWriter>
#include <QPainter>

Thumbnailer::Thumbnailer(Settings config){
    this->config = config;
    avcodec_register_all();
    av_register_all();
    av_log_set_level(AV_LOG_QUIET);

    qDebug() << "thumbnailer dirs: " << config.javDirs;
}

void Thumbnailer::import() {
    foreach(const QString& d, config.javDirs) {
        QDir dir(d);
        QDirIterator iterator(dir.absolutePath(), QDir::AllDirs|QDir::Files, QDirIterator::Subdirectories);

        while (iterator.hasNext()) {
            iterator.next();
            if (iterator.fileInfo().isFile()) {
                generateScreens(iterator.filePath().toStdString().c_str(), iterator.fileInfo().baseName().toStdString().c_str());
            }
        }
    }

}

void Thumbnailer::generateScreens(const char *videofile, const char *basename) {
    AVFormatContext *fctx = NULL;
    AVCodecContext *cctx = NULL;
    AVCodec *codec = NULL;
    AVFrame *frame = NULL, *frameRGB = NULL;
    struct SwsContext *sctx = NULL;
    int stream, numBytes, duration, step;
    AVPixelFormat pixfmt = PIX_FMT_RGB24;
    uint8_t *buffer;

    avformat_open_input(&fctx, videofile, NULL, NULL);
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

    sctx = sws_getCachedContext(sctx, cctx->width, cctx->height, cctx->pix_fmt, cctx->width, cctx->height, pixfmt, SWS_BILINEAR, NULL, NULL, NULL);
    numBytes = avpicture_get_size(pixfmt, cctx->width, cctx->height);
    buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));
    avpicture_fill((AVPicture *)frameRGB, buffer, pixfmt, cctx->width, cctx->height);

    saveScreenshots(fctx, cctx, sctx, frameRGB, frame, stream, step, videofile, basename);

    if(frame) av_frame_unref(frame);
    if(frameRGB) av_frame_unref(frameRGB);
    if(buffer) av_free(buffer);
    if(frameRGB) av_free(frameRGB);
    if(frame) av_free(frame);
    if(sctx) sws_freeContext(sctx);
    if(cctx) {
        avcodec_close(cctx);
        av_free(cctx);
    }
    if(fctx) {
        avformat_close_input(&fctx);
        av_free(fctx);
    }
}

void Thumbnailer::saveScreenshots(AVFormatContext *fctx, AVCodecContext *cctx, struct SwsContext *sctx, AVFrame *frameRGB, AVFrame *frame, int stream ,int step, const char *file, const char *basename) {
    for (int i = 0; i < rowCount; ++i) {
        for (int j = 0; j < colCount; ++j) {
            int timeInSeconds = step * ( j * rowCount + i + 1);
            int64_t timestamp = av_rescale(((uint64_t)timeInSeconds)*1000, fctx->streams[stream]->time_base.den, fctx->streams[stream]->time_base.num)/1000;
            avformat_seek_file(fctx, stream, INT64_MIN, timestamp, INT64_MAX, AVSEEK_FLAG_FRAME);
            avcodec_flush_buffers(cctx);

            AVPacket packet;
            int frameComplete = 0;

            while((av_read_frame(fctx, &packet)>=0) && !frameComplete) {
                if(packet.stream_index== stream) {
                    avcodec_decode_video2(cctx, frame, &frameComplete, &packet);
                    if(frameComplete) {
                        QImage myImage(dst_w, dst_h, QImage::Format_RGB32);
                        sws_scale(sctx, frame->data, frame->linesize, 0, cctx->height,  frameRGB->data, frameRGB->linesize);

                        uint8_t *src = (uint8_t *)(frameRGB->data[0]);
                        for (int y = 0; y < dst_h; y++) {
                            QRgb *scanLine = (QRgb *) myImage.scanLine(y);
                            for (int x = 0; x < dst_w; x=x+1) {
                                scanLine[x] = qRgb(src[3*x], src[3*x+1], src[3*x+2]);
                            }
                            src += frameRGB->linesize[0];
                        }

                        QString filename = "/tmp/ffmpeg/";
                        filename.append(basename);
                        filename.append("-");
                        filename.append(QString::number(timeInSeconds));
                        filename.append(".jpg");

                        QImageWriter writer(filename);
                        if(!writer.write(myImage)) {
                            qDebug() << writer.errorString();
                        }

                    }
                }

                av_free_packet(&packet);
            }

        }
    }

}

void Thumbnailer::saveBestFrame(const char *file) {

}

Thumbnailer::~Thumbnailer() {
    //    if(buffer) av_free(buffer);
    //    if(frameRGB) av_free(frameRGB);
    //    if(frame) av_free(frame);
    //    if(sctx) sws_freeContext(sctx);
    //    if(cctx) avcodec_close(cctx);
    //    if(fctx) avformat_close_input(&fctx);
}
