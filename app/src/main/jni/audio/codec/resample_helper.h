//
// Created by Lvlingling on 2020-06-03.
//

#ifndef SUPERPLAYER_RESAMPLE_HELPER_H
#define SUPERPLAYER_RESAMPLE_HELPER_H

extern "C" {
#include <libswresample/swresample.h>
#include <libavutil/samplefmt.h>
}

using namespace std;

class ResampleHelper {

private:

    SwrContext *pSwrContext;
    int dst_nb_samples;
    int max_dst_nb_samples = 0;
    int dst_line_size = 0;
    uint8_t **out_buffer{nullptr};
    int out_sample = 48000;
    int out_channel = 2;

public:

    ResampleHelper(int in_channel, int in_sample, int out_channel, int out_sample, AVSampleFormat in_format = AVSampleFormat::AV_SAMPLE_FMT_S16) {
        this->out_sample = out_sample;
        this->out_channel = out_channel;
        pSwrContext = swr_alloc();
        swr_alloc_set_opts(pSwrContext,
                           av_get_default_channel_layout(out_channel),
                           AV_SAMPLE_FMT_S16,
                           out_sample,
                           av_get_default_channel_layout(in_channel),
                           in_format,
                           in_sample,
                           0,
                           nullptr);
        swr_init(pSwrContext);

        max_dst_nb_samples = dst_nb_samples = av_rescale_rnd(1024, out_sample, in_sample, AV_ROUND_UP);
        av_samples_alloc_array_and_samples(&out_buffer, &dst_line_size, out_channel, dst_nb_samples, AVSampleFormat::AV_SAMPLE_FMT_S16, 0);
    }


    uint8_t *resample(AVFrame *avFrame, int &out_size) {

        dst_nb_samples = av_rescale_rnd(swr_get_delay(pSwrContext, avFrame->sample_rate) + avFrame->nb_samples, out_sample, avFrame->sample_rate, AV_ROUND_UP);
        if (dst_nb_samples > max_dst_nb_samples) {
            av_freep(&out_buffer[0]);
            av_samples_alloc(out_buffer, &dst_line_size, out_channel, dst_nb_samples, AVSampleFormat::AV_SAMPLE_FMT_S16, 1);
            max_dst_nb_samples = dst_nb_samples;
        }

        int len = swr_convert(pSwrContext,
                              out_buffer,
                              dst_nb_samples,
                              (const uint8_t **) (avFrame->extended_data),
                              avFrame->nb_samples);

        if (len > 0) {
            int size = av_samples_get_buffer_size(&dst_line_size, out_channel, len, AVSampleFormat::AV_SAMPLE_FMT_S16, 1);
            uint8_t *out = new uint8_t[size];
            memcpy(out, out_buffer[0], size * sizeof(char));
            out_size = size;
            return out;
        }

        return nullptr;
    }

    int resample(uint8_t *output, int32_t out_size, const uint8_t *input, int32_t input_size) {
        return swr_convert(pSwrContext, &output, out_size, &input, input_size);
    }

    int resample(uint8_t **output, int32_t out_size, const uint8_t **input, int32_t input_size) {
        return swr_convert(pSwrContext, output, out_size, input, input_size);
    }

    virtual ~ResampleHelper() {
        if (out_buffer) {
            av_freep(&out_buffer[0]);
            av_freep(&out_buffer);
        }
        swr_free(&pSwrContext);
    }
};


#endif //SUPERPLAYER_RESAMPLE_HELPER_H
