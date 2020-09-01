//
// Created by ldr on 2020/6/10.
//

#ifndef SUPERPLAYER_AUDIO_ENCODER_H
#define SUPERPLAYER_AUDIO_ENCODER_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}

#include "codec_utils.h"
#include <fstream>
#include "../../common/common_tools.h"
#include "../../common/circle_buffer.h"
#include "i_audio_encoder.h"

#define ENCODE_BUFFER_SIZE 4096
#define ENCODE_SAMPLE 44100
#define ENCODE_CHANNELS 2

class AudioEncoder : public IAudioEncoder {

private:
    AVFormatContext *format_ctx{nullptr};
    AVCodecContext *codec_ctx{nullptr};
    AVPacket *packet{nullptr};
    AVFrame *src_frame{nullptr};
    AVFrame *dst_frame{nullptr};
    SwrContext *swrContext{nullptr};

    int pts{0};
    std::ofstream *file_stream{nullptr};
    CircleBuffer<short> *encode_buffer{nullptr};
    int buffer_key{0};
    int buffer_size{0};

    void encode(AVFrame *av_frame);

public:

    void init(int in_sample, int in_channels, const char *file_name) override;

    void encode(short *data, int len) override;

    void close() override;


};


#endif //SUPERPLAYER_AUDIO_ENCODER_H
