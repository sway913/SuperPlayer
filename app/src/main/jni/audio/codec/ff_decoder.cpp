//
// Created by ldr on 2019-11-20.
//

#include "ff_decoder.h"

FFDecoder::FFDecoder() = default;

bool FFDecoder::prepare(const char *fileName, int sample) {
    LOGI("decode file path: %s", fileName);
    p_format_cxt = avformat_alloc_context();
    //avformat_context如果没有被声明分配内存，此方法会给分配。
    int ret = avformat_open_input(&p_format_cxt, fileName, nullptr, nullptr);
    if (ret != 0) {
        return false;
    }
    ret = avformat_find_stream_info(p_format_cxt, nullptr);
    if (ret != 0) {
        return false;
    }
    totalTime = p_format_cxt->duration / (AV_TIME_BASE / 1000);

    ret = av_find_best_stream(p_format_cxt, AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);
    if (ret < 0) {
        return false;
    }
    audioIndex = ret;

    p_codec_para = p_format_cxt->streams[audioIndex]->codecpar;
    AVCodec *codec = avcodec_find_decoder(p_codec_para->codec_id);
    p_codec_cxt = avcodec_alloc_context3(codec);
//    pCodecContext = pFormatContext->streams[audioIndex]->codec;

    p_codec_cxt = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(p_codec_cxt, p_codec_para);

//    AVDictionary *options = nullptr;
//    av_dict_set(&options, "threads", "auto", 0);
    ret = avcodec_open2(p_codec_cxt, codec, nullptr);

    if (ret != 0) {
        return false;
    }


    p_av_packet = av_packet_alloc();
    p_av_frame = av_frame_alloc();

    need_resample = (p_codec_para->sample_rate != sample
                     || p_codec_para->channels != 2
                     || p_codec_para->format != AV_SAMPLE_FMT_S16);
    resampleHelper = new ResampleHelper(p_codec_para->channels,
                                        p_codec_para->sample_rate,
                                        2,
                                        sample,
                                        p_codec_cxt->sample_fmt);

    this->out_sample = sample;

    return true;
}


AudioBuffer *FFDecoder::decodeFrame() {
    auto *audioBuffer = new AudioBuffer();
    int ret = av_read_frame(p_format_cxt, p_av_packet);
    if (ret < 0) {
        av_packet_unref(p_av_packet);
        return audioBuffer;
    }

    ret = avcodec_send_packet(p_codec_cxt, p_av_packet);
    if (ret < 0) {
        av_packet_unref(p_av_packet);
        return audioBuffer;
    }

    while ((ret = avcodec_receive_frame(p_codec_cxt, p_av_frame)) >= 0) {
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            break;
        }
        if (need_resample) {
            int len = 0;
            uint8_t *out = resampleHelper->resample(p_av_frame, len);
            if (len > 0 && out) {
                audioBuffer->push((char *) (out), len);
            }
            free(out);
        } else {
            int size = p_av_frame->nb_samples * p_av_frame->channels * av_get_bytes_per_sample((AVSampleFormat) p_codec_para->format);
            audioBuffer->push((char *) (p_av_frame->extended_data[0]), size);
        }
        av_frame_unref(p_av_frame);
    }
    av_packet_unref(p_av_packet);
    return audioBuffer;
}

void FFDecoder::close() {
    if (p_format_cxt) {
        avformat_close_input(&p_format_cxt);
        avformat_free_context(p_format_cxt);
    }
    if (p_av_frame) {
        av_frame_free(&p_av_frame);
    }

    if (p_av_packet) {
        av_packet_free(&p_av_packet);
    }

    if (p_codec_cxt) {
        avcodec_close(p_codec_cxt);
        avcodec_free_context(&p_codec_cxt);
    }

    p_codec_para = nullptr;

    DELETEOBJ(resampleHelper);

}

void FFDecoder::seek(int64_t millis) {
    avformat_flush(p_format_cxt);
    int64_t seekPts = 0;
    seekPts = static_cast<int64_t>(p_format_cxt->streams[audioIndex]->duration * millis / totalTime);
    int ret = av_seek_frame(p_format_cxt, audioIndex, seekPts, AVSEEK_FLAG_FRAME);
}

int64_t FFDecoder::getTotalMs() {
    return totalTime;
}

FFDecoder::~FFDecoder() = default;

