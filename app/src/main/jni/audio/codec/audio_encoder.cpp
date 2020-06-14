//
// Created by ldr on 2020/6/10.
//

#include "audio_encoder.h"
#include "../../common/android_log.h"

void AudioEncoder::init(int in_sample, int in_channels, const char *file_name) {
    CodecFormat codec_format = CodecFormatAAC;
    AVCodec *codec = select_codec(codec_format);
    if (codec == nullptr) {
        return;
    }
    codec_ctx = avcodec_alloc_context3(codec);
    if (codec_ctx == nullptr) {
        return;
    }
    codec_ctx->bit_rate = select_bit_rate(codec_format);
    codec_ctx->sample_rate = select_sample_rate(codec, ENCODE_SAMPLE);
    codec_ctx->sample_fmt = select_sample_fmt(codec, AV_SAMPLE_FMT_S16);
    codec_ctx->channel_layout = select_channel_layout(codec, av_get_default_channel_layout(ENCODE_CHANNELS));
    codec_ctx->channels = av_get_channel_layout_nb_channels(codec_ctx->channel_layout);

    int ret = avcodec_open2(codec_ctx, codec, nullptr);
    if (ret < 0) {
        return;
    }

    packet = av_packet_alloc();
    src_frame = av_frame_alloc();
    dst_frame = av_frame_alloc();

    src_frame->nb_samples = (int) ((float) codec_ctx->frame_size * (float) in_sample / ENCODE_SAMPLE);
    dst_frame->nb_samples = codec_ctx->frame_size;

    src_frame->format = AV_SAMPLE_FMT_S16;
    dst_frame->format = codec_ctx->sample_fmt;

    src_frame->channel_layout = av_get_default_channel_layout(in_channels);
    dst_frame->channel_layout = codec_ctx->channel_layout;

    src_frame->sample_rate = in_sample;
    dst_frame->sample_rate = codec_ctx->sample_rate;

    src_frame->channels = in_channels;
    dst_frame->channels = ENCODE_CHANNELS;

    ret = av_frame_get_buffer(src_frame, 0);
    if (ret < 0) {
        av_frame_unref(src_frame);
        return;
    }

    ret = av_frame_make_writable(src_frame);
    if (ret < 0) {
        av_frame_unref(src_frame);
        return;
    }

    ret = av_frame_get_buffer(dst_frame, 0);
    if (ret < 0) {
        av_frame_unref(dst_frame);
        return;
    }

    ret = av_frame_make_writable(dst_frame);
    if (ret < 0) {
        av_frame_unref(dst_frame);
        return;
    }

    ret = avformat_alloc_output_context2(&format_ctx, nullptr, nullptr, file_name);
    if (ret < 0) {
        return;
    }

    AVStream *stream = avformat_new_stream(format_ctx, nullptr);
    if (!stream) {
        return;
    }

    ret = avio_open(&format_ctx->pb, file_name, AVIO_FLAG_WRITE);
    if (ret < 0) {
        return;
    }

    avcodec_parameters_from_context(stream->codecpar, codec_ctx);
    ret = avformat_write_header(format_ctx, nullptr);
    if (ret < 0) {
        return;
    }

    av_dump_format(format_ctx, 0, nullptr, 1);

    swrContext = swr_alloc();
    swr_alloc_set_opts(swrContext,
                       dst_frame->channel_layout,
                       (AVSampleFormat) dst_frame->format,
                       dst_frame->sample_rate,
                       src_frame->channel_layout,
                       (AVSampleFormat) src_frame->format,
                       src_frame->sample_rate,
                       0,
                       nullptr);
    swr_init(swrContext);

    file_stream = new std::ofstream(file_name, std::ios::binary | std::ios::out);
    encode_buffer = new CircleBuffer<short>(ENCODE_BUFFER_SIZE);
    buffer_key = encode_buffer->getReadKey();

//    buffer_size = av_samples_get_buffer_size(nullptr, src_frame->channels, src_frame->nb_samples, (AVSampleFormat) src_frame->format, 0) / sizeof(short);
    buffer_size = src_frame->nb_samples * in_channels;
}

void AudioEncoder::encode(short *data, int len) {

    encode_buffer->write(data, len);

    while (encode_buffer->length(buffer_key) >= buffer_size) {
        encode_buffer->read(buffer_key, (short *) (src_frame->data[0]), buffer_size);
        src_frame->pts = pts;

//        int ret = swr_convert_frame(swrContext, dst_frame, src_frame);
        int ret = swr_convert(swrContext, (uint8_t **) (dst_frame->data), dst_frame->nb_samples, (const uint8_t **) (src_frame->data), src_frame->nb_samples);

        if (ret < 0) {
            break;
        }
        dst_frame->pts = pts;
        encode(dst_frame);
        pts++;

    }
}

void AudioEncoder::encode(AVFrame *av_frame) {
    int ret = avcodec_send_frame(codec_ctx, av_frame);
    while (ret >= 0) {
        ret = avcodec_receive_packet(codec_ctx, packet);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            break;
        } else if (ret < 0) {
            break;
        }
        av_write_frame(format_ctx, packet);
        file_stream->write((const char *) packet->data, packet->size);
        av_packet_unref(packet);
    }
}

void AudioEncoder::close() {

    int len = encode_buffer->length(buffer_key);
    if (len > 0) {
        encode_buffer->read(buffer_key, (short *) (src_frame->data[0]), len);
        src_frame->pts = pts;
        int ret = swr_convert(swrContext, (uint8_t **) (dst_frame->data), dst_frame->nb_samples, (const uint8_t **) (src_frame->data), len / src_frame->channels);
        dst_frame->pts = pts;
        encode(dst_frame);
        pts++;
    }

    while (swr_convert_frame(swrContext, dst_frame, nullptr) == 0) {
        if (dst_frame->nb_samples > 0) {
            dst_frame->pts = pts;
            encode(dst_frame);
        } else {
            break;
        }
        pts++;
    }

    encode(nullptr);

    av_write_trailer(format_ctx);

    file_stream->close();
    DELETEOBJ(file_stream)
    DELETEOBJ(encode_buffer);

    if (format_ctx) {
        avformat_close_input(&format_ctx);
    }
    if (codec_ctx) {
        avcodec_close(codec_ctx);
        avcodec_free_context(&codec_ctx);
    }

    if (src_frame) {
        av_frame_free(&src_frame);
        src_frame = nullptr;
    }

    if (dst_frame) {
        av_frame_free(&dst_frame);
        dst_frame = nullptr;
    }

    if (packet) {
        av_packet_free(&packet);
        packet = nullptr;
    }
}