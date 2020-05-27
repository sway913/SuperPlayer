//
// Created by ldr on 2019-11-20.
//

#include "FFDecoder.h"
#include "../common/AndroidLog.h"

FFDecoder::FFDecoder() {

}

bool FFDecoder::prepare(const char *fileName, AudioProperty outProperty) {
    int ret = avformat_open_input(&pFormatContext, fileName, nullptr, nullptr);
    if (ret != 0) {
        LOGI("decoder file open fail");
        return false;
    }
    ret = avformat_find_stream_info(pFormatContext, nullptr);
    if (ret != 0) {
        LOGI("decoder find streaminfo fail");
        return false;
    }
    totalTime = pFormatContext->duration / (AV_TIME_BASE / 1000);

    ret = av_find_best_stream(pFormatContext, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (ret != 0) {
        LOGI("decoder find best stream fail");
        return false;
    }
    audioIndex = ret;

    pCodecParameters = pFormatContext->streams[ret]->codecpar;

    AVCodec *codec = avcodec_find_decoder(pCodecParameters->codec_id);
    pCodecContext = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(pCodecContext, pCodecParameters);

    ret = avcodec_open2(pCodecContext, nullptr, nullptr);

    if (ret != 0) {
        LOGI("decoder open fail");
        return false;
    }


    pAvPacket = av_packet_alloc();
    pAvFrame = av_frame_alloc();


//    needResample = pCodecParameters->sample_rate != outProperty.sampleRate
//                   || pCodecParameters->channels != outProperty.channelCount
//                   || pCodecParameters->format != outProperty.sampleFormat;

    needResample = true;

    if (needResample) {
        AudioProperty inAudioProperty;
        inAudioProperty.sampleRate = pCodecParameters->sample_rate;
        inAudioProperty.channelCount = pCodecParameters->channels;
        inAudioProperty.sampleFormat = pCodecContext->sample_fmt;
        resample = new Resample(inAudioProperty, outProperty);
        LOGI("file need resample");
    }

    this->outProperty = outProperty;

    return true;
}

AudioData FFDecoder::decodeFrame() {

    int ret = av_read_frame(pFormatContext, pAvPacket);
    if (ret < 0) {
        // LOGE("read frame fail%d:",ret);
        return AudioData();
    }

    ret = avcodec_send_packet(pCodecContext, pAvPacket);
    if (ret < 0) {
        //LOGE("send packet fail%d:",ret);
        return AudioData();
    }

//    send一次，receive不一定一次
//    for(;;){
//        ret = avcodec_receive_frame(pCodecContext, pAvFrame);
//        if (ret < 0) {
//            break;
//        }
//    }
    ret = avcodec_receive_frame(pCodecContext, pAvFrame);
    if (ret < 0) {
        //LOGE("receive frame fail");
        return AudioData();
    }

    AudioData audioData;
    if (needResample) {
        audioData = resample->resample(pAvFrame);
    } else {
        int out_buffer_size = av_samples_get_buffer_size(nullptr, outProperty.channelCount, pAvFrame->nb_samples, outProperty.sampleFormat, 1);
        audioData = AudioData(reinterpret_cast<char *>(pAvFrame->extended_data[0]), out_buffer_size);
    }
    av_frame_unref(pAvFrame);
    av_packet_unref(pAvPacket);
    return audioData;

}


int FFDecoder::decodeFrame(uint8_t *&output) {
    int ret = av_read_frame(pFormatContext, pAvPacket);
    if (ret < 0) {
        return 0;
    }

    ret = avcodec_send_packet(pCodecContext, pAvPacket);
    if (ret < 0) {
        return 0;
    }

    ret = avcodec_receive_frame(pCodecContext, pAvFrame);
    if (ret < 0) {
        return 0;
    }

    if (needResample) {

        int in_nb_sample = pAvFrame->nb_samples;
        int out_nb_sample = static_cast<int>(roundf(in_nb_sample * (float) outProperty.sampleRate / (float) pCodecParameters->sample_rate * 1.2f));
        int out_size = out_nb_sample * outProperty.channelCount * sizeof(int16_t);

        if (bufferLen < out_size) {
            output = static_cast<uint8_t *>(realloc(output, sizeof(uint8_t) * out_size));
            bufferLen = out_size;
            LOGI("decode realloc");
        }

        int len = resample->resample(&output, out_nb_sample, (const uint8_t **) pAvFrame->extended_data, in_nb_sample);

        av_frame_unref(pAvFrame);
        av_packet_unref(pAvPacket);

        return len * outProperty.channelCount * sizeof(int16_t);

    } else {
        int out_buffer_size = av_samples_get_buffer_size(nullptr, outProperty.channelCount, pAvFrame->nb_samples, outProperty.sampleFormat, 1);
        output = pAvFrame->extended_data[0];
        av_frame_unref(pAvFrame);
        av_packet_unref(pAvPacket);
        return out_buffer_size;
    }
}

void FFDecoder::close() {
    if (pFormatContext) {
        avformat_close_input(&pFormatContext);
        avformat_free_context(pFormatContext);
    }
    if (pAvFrame) {
        av_frame_free(&pAvFrame);
    }

    if (pAvPacket) {
        av_packet_free(&pAvPacket);
    }

    if (pCodecContext) {
        avcodec_close(pCodecContext);
        avcodec_free_context(&pCodecContext);
    }

    pCodecParameters = nullptr;

    if (resample) {
        delete resample;
        resample = nullptr;
    }

}

void FFDecoder::seek(int64_t millis) {
    avformat_flush(pFormatContext);
    int64_t seekPts = 0;
    seekPts = static_cast<int64_t>(pFormatContext->streams[audioIndex]->duration * millis / totalTime);
    int ret = av_seek_frame(pFormatContext, audioIndex, seekPts, AVSEEK_FLAG_FRAME);
    //LOGI("ff seek millis %d,seekpts % d,audio index is %d", millis, seekPts, audioIndex);
    if (ret < 0) {
        LOGE("av_seek_frame failed %d", ret);
        return;
    } else {
        return;
    }
}

int64_t FFDecoder::getTotalMs() {
    return totalTime;
}

FFDecoder::~FFDecoder() {

}

