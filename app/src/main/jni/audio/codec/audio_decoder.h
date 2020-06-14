//
// Created by ldr on 2019-11-20.
//

#ifndef SUPERPLAYER_AUDIO_DECODER_H
#define SUPERPLAYER_AUDIO_DECODER_H

#include "resample_helper.h"
#include "../../common/android_log.h"
#include "../../common/common_tools.h"
#include "../../common/audio_buffer.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

}

class AudioDecoder {

private:

    AVFormatContext *p_format_cxt{nullptr};
    int audioIndex{};
    AVCodecContext *p_codec_cxt{nullptr};
    AVFrame *p_av_frame{nullptr};
    AVPacket *p_av_packet{nullptr};
    AVCodecParameters *p_codec_para{nullptr};

    int64_t totalTime{};

    ResampleHelper *resampleHelper{nullptr};
    int out_sample{};
    bool need_resample{false};

public:

    AudioDecoder();

    virtual ~AudioDecoder();

    bool prepare(const char *file_name, int sample);

    AudioBuffer* decodeFrame();

    int64_t getTotalMs();

    void seek(int64_t millis);

    void close();

};


#endif //SUPERPLAYER_AUDIO_DECODER_H
