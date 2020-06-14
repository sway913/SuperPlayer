//
// Created by ldr on 2020/6/10.
//

#ifndef SUPERPLAYER_CODEC_UTILS_H
#define SUPERPLAYER_CODEC_UTILS_H


enum CodecFormat {
    CodecFormatMP3 = 0,
    CodecFormatAC3 = 1,
    CodecFormatAAC = 2
};

/**
 *  判断采样格式对于指定的编码器是否支持，如果支持则返回该采样格式；否则返回编码器支持的枚举值最大的采样格式
 */
static inline enum AVSampleFormat select_sample_fmt(const AVCodec *codec, enum AVSampleFormat sample_fmt) {
    const enum AVSampleFormat *p = codec->sample_fmts;
    enum AVSampleFormat rfmt = AV_SAMPLE_FMT_NONE;
    while (*p != AV_SAMPLE_FMT_NONE) {
        if (*p == sample_fmt) {
            return sample_fmt;
        }
        if (rfmt == AV_SAMPLE_FMT_NONE) {
            rfmt = *p;
        }
        p++;
    }
    return rfmt;
}

/**
 *  返回指定编码器接近尽量接近44100的采样率
 */
static inline int select_sample_rate(const AVCodec *codec, int default_sample_rate) {
    const int *p = 0;
    int best_sample_rate = 0;
    if (!codec->supported_samplerates) {
        return 44100;
    }

    p = codec->supported_samplerates;
    while (*p) {
        if (*p == default_sample_rate) {
            return *p;
        }

        if (!best_sample_rate || abs(44100 - *p) < abs(44100 - best_sample_rate)) {
            best_sample_rate = *p;
        }

        p++;
    }

    return best_sample_rate;
}

/** 返回编码器支持的声道格式中声道数最多的声道格式
 *  声道格式和声道数一一对应
 */
static inline uint64_t select_channel_layout(const AVCodec *codec, uint64_t default_layout) {
    uint64_t best_ch_layout = AV_CH_LAYOUT_STEREO;
    const uint64_t *p = codec->channel_layouts;
    if (p == NULL) {
        return AV_CH_LAYOUT_STEREO;
    }
    int best_ch_layouts = 0;
    while (*p) {
        int layouts = av_get_channel_layout_nb_channels(*p);
        if (*p == default_layout) {
            return *p;
        }

        if (layouts > best_ch_layouts) {
            best_ch_layout = *p;
            best_ch_layouts = layouts;
        }
        p++;
    }


    return best_ch_layout;
}

static inline AVCodec *select_codec(CodecFormat format) {
    AVCodec *codec = NULL;
    if (format == CodecFormatAAC) {
        /** 可以通过如下两个方法查找编码器，aac编码器对应的库是fdk_aac
         */
//        codec = avcodec_find_encoder(AV_CODEC_ID_AAC);
        codec = avcodec_find_encoder_by_name("libfdk_aac");
    } else if (format == CodecFormatMP3) {
//        codec = avcodec_find_encoder(AV_CODEC_ID_MP3);
        codec = avcodec_find_encoder_by_name("libmp3lame");
    } else if (format == CodecFormatAC3) {
        codec = avcodec_find_encoder(AV_CODEC_ID_AC3);
    }

    return codec;
}

static inline int select_bit_rate(CodecFormat format) {
    // 对于不同的编码器最优码率不一样，单位bit/s;对于mp3来说，192kbps可以获得较好的音质效果。
    int bit_rate = 64000;
    if (format == CodecFormatMP3) {
        bit_rate = 192000;
    } else if (format == CodecFormatAC3) {
        bit_rate = 192000;
    } else if (format == CodecFormatAAC) {
        bit_rate = 64000;
    }

    return bit_rate;
}

#endif //SUPERPLAYER_CODEC_UTILS_H
