#include "frameencoder.h"


//fix compilation error for ffmpeg
#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif



//fix msvc64 compalation error for some ffmpeg versions
#include <memory>
#undef av_err2str
#define av_err2str(errnum) \
    av_make_error_string(reinterpret_cast<char*>(alloca(AV_ERROR_MAX_STRING_SIZE)),\
    AV_ERROR_MAX_STRING_SIZE, errnum)




FrameEncoder::FrameEncoder()
{
    ost = (OutputStream*)av_malloc(sizeof(OutputStream*));
    this->swsContext = 0x0;
    this->formatContext = 0x0;
    this->outputFormat = 0x0;
    this->videoStream = 0x0;
    this->videoCodec = 0x0;
    this->ost->sws_ctx = 0x0;
    this->frame = 0x0;
    this->swsFlags = SWS_BICUBIC;
    this->streamPixFmt = AV_PIX_FMT_YUV420P;
    this->width = 640;
    this->height = 480;

    // Init FFmpeg
    av_register_all();
}

FrameEncoder::~FrameEncoder()
{
    //flush();
    //close the media file
    closeMediaFile(); //implicit close of videofile
}


///
/// \brief FrameEncoder::setResolution set the resolution of the video
/// \param inWidth the new width of the output video
/// \param inHeight the new height of the output video
///
void FrameEncoder::setResolution(int inWidth, int inHeight)
{

    //use q_assert to make sure the widths and hights are divisible by 2 for rescaling algorithms
    Q_ASSERT(inWidth%2  == 0);
    Q_ASSERT(inHeight%2 == 0);
    width  = inWidth;
    height = inHeight;
}



bool FrameEncoder::recieveFrame(const QImage & image, qint64 timestamp)
{

    //would be required for more accureate video encoding
    Q_UNUSED(timestamp);


    //image to smaller image
    QImage scaled = image.scaled(this->width,this->height);

    //But for now a simple linear process
    const int width = scaled.width();
    const int height = scaled.height();

    byte* srcData = (uchar *)scaled.bits();


    //writeVideoFrame(srcData,width,height,formatContext,videoStream);
    writeVideoFrame(formatContext,srcData,width, height);


    //simplify with one call to sws_scale and use byte* instead of two in our current case






    return true;
}

bool FrameEncoder::rgb32ToYuv420P(byte* src, int width, int height)
{
    byte* dest_planes[3] = {frame->data[0],frame->data[1], frame->data[2]};
    const int dest_stride[3] = {width, width / 2, width / 2};
    byte* src_planes[3] = {src,NULL,NULL};
    int src_stride[3] = {width*4, 0 , 0};
    if(!ost->sws_ctx){
       ost->sws_ctx = sws_getContext(
                    width,height,
                    AV_PIX_FMT_RGB32,
                    width,height,AV_PIX_FMT_YUV420P,
                    swsFlags,NULL,NULL,NULL);
    }
    sws_scale(ost->sws_ctx, src_planes,src_stride,0,height,dest_planes,dest_stride);
    return true;
}

int FrameEncoder::writeFrame(AVFormatContext *fmt_ctx,const AVRational *time_base,AVStream * st, AVPacket *pkt){

    /* rescale output packet timestamp values from codec to stream timebase */
    av_packet_rescale_ts(pkt, *time_base, st->time_base);
    pkt->stream_index = st->index;

    /* Write the compressed frame to the media file. */
    //log_packet(fmt_ctx, pkt); //TODO: implement log packet
    return av_interleaved_write_frame(fmt_ctx, pkt);

}

// FFmpeg 3.0 version of encoding
bool FrameEncoder::writeVideoFrame(AVFormatContext *oc, byte *srcData, int srcWidth, int srcHeight){
    int ret;
    AVCodecContext *c = ost->st->codec;
    frame = getVideoFrame(ost);


    ret = rgb32ToYuv420P(srcData, srcWidth, srcHeight);

    if(!ret){
        qDebug() << "Failed to convert from RGB to YUV" << endl;
    }

    // encode the image
    AVPacket pkt;
    int gotOutput;
    av_init_packet(&pkt);
    pkt.data = NULL;    // packet data will be allocated by the encoder
    pkt.size = 0;
    pkt.stream_index = ost->st->index;
    av_packet_rescale_ts(&pkt,ost->st->time_base,ost->st->time_base);
    ret = avcodec_encode_video2(c, &pkt, frame, &gotOutput);
    if (ret < 0)
    {
        qDebug() << "Error Encoding video frame: " << av_err2str(ret) << endl;
        return false;
    }
    // If size is zero, it means the image was buffered.
    if (gotOutput)
    {
       ret = writeFrame(oc, &c->time_base, ost->st, &pkt);
    }
    else
    {
        ret = 0;
    }

    if (ret != 0)
    {
        qDebug() << "Error Writing video Frame" << av_err2str(ret) <<endl;
        return false;
    }
    frameCount++;
    return true;
}
AVFrame * FrameEncoder::allocPicture(enum AVPixelFormat pix_fmt, int width, int height)
{
    AVFrame *picture;
    int ret = 0;

    picture = av_frame_alloc();
    if (!picture)
        return NULL;

    picture->format = pix_fmt;
    picture->width  = width;
    picture->height = height;

    /* allocate the buffers for the frame data */
    ret = av_frame_get_buffer(picture, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate frame data.\n");
        exit(1);
    }

    return picture;
}

/// Preferred method of opening video in ffmpeg 3.0 branch
/// \brief FrameEncoder::openVideo
/// \param oc Format context
/// \param codec codec to be used
/// \param ost output stream
/// \param opt_args arguments for the muxer
/// \return true on success
///
bool FrameEncoder::openVideo(AVFormatContext *oc, AVCodec *codec, OutputStream *ost){
    Q_UNUSED(oc);
    int ret;
    AVCodecContext *c = ost->st->codec;


    /* open the codec */
    ret = avcodec_open2(c, codec, NULL);
    if (ret < 0) {
        fprintf(stderr, "Could not open video codec: %s\n", av_err2str(ret));
        return false;
    }

    /* allocate and init a re-usable frame */
    ost->frame = allocPicture(c->pix_fmt, c->width, c->height);
    if (!ost->frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        return false;
    }

    /* If the output format is not YUV420P, then a temporary YUV420P
         * picture is needed too. It is then converted to the required
         * output format. */
    ost->tmp_frame = NULL;
    if (c->pix_fmt != AV_PIX_FMT_YUV420P) {
        ost->tmp_frame = allocPicture(AV_PIX_FMT_YUV420P, c->width, c->height);
        if (!ost->tmp_frame) {
            fprintf(stderr, "Could not allocate temporary picture\n");
            return false;
        }
    }
    return true;
}


//ffmpeg 3.0 way of encoding streams
void FrameEncoder::addStream(OutputStream *ost, AVFormatContext * oc, AVCodec **codec, enum AVCodecID codec_id){
    AVCodecContext *c;
    int i;

    /* find the encoder */
    *codec = avcodec_find_encoder(codec_id);
    if (!(*codec)) {
        fprintf(stderr, "Could not find encoder for '%s'\n",
                avcodec_get_name(codec_id));
        exit(1);
    }

    ost->st = avformat_new_stream(oc, *codec);
    if (!ost->st) {
        fprintf(stderr, "Could not allocate stream\n");
        exit(1);
    }
    ost->st->id = oc->nb_streams-1;
    c = ost->st->codec;

    switch ((*codec)->type) {
    case AVMEDIA_TYPE_AUDIO:
        hasAudio = true;
        c->sample_fmt  = (*codec)->sample_fmts ?
                    (*codec)->sample_fmts[0] : AV_SAMPLE_FMT_FLTP;
        c->bit_rate    = 64000;
        c->sample_rate = 44100;
        if ((*codec)->supported_samplerates) {
            c->sample_rate = (*codec)->supported_samplerates[0];
            for (i = 0; (*codec)->supported_samplerates[i]; i++) {
                if ((*codec)->supported_samplerates[i] == 44100)
                    c->sample_rate = 44100;
            }
        }
        c->channels        = av_get_channel_layout_nb_channels(c->channel_layout);
        c->channel_layout = AV_CH_LAYOUT_STEREO;
        if ((*codec)->channel_layouts) {
            c->channel_layout = (*codec)->channel_layouts[0];
            for (i = 0; (*codec)->channel_layouts[i]; i++) {
                if ((*codec)->channel_layouts[i] == AV_CH_LAYOUT_STEREO)
                    c->channel_layout = AV_CH_LAYOUT_STEREO;
            }
        }
        c->channels        = av_get_channel_layout_nb_channels(c->channel_layout);
        ost->st->time_base = { 1, c->sample_rate };
        break;

    case AVMEDIA_TYPE_VIDEO:
        hasVideo = true;
        c->codec_id = codec_id;

        c->bit_rate = 400000;
        /* Resolution must be a multiple of two. */
        c->width    = this->width;
        c->height   = this->height;
        /* timebase: This is the fundamental unit of time (in seconds) in terms
             * of which frame timestamps are represented. For fixed-fps content,
             * timebase should be 1/framerate and timestamp increments should be
             * identical to 1. */
        ost->st->time_base = { 1, c->sample_rate};
        c->time_base       = ost->st->time_base;

        c->gop_size      = 12; /* emit one intra frame every twelve frames at most */
        c->pix_fmt       =  AV_PIX_FMT_YUV420P;
        if (c->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
            /* just for testing, we also add B frames */
            c->max_b_frames = 2;
        }
        if (c->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
            /* Needed to avoid using macroblocks in which some coeffs overflow.
                 * This does not happen with normal video, it just happens here as
                 * the motion of the chroma plane does not match the luma plane. */
            c->mb_decision = 2;
        }
        break;

    default:
        break;
    }

    /* Some formats want stream headers to be separate. */
    if (oc->oformat->flags & AVFMT_GLOBALHEADER)
        c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

}

AVFrame * FrameEncoder::getVideoFrame(OutputStream * ost){

   AVCodecContext *c = ost->st->codec;

//            /* check if we want to generate more frames */
//            if (av_compare_ts(ost->next_pts, c->time_base,
//                              STREAM_DURATION, (AVRational){ 1, 1 }) >= 0)
//                return NULL;

   if (c->pix_fmt != AV_PIX_FMT_YUV420P) {
       /* as we only generate a YUV420P picture, we must convert it
            * to the codec pixel format if needed */
       if (!ost->sws_ctx) {
           ost->sws_ctx = sws_getContext(c->width, c->height,
                                         AV_PIX_FMT_YUV420P,
                                         c->width, c->height,
                                         c->pix_fmt,
                                         swsFlags, NULL, NULL, NULL);
           if (!ost->sws_ctx) {
               fprintf(stderr,
                       "Could not initialize the conversion context\n");
               exit(1);
           }
       }
       //fill_yuv_image(ost->tmp_frame, ost->next_pts, c->width, c->height);
       sws_scale(ost->sws_ctx,
                 (const uint8_t * const *)ost->tmp_frame->data, ost->tmp_frame->linesize,
                 0, c->height, ost->frame->data, ost->frame->linesize);
   } else {
       //fill_yuv_image(ost->frame, ost->next_pts, c->width, c->height);
   }

   ost->frame->pts = ost->next_pts++;

   return ost->frame;
}
void FrameEncoder::fill_yuv_image(AVFrame * pict, int frame_index, int width, int height){

    //May not need this. We have already allocated a buffer. I don't see why we need to fill it twice
    int x, y, i, ret = 0;

      /* when we pass a frame to the encoder, it may keep a reference to it
       * internally;
       * make sure we do not overwrite it here
       */
    ret = av_frame_make_writable(pict);
    if (ret < 0)
        exit(1);

    i = frame_index;

    /* Y */
    for (y = 0; y < height; y++)
        for (x = 0; x < width; x++)
            pict->data[0][y * pict->linesize[0] + x] = x + y + i * 3;

    /* Cb and Cr */
    for (y = 0; y < height / 2; y++) {
        for (x = 0; x < width / 2; x++) {
            pict->data[1][y * pict->linesize[1] + x] = 128 + y + i * 2;
            pict->data[2][y * pict->linesize[2] + x] = 64 + x + i * 5;
        }
    }


}

bool FrameEncoder::openMediaFile(int width, int height, const QString &filename)
{

   this->width = width;
   this->height = height;

   // allocate the output media context
   avformat_alloc_output_context2(&formatContext, NULL, NULL, filename.toStdString().c_str());
   if (!formatContext)
   {
       qDebug() << "Can't deduce the file extension using x264" << endl;
       avformat_alloc_output_context2(&formatContext, NULL, "mp4", filename.toStdString().c_str());
   }
   if (!formatContext)
   {
       return false;
   }
   outputFormat = formatContext->oformat;
   // Add the video streams using the default format codecs
   // and initialize the codecs.
   videoStream = NULL;
   if (outputFormat->video_codec != AV_CODEC_ID_NONE)
   {
       //videoStream = addStream(formatContext, &videoCodec, outputFormat->video_codec); //TODO:change to OST DONE.
       addStream(ost,formatContext,&videoCodec, formatContext->oformat->video_codec);
   }


   // Now that all the parameters are set, we can open the audio and
   // video codecs and allocate the necessary encode buffers.
   if (hasVideo){
       openVideo(formatContext,videoCodec, ost);
       //        videoStream->time_base.den = streamFrameRate; //added to get
       //        videoStream->time_base.num = 1;
   }

   av_dump_format(formatContext, 0, filename.toStdString().c_str(), 1);
   int ret = 0;
   // open the output file, if needed
   if (!(outputFormat->flags & AVFMT_NOFILE))
   {
       ret = avio_open(&formatContext->pb, filename.toStdString().c_str(), AVIO_FLAG_WRITE);
       if (ret < 0)
       {
           qDebug() << "Could not open" + filename + av_err2str(ret)  << endl;
           return false;
       }
   }
   // Write the stream header, if any.
   ret = avformat_write_header(formatContext, NULL);
   if (ret < 0)
   {
       qDebug() << "Error occured opening output file: " << av_err2str(ret) << endl;
       return false;
   }
   return true;
}

void FrameEncoder::closeVideo(AVStream *stream)
{
   avcodec_close(stream->codec);
   av_free(frame);
}

bool FrameEncoder::closeMediaFile()
{

   // Write the trailer, if any. The trailer must be written before you
   // close the CodecContexts open when you wrote the header; otherwise
   // av_write_trailer() may try to use memory that was freed on
   // av_codec_close().
   av_write_trailer(formatContext);
   // Close each codec.
   if (hasVideo)
       closeVideo(ost->st);
   if (ost->sws_ctx)
   {
       sws_freeContext(ost->sws_ctx);
       ost->sws_ctx = 0x0;
   }
   // Free the streams.
   for (unsigned int i = 0; i < formatContext->nb_streams; i++)
   {
       av_freep(&formatContext->streams[i]->codec);
       av_freep(&formatContext->streams[i]);
   }
   if (!(outputFormat->flags & AVFMT_NOFILE))
   {
       // Close the output file.
       avio_close(formatContext->pb);
   }
   // free the stream
   av_free(formatContext);

   return true;
}



