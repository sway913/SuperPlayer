//
// Created by ldr on 2020/9/14.
//

#ifndef SUPERPLAYER_FFVIDEOENCODER_H
#define SUPERPLAYER_FFVIDEOENCODER_H

#include "VideoEncoder.h"
#include "../utils/thread_safe_queue.h"
#include "../../thirdpart/oboe-1.3/samples/RhythmGame/src/main/cpp/utils/LockFreeQueue.h"
#include <future>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>

}

class FFVideoEncoder : public VideoEncoder {

public:

    FFVideoEncoder(int w, int h, const char *path_);

    void start();

    void encodeFrame(uint8_t *data) override;

    int encode(AVCodecContext *pCodecCtx, AVFrame *pAvFrame, AVPacket *pAvPacket);

    void stop() override;

    virtual ~FFVideoEncoder();

private:

    LockFreeQueue<uint8_t *, 16> *data_queue{nullptr};
    AVFormatContext *fmt_ctx{nullptr};
    AVOutputFormat *fmt{nullptr};
    AVStream *video_st{nullptr};
    AVCodecContext *codec_ctx{nullptr};
    AVCodec *av_codec{nullptr};
    AVPacket *av_packet{nullptr};
    AVFrame *av_frame{nullptr};
    int frame_cnt{0};
    std::future<void> thread_handler{};
    bool end{false};

    void looper();

    int input_width{0};
    int input_height{0};

    int output_width{0};
    int output_height{0};

    const char *path{nullptr};

    uint64_t startTime{0};
    struct timeval tv {};


};


#endif //SUPERPLAYER_FFVIDEOENCODER_H
