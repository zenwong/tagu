#ifndef FFMPEG_HPP
#define FFMPEG_HPP
#include <QDirIterator>
#include <QElapsedTimer>
#include "Settings.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <mozjpeg/jpeglib.h>
}
using namespace std;

class FFMpeg
{
public:
   FFMpeg(Settings config);

   int run();

   void parse(QString path, QString name);
   void seek();

 private:
   AVFormatContext *fctx = NULL;
   AVCodecContext *cctx = NULL;
   AVStream *vstream;
   AVCodec *codec = NULL;
   AVPacket packet;
   AVFrame *frame = NULL, *frameRGB = NULL;
   struct SwsContext *sctx = NULL;
   int stream, numBytes, duration, step;
   AVPixelFormat pixfmt = PIX_FMT_RGB24;
   uint8_t *buffer;

   int rowCount = 10;
   int colCount = 4;
   int width  = 400;
   int height = 225;
   int jpegQuality = 70;

   int imported;

   void genScreens();
   void saveJpeg(int seconds);

   void decodeVideoFrame();
   bool decodeVideoPacket();
   bool getVideoPacket();

   QString path, name;
   QElapsedTimer timer;
   Settings config;
   QStringList filters;
};

#endif // FFMPEG_HPP

