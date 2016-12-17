#ifndef FRAMEENCODER_H
#define FRAMEENCODER_H

#include <QObject>
#include <QImage>
#include <QDebug>
#include <stdint.h>

extern "C" {
#include "libavutil/mathematics.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavutil/imgutils.h"
}
typedef unsigned char byte;

// a wrapper around a single output AVStream
typedef struct OutputStream {
    AVStream *st;
    AVCodecContext *enc;

    /* pts of the next frame that will be generated */
    int64_t next_pts;
    int samples_count;

    AVFrame *frame;
    AVFrame *tmp_frame;

    float t, tincr, tincr2;

    struct SwsContext *sws_ctx;
    struct SwrContext *swr_ctx;
} OutputStream;

class FrameEncoder : public QObject
{
public:
    FrameEncoder();
    ~FrameEncoder();

    bool openMediaFile(int width,
                       int height,
                       const QString & filename);
    bool closeMediaFile();
    void setResolution(int width, int height);
    bool isFlushed();

signals:
    bool flushed();
public slots:
    bool recieveFrame(const QImage &image, qint64 timestamp);
    void flush();



protected:
    AVStream * addStream(AVFormatContext *inFormatContext,
                         AVCodec **codec,
                         AVCodecID codecId) const;
    void addStream(OutputStream * ost,AVFormatContext *oc, AVCodec **codec,enum AVCodecID codecId) const;
    bool openVideo(AVFormatContext * oc, AVCodec *codec, OutputStream *ost);
    bool writeVideoFrame(byte* srcData, int srcWidth, int srcHeight, AVFormatContext * inFormatContext, AVStream * stream);
    bool writeVideoFrame(AVFormatContext *oc, byte* srcData, int srcWidth, int srcHeight);
    void closeVideo(AVStream *st);

    int writeFrame(AVFormatContext *fmt_ctx,const AVRational *time_base,AVStream * st, AVPacket *pkt);


    void addStream(OutputStream *ost, AVFormatContext * oc, AVCodec **codec, enum AVCodecID codec_id);

    AVFrame *allocPicture(enum AVPixelFormat pix_fmt, int width, int height);
    AVFrame *getVideoFrame(OutputStream *ost);
    void fill_yuv_image(AVFrame * pict, int next_index, int width, int height);


    bool rgb32ToYuv420P(byte * src, int width, int height);


    OutputStream    * ost;
    AVFormatContext * formatContext;
    AVOutputFormat  * outputFormat;
    AVStream        * videoStream;
    AVCodec         * videoCodec;
    SwsContext      * swsContext;
    AVFrame         * frame;
    AVPixelFormat streamPixFmt;
    int swsFlags;
    int streamFrameRate;
    int width;
    int height;
    int frameCount;
    bool hasVideo, hasAudio;
};

#endif // FRAMEENCODER_H
