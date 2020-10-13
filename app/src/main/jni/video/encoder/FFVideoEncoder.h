//
// Created by ldr on 2020/9/14.
//

#ifndef SUPERPLAYER_FFVIDEOENCODER_H
#define SUPERPLAYER_FFVIDEOENCODER_H

#include "VideoEncoder.h"
#include "../utils/thread_safe_queue.h"
#include <future>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>

}


#define ROTATE_0_CROP_LT 0

/**
 * 旋转90度剪裁左上
 */
#define ROTATE_90_CROP_LT 1
/**
 * 暂时没处理
 */
#define ROTATE_180 2
/**
 * 旋转270(-90)裁剪左上，左右镜像
 */
#define ROTATE_270_CROP_LT_MIRROR_LR 3

class FFVideoEncoder : public VideoEncoder {

public:

    FFVideoEncoder(int w, int h, const char *path);

    void encodeFrame(uint8_t *data) override;

    void stop() override;

    virtual ~FFVideoEncoder();

private:

    BlockQueue<uint8_t *> *data_queue{nullptr};
    AVFormatContext *fmt_ctx{nullptr};
    AVOutputFormat *fmt{nullptr};
    AVStream *video_st{nullptr};
    AVCodecContext *codec_ctx{nullptr};
    AVCodec *av_codec{nullptr};
    AVPacket *av_packet{nullptr};
    AVFrame *av_frame{nullptr};
    int pic_size{0};
    int out_y_size{0};
    int frame_cnt{0};
    std::future<void> thread_handler{};
    bool end{false};

    void looper();

    int flush();

    void do_filter(const uint8_t *pic_data, int in_y_size, int format);

    static int NV21_TO_yuv420P(AVFrame *dst, uint8_t *src, int w, int h);

};


#endif //SUPERPLAYER_FFVIDEOENCODER_H
