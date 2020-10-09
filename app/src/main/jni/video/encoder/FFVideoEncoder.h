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

};


#endif //SUPERPLAYER_FFVIDEOENCODER_H
