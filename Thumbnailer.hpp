#ifndef THUMBNAILER_HPP
#define THUMBNAILER_HPP

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include <string>
#include <iostream>
#include <memory>
#include <cstdio>
#include <cstring>
#include <QLabel>
#include "Settings.hpp"
using namespace std;

class Thumbnailer
{
public:
    Thumbnailer(Settings config);
    ~Thumbnailer();

    void import();
    void generateScreens(const char *, const char *);
    void saveScreenshots(AVFormatContext*, AVCodecContext*, struct SwsContext *,AVFrame*, AVFrame*, int,int,const char *,const char *);
    void saveBestFrame(const char *);

private:
//    AVFormatContext *fctx = NULL;
//    AVCodecContext *cctx = NULL;
//    AVCodec *codec = NULL;
//    AVFrame *frame = NULL, *frameRGB = NULL;
//    struct SwsContext *sctx = NULL;
    //int stream, numBytes, duration, step;
    int rowCount = 5;
    int colCount  = 3;
    int dst_w = 400;
    int dst_h = 225;
//    AVPixelFormat pixfmt = PIX_FMT_RGB24;
//    uint8_t *buffer;
    Settings config;
};


#endif // THUMBNAILER_HPP
