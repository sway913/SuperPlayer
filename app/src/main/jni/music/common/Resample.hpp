//
// Created by ldr on 2019-11-21.
//

#ifndef SUPERPLAYER_RESAMPLE_HPP
#define SUPERPLAYER_RESAMPLE_HPP
extern "C" {
#include <libswresample/swresample.h>
}

#include "AudioProperty.h"
#include "AudioData.h"

using namespace std;

class Resample {

private:

    SwrContext *pSwrContext;
    AudioProperty outProperty;
    int dst_nb_samples;
    int max_dst_nb_samples = 0;
    int dst_line_size = 0;
    uint8_t **outBuffer;

public:
    Resample(AudioProperty inProperty, AudioProperty outProperty) {
        this->outProperty = outProperty;
        pSwrContext = swr_alloc();
        swr_alloc_set_opts(pSwrContext,
                           av_get_default_channel_layout(outProperty.channelCount),
                           outProperty.sampleFormat,
                           outProperty.sampleRate,
                           av_get_default_channel_layout(inProperty.channelCount),
                           inProperty.sampleFormat,
                           inProperty.sampleRate,
                           0,
                           nullptr);
        swr_init(pSwrContext);

        max_dst_nb_samples = dst_nb_samples = av_rescale_rnd(1024, outProperty.sampleRate, inProperty.sampleRate, AV_ROUND_UP);
        av_samples_alloc_array_and_samples(&outBuffer, &dst_line_size, outProperty.channelCount, dst_nb_samples, outProperty.sampleFormat, 0);
    }


    AudioData resample(AVFrame *avFrame) {

        dst_nb_samples = av_rescale_rnd(swr_get_delay(pSwrContext, avFrame->sample_rate) + avFrame->nb_samples, outProperty.sampleRate, avFrame->sample_rate, AV_ROUND_UP);
        if (dst_nb_samples > max_dst_nb_samples) {
            av_freep(&outBuffer[0]);
            av_samples_alloc(outBuffer, &dst_line_size, outProperty.channelCount, dst_nb_samples, outProperty.sampleFormat, 1);
            max_dst_nb_samples = dst_nb_samples;
        }

        int len = swr_convert(pSwrContext,
                              outBuffer,
                              dst_nb_samples,
                              (const uint8_t **) (avFrame->extended_data),
                              avFrame->nb_samples);

        if (len > 0) {
            int size = av_samples_get_buffer_size(&dst_line_size, outProperty.channelCount, len, outProperty.sampleFormat, 1);
            return AudioData(reinterpret_cast<char *>(outBuffer[0]), size);
        }

        return AudioData();
    }

    int resample(uint8_t *output, int32_t outputSize, const uint8_t *input, int32_t inputSize) {
        return swr_convert(pSwrContext, &output, outputSize, &input, inputSize);
    }

    int resample(uint8_t **output, int32_t outputSize, const uint8_t **input, int32_t inputSize) {
        return swr_convert(pSwrContext, output, outputSize, input, inputSize);
    }

    virtual ~Resample() {
        if (outBuffer) {
            av_freep(&outBuffer[0]);
            av_freep(&outBuffer);
        }
        swr_free(&pSwrContext);
    }
};

#endif //SUPERPLAYER_RESAMPLE_HPP
