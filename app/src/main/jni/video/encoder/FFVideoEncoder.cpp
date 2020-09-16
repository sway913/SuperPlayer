//
// Created by ldr on 2020/9/14.
//

#include "FFVideoEncoder.h"
#include "../../common/android_log.h"
#include "../../common/common_tools.h"

FFVideoEncoder::FFVideoEncoder(int w, int h, const char *path) {
    data_queue = new BlockQueue<uint8_t *>();
    avformat_alloc_output_context2(&fmt_ctx, nullptr, nullptr, path);
    if (avio_open(&fmt_ctx->pb, path, AVIO_FLAG_READ_WRITE) < 0) {
        LOGI("fail to open file");
        return;
    }

    video_st = avformat_new_stream(fmt_ctx, nullptr);
    if (!video_st) {
        LOGI("video stream null");
        return;
    }

    codec_ctx = video_st->codec;
    codec_ctx->codec_id = AV_CODEC_ID_H264;
    codec_ctx->codec_type = AVMEDIA_TYPE_VIDEO;
    codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;

    codec_ctx->width = h;
    codec_ctx->height = w;

    codec_ctx->bit_rate = 4000000;
    codec_ctx->gop_size = 250;
    codec_ctx->thread_count = 8;

    codec_ctx->time_base.num = 1;
    codec_ctx->time_base.den = 30;

    codec_ctx->qmin = 10;
    codec_ctx->qmax = 51;

    codec_ctx->max_b_frames = 3;

    AVDictionary *param = nullptr;
    if (codec_ctx->codec_id == AV_CODEC_ID_H264) {
        av_opt_set(codec_ctx->priv_data, "preset", "superfast", 0);
        av_dict_set(&param, "profile", "baseline", 0);
    }

    av_dump_format(fmt_ctx, 0, path, 1);

    av_codec = avcodec_find_encoder(codec_ctx->codec_id);

    if (!av_codec) {
        LOGI("can't find encoder");
        return;
    }

    if (avcodec_open2(codec_ctx, av_codec, &param) < 0) {
        LOGI("fail to open encoder");
        return;
    }

    av_frame = av_frame_alloc();
    pic_size = avpicture_get_size(codec_ctx->pix_fmt, codec_ctx->width, codec_ctx->height);
    auto *buf = (uint8_t *) av_malloc(pic_size);
    avpicture_fill((AVPicture *) av_frame, buf, codec_ctx->pix_fmt, codec_ctx->width, codec_ctx->height);

    avformat_write_header(fmt_ctx, nullptr);
    av_new_packet(av_packet, pic_size);
    out_y_size = codec_ctx->width * codec_ctx->height;
    thread_handler = std::async(std::launch::async, &FFVideoEncoder::looper, this);
}

void FFVideoEncoder::encodeFrame(uint8_t *data) {
    auto *frame = new uint8_t[codec_ctx->width * codec_ctx->coded_height * 3 / 2];
    data_queue->push(frame);
}

void FFVideoEncoder::looper() {
    while (!end) {
        if (data_queue->empty()) {
            continue;
        }
        uint8_t *buf = nullptr;
        if (!data_queue->pop(buf)) {
            continue;
        }
        av_frame->pts = frame_cnt;
        frame_cnt++;
        int got_pic = 0;
        int ret = avcodec_encode_video2(codec_ctx, av_packet, av_frame, &got_pic);
        if (ret < 0) {
            LOGI("fail to encode");
        }
        if (got_pic == 1) {
            av_packet->stream_index = video_st->index;
            ret = av_write_frame(fmt_ctx, av_packet);
            av_free_packet(av_packet);
        }
        delete[] buf;
    }
    if (flush(fmt_ctx, 0) < 0) {
        return;
    }
    av_write_trailer(fmt_ctx);
    if(video_st) {
        avcodec_close(video_st->codec);
        av_free(av_frame);
    }

    avio_close(fmt_ctx->pb);
    avformat_free_context(fmt_ctx);
}

int FFVideoEncoder::flush(AVFormatContext *av_fmt_ctx, unsigned int stream_index) {
    int ret;
    int got_frame;
    AVPacket enc_pkt;
//    if (!(av_fmt_ctx->streams[stream_index]->codec->codec->capabilities & CODEC_CAP_DELAY))
//        return 0;
    while (true) {
        enc_pkt.data = nullptr;
        enc_pkt.size = 0;
        av_init_packet(&enc_pkt);
        ret = avcodec_encode_video2(av_fmt_ctx->streams[stream_index]->codec, &enc_pkt, nullptr, &got_frame);
        av_frame_free(nullptr);
        if (ret < 0)
            break;
        if (!got_frame) {
            ret = 0;
            break;
        }
        LOGI("_Flush Encoder: Succeed to encode 1 frame video!\tsize:%5d\n", enc_pkt.size);
        /* mux encoded frame */
        ret = av_write_frame(av_fmt_ctx, &enc_pkt);
        if (ret < 0)
            break;
    }

    return ret;

}

void FFVideoEncoder::stop() {
    end = true;
    JOIN(thread_handler);
}

FFVideoEncoder::~FFVideoEncoder() {
    DELETEOBJ(data_queue)
}
