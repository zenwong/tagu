#ifndef FFMPEG_HPP
#define FFMPEG_HPP
#include <QDirIterator>
#include <QElapsedTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QCoreApplication>
#include "Utils.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
}
using namespace std;

class FFMpeg
{
public:
   FFMpeg(QSqlDatabase db);
   ~FFMpeg();

   int doImport();

   void parse(QString path, QString name);
   void seek();

 private:
   AVFormatContext *fctx;
   AVCodecContext *cctx;
   AVStream *vstream;
   AVCodec *codec;
   AVPacket packet;
   AVFrame *frame, *frameRGB;
   struct SwsContext *sctx = nullptr;
   int stream, numBytes, duration, step;
   AVPixelFormat pixfmt = PIX_FMT_RGB24;
   uint8_t *buffer;

   int rowCount;
   int colCount;
   int width;
   int height;
   int jpegQuality;

   int imported;

   void genScreens();
   QImage getJpeg();

   void decodeVideoFrame();
   bool decodeVideoPacket();
   bool getVideoPacket();

   QString path, name;
   QElapsedTimer timer;
   QStringList filters;
   QSqlDatabase db;

   QString thumbDir, screenDir;
};

#endif // FFMPEG_HPP

