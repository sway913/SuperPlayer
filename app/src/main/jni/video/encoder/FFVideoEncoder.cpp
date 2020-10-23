//
// Created by ldr on 2020/9/14.
//

#include "FFVideoEncoder.h"
#include "../../common/android_log.h"
#include "../../common/common_tools.h"
#include "../../thirdpart/libyuv/include/libyuv/convert.h"
#include "../../thirdpart/libyuv/include/libyuv.h"

FFVideoEncoder::FFVideoEncoder(int w, int h, const char *path_) {
    this->input_width = 1920;
    this->input_height = 1080;

    this->output_width = 1080;
    this->output_height = 720;

    this->path = path_;
    start();
}

void FFVideoEncoder::start() {
    data_queue = new LockFreeQueue<uint8_t *, 16>();
    int ret;
    ret = avformat_alloc_output_context2(&fmt_ctx, nullptr, nullptr, path);
    if (ret < 0) {
        LOGI("h264_encoder avformat_alloc_output_context2 error %s", av_err2str(ret));
        return;
    }
    fmt = fmt_ctx->oformat;

    av_codec = avcodec_find_encoder(AVCodecID::AV_CODEC_ID_H264);
    if (!av_codec) {
        LOGI("h264_encoder h264_encoder can't find encoder");
        return;
    }

    video_st = avformat_new_stream(fmt_ctx, av_codec);
    if (!video_st) {
        LOGI("h264_encoder video stream null");
        return;
    }
    video_st->id = 0;

    codec_ctx = video_st->codec;
    if (codec_ctx == nullptr) {
        LOGI("h264_encoder codec ctx null");
    }

    codec_ctx->codec_id = AVCodecID::AV_CODEC_ID_H264;
    codec_ctx->codec_type = AVMEDIA_TYPE_VIDEO;
    codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
    codec_ctx->width = output_height;
    codec_ctx->height = output_width;
    codec_ctx->bit_rate = 400000;
    codec_ctx->gop_size = 30;
    codec_ctx->time_base.num = 1;
    codec_ctx->time_base.den = 25;

//    codec_ctx->codec_tag = 0;
//    codec_ctx->has_b_frames = 0;
//    codec_ctx->thread_count = 2;
//    codec_ctx->qmin = 10;
//    codec_ctx->qmax = 51;
//    codec_ctx->max_b_frames = 3;


    if (fmt->flags & AVFMT_GLOBALHEADER) {
        codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    AVDictionary *param = nullptr;
    if (codec_ctx->codec_id == AV_CODEC_ID_H264) {
//        av_opt_set(codec_ctx->priv_data, "preset", "slow", 0);
//        av_opt_set(codec_ctx->priv_data, "tune", "zerolatency", 0);
//        av_opt_set(codec_ctx->priv_data, "crf", "35", 0);
//        av_dict_set(&param, "profile", "baseline", 0);
        av_dict_set(&param, "preset", "slow", 0);
        av_dict_set(&param, "tune", "zerolatency", 0);
    }

//    av_dump_format(fmt_ctx, 0, path, 1);
    if (avcodec_open2(codec_ctx, av_codec, &param) < 0) {
        LOGI("h264_encoder fail to open encoder");
        return;
    }

    if (avio_open(&fmt_ctx->pb, path, AVIO_FLAG_WRITE) < 0 || fmt_ctx->pb == nullptr) {
        LOGI("h264_encoder fail to open file");
        return;
    }

    fmt_ctx->video_codec_id = codec_ctx->codec_id;
    if (avformat_write_header(fmt_ctx, nullptr) < 0) {
        LOGI("h264_encoder avformat_write_header error %s  file %s", av_err2str(ret), path);
    }

    av_frame = av_frame_alloc();
//    int pic_size = avpicture_get_size(codec_ctx->pix_fmt, codec_ctx->width, codec_ctx->height);
//    avpicture_fill((AVPicture *) av_frame, buf, codec_ctx->pix_fmt, codec_ctx->width, codec_ctx->height);

    int pic_size = av_image_get_buffer_size(codec_ctx->pix_fmt, codec_ctx->width, codec_ctx->height, 1);
    dst_buf = (uint8_t *) av_malloc(pic_size);
    av_image_fill_arrays(av_frame->data, av_frame->linesize, dst_buf, codec_ctx->pix_fmt, codec_ctx->width, codec_ctx->height, 1);
    av_packet = av_packet_alloc();//static_cast<AVPacket *>(av_mallocz(sizeof(AVPacket)));
    av_init_packet(av_packet);
//    av_new_packet(av_packet, pic_size * 3);
    thread_handler = std::async(std::launch::async, &FFVideoEncoder::looper, this);
}

void FFVideoEncoder::encodeFrame(uint8_t *data) {
    if (data_queue == nullptr || end) {
        return;
    }
    auto *frame = new uint8_t[input_width * input_height * 3 / 2];
    memcpy(frame, data, input_width * input_height * 3 / 2);
    data_queue->push(frame);
}

int FFVideoEncoder::encode(AVCodecContext *pCodecCtx, AVFrame *pAvFrame, AVPacket *pAvPacket) {
    int ret = avcodec_send_frame(pCodecCtx, pAvFrame);
    if(ret < 0) {
        LOGI("fail to send frame");
        return -1;
    }

    while (!ret) {
        ret = avcodec_receive_packet(pCodecCtx, pAvPacket);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return 0;
        } else if (ret < 0) {
            //error during encoding
            return -1;
        }
        pAvPacket->stream_index = video_st->index;
        ret = av_write_frame(fmt_ctx, pAvPacket);
        av_packet_unref(pAvPacket);
    }
    return 0;
}

void FFVideoEncoder::looper() {
    while (!end) {
        if (data_queue->size() == 0) {
            continue;
        }
        uint8_t *buf = nullptr;
        if (!data_queue->pop(buf)) {
            continue;
        }

        uint8_t *i420_y = dst_buf;
        uint8_t *i420_u = dst_buf + output_width * output_height;
        uint8_t *i420_v = dst_buf + output_width * output_height * 5 / 4;


        libyuv::ConvertToI420(buf, input_width * input_height,
                              i420_y, output_height,
                              i420_u, output_height / 2,
                              i420_v, output_height / 2,
                              (input_width - output_width) / 2, (input_height - output_height) / 2,
                              input_width, input_height,
                              output_width, output_height,
                              libyuv::kRotate270, libyuv::FOURCC_NV21);


        av_frame->data[0] = i420_y;
        av_frame->data[1] = i420_u;
        av_frame->data[2] = i420_v;

        gettimeofday(&tv, nullptr);
        int64_t usec = tv.tv_sec * 1000 * 1000 + tv.tv_usec;
        if (startTime == 0) {
            startTime = usec;
        }
        av_frame->pts = frame_cnt * (video_st->time_base.den) / ((video_st->time_base.num) * 25); //usec - startTime;
        frame_cnt++;
        encode(codec_ctx, av_frame, av_packet);
        delete[] buf;
    }

}


void FFVideoEncoder::stop() {
    end = true;
    JOIN(thread_handler);

    encode(codec_ctx, nullptr, av_packet);
    av_write_trailer(fmt_ctx);

    if(codec_ctx) {
        avcodec_close(codec_ctx);
        avcodec_free_context(&codec_ctx);
        codec_ctx = nullptr;
    }

    if(fmt_ctx) {
        avio_close(fmt_ctx->pb);
        avformat_free_context(fmt_ctx);
        fmt_ctx = nullptr;
    }

    if (dst_buf) {
        av_free(dst_buf);
        dst_buf = nullptr;
    }

    if(av_frame) {
        av_free(av_frame);
        av_frame = nullptr;
    }

    if (av_packet) {
        av_packet_free(&av_packet);
    }
    DELETEOBJ(data_queue)
}


FFVideoEncoder::~FFVideoEncoder() = default;
