//
// Created by ldr on 2020/9/14.
//

#include "FFVideoEncoder.h"
#include "../../common/android_log.h"
#include "../../common/common_tools.h"

FFVideoEncoder::FFVideoEncoder(int w, int h, const char *path) {
    data_queue = new BlockQueue<uint8_t *>();
    int ret;
    ret = avformat_alloc_output_context2(&fmt_ctx, nullptr, nullptr, path);
    if (ret < 0) {
        LOGI("ffencode avformat_alloc_output_context2 error   %s", av_err2str(ret));
        return;
    }
    fmt = fmt_ctx->oformat;

    av_codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!av_codec) {
        LOGI("ffencode can't find encoder");
        return;
    }

    video_st = avformat_new_stream(fmt_ctx, av_codec);
    if (!video_st) {
        LOGI("ffencode video stream null");
        return;
    }
    video_st->id = 0;

    codec_ctx = video_st->codec;
    if (codec_ctx == nullptr) {
        LOGI("ffencode codec ctx null");
    }
//    LOGI("ffencode file path %s", path);
    codec_ctx->codec_id = AV_CODEC_ID_H264;


    codec_ctx->codec_type = AVMEDIA_TYPE_VIDEO;
    codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;

    codec_ctx->width = h;
    codec_ctx->height = w;

    codec_ctx->bit_rate = 4000000;
    codec_ctx->gop_size = 250;
//    codec_ctx->thread_count = 2;

    codec_ctx->time_base.num = 1;
    codec_ctx->time_base.den = 25;

//    codec_ctx->codec_tag = 0;
//    codec_ctx->has_b_frames = 0;

    codec_ctx->qmin = 10;
    codec_ctx->qmax = 51;
    codec_ctx->max_b_frames = 3;


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
        LOGI("ffencode fail to open encoder");
        return;
    }

    if (avio_open(&fmt_ctx->pb, path, AVIO_FLAG_WRITE) < 0 || fmt_ctx->pb == nullptr) {
        LOGI("ffencode fail to open file");
        return;
    }

    fmt_ctx->video_codec_id = codec_ctx->codec_id;
    if (avformat_write_header(fmt_ctx, nullptr) < 0) {
        LOGI("ffencode avformat_write_header error   %s  file %s", av_err2str(ret), path);
    }

    av_frame = av_frame_alloc();
    pic_size = avpicture_get_size(codec_ctx->pix_fmt, codec_ctx->width, codec_ctx->height);
    auto *buf = (uint8_t *) av_malloc(pic_size);
    avpicture_fill((AVPicture *) av_frame, buf, codec_ctx->pix_fmt, codec_ctx->width, codec_ctx->height);

    av_packet = static_cast<AVPacket *>(av_mallocz(sizeof(AVPacket)));
    av_init_packet(av_packet);
//    av_new_packet(av_packet, pic_size);
    out_y_size = codec_ctx->width * codec_ctx->height;
    thread_handler = std::async(std::launch::async, &FFVideoEncoder::looper, this);
}

void FFVideoEncoder::encodeFrame(uint8_t *data) {
    auto *frame = new uint8_t[codec_ctx->width * codec_ctx->coded_height * 3 / 2];
    memcpy(frame, data, codec_ctx->width * codec_ctx->coded_height * 3 / 2);
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

        auto *dst = new uint8_t[codec_ctx->width * codec_ctx->height * 3 / 2];
        NV21_TO_yuv420P(dst, buf, codec_ctx->width, codec_ctx->height);
//        do_filter(buf, codec_ctx->width * codec_ctx->height, ROTATE_270_CROP_LT_MIRROR_LR);

        av_frame->data[0] = dst;
        av_frame->data[1] = dst + out_y_size;
        av_frame->data[2] = dst + out_y_size * 5 / 4;

//        rotateYUV420Degree270(buf, dst, codec_ctx->width, codec_ctx->height);
//        av_frame->data[0] = buf;
//        av_frame->data[1] = buf + out_y_size;
//        av_frame->data[2] = buf + out_y_size * 5 / 4;

        av_frame->pts = frame_cnt * (video_st->time_base.den) / ((video_st->time_base.num) * 25);
        frame_cnt++;
        int got_pic = 0;
        int ret = avcodec_encode_video2(codec_ctx, av_packet, av_frame, &got_pic);
        if (ret < 0) {
            LOGI("fail to encode");
        }
        if (got_pic && ret == 0) {
            av_packet->stream_index = video_st->index;
            ret = av_write_frame(fmt_ctx, av_packet);
            av_packet_unref(av_packet);
            if (ret < 0) {
                LOGI("write failed  %s", av_err2str(ret));
            }
        }
        delete[] buf;
        delete[] dst;
    }
    if (flush() < 0) {
        return;
    }
    av_write_trailer(fmt_ctx);
    if (video_st) {
        avcodec_close(video_st->codec);
        av_free(av_frame);
    }

    avio_close(fmt_ctx->pb);
    avformat_free_context(fmt_ctx);
}

int FFVideoEncoder::flush() {
    LOGI("encoder flush");
    int ret, got_pkt = 1;
    while (got_pkt) {
        av_packet->data = nullptr;
        av_packet->size = 0;
        av_init_packet(av_packet);
        ret = avcodec_encode_video2(codec_ctx, av_packet, nullptr, &got_pkt);
        av_frame_free(nullptr);
        if (ret == 0 && got_pkt) {
            av_packet->stream_index = 0;
            int write_ret = av_write_frame(fmt_ctx, av_packet);
            av_packet_unref(av_packet);
            if (write_ret < 0) {
                LOGI("write failed  %s", av_err2str(write_ret));
                break;
            }
        } else {
            break;
        }
    }
    LOGI("encoder flush end");
    return true;
}

void FFVideoEncoder::do_filter(const uint8_t *picture_buf, int in_y_size, int format) {
    int w = codec_ctx->height;
    int h = codec_ctx->width;
    //   y值在H方向开始行
    int y_height_start_index = 0;
    //   uv值在H方向开始行
    int uv_height_start_index = y_height_start_index / 2;

    if (format == ROTATE_90_CROP_LT) {

        for (int i = y_height_start_index; i < h; i++) {

            for (int j = 0; j < codec_ctx->width; j++) {

                int index = codec_ctx->width * i + j;
                uint8_t value = *(picture_buf + index);
                *(av_frame->data[0] + j * codec_ctx->coded_height + (h - (i - y_height_start_index) - 1)) = value;
            }
        }

        for (int i = uv_height_start_index; i < h / 2; i++) {
            for (int j = 0; j < w / 2; j++) {
                int index = w / 2 * i + j;
                uint8_t v = *(picture_buf + in_y_size + index);
                uint8_t u = *(picture_buf + in_y_size * 5 / 4 + index);
                *(av_frame->data[2] + (j * h / 2 + (h / 2 - (i - uv_height_start_index) - 1))) = v;
                *(av_frame->data[1] + (j * h / 2 + (h / 2 - (i - uv_height_start_index) - 1))) = u;
            }
        }
    } else if (format == ROTATE_0_CROP_LT) {


        for (int i = y_height_start_index; i < h; i++) {

            for (int j = 0; j < w; j++) {

                int index = w * i + j;
                uint8_t value = *(picture_buf + index);

                *(av_frame->data[0] + (i - y_height_start_index) * w +
                  j) = value;
            }
        }


        for (int i = uv_height_start_index; i < h / 2; i++) {
            for (int j = 0; j < w / 2; j++) {

                int index = w / 2 * i + j;
                uint8_t v = *(picture_buf + in_y_size + index);

                uint8_t u = *(picture_buf + in_y_size * 5 / 4 + index);
                *(av_frame->data[2] +
                  ((i - uv_height_start_index) * w / 2 + j)) = v;
                *(av_frame->data[1] +
                  ((i - uv_height_start_index) * w / 2 + j)) = u;
            }
        }
    } else if (format == ROTATE_270_CROP_LT_MIRROR_LR) {

        int y_width_start_index = w - w;
        int uv_width_start_index = y_width_start_index / 2;

        for (int i = 0; i < h; i++) {

            for (int j = y_width_start_index; j < w; j++) {

                int index = w * (h - i - 1) + j;
                uint8_t value = *(picture_buf + index);
                *(av_frame->data[0] + (w - (j - y_width_start_index) - 1)
                                      * h +
                  i) = value;
            }
        }
        for (int i = 0; i < h / 2; i++) {
            for (int j = uv_width_start_index; j < w / 2; j++) {
                int index = w / 2 * (h / 2 - i - 1) + j;
                uint8_t v = *(picture_buf + in_y_size + index);
                uint8_t u = *(picture_buf + in_y_size * 5 / 4 + index);
                *(av_frame->data[2] + (w / 2 - (j - uv_width_start_index) - 1)
                                      * h / 2 +
                  i) = v;
                *(av_frame->data[1] + (w / 2 - (j - uv_width_start_index) - 1)
                                      * h / 2 +
                  i) = u;
            }
        }
    }

}

void FFVideoEncoder::stop() {
    end = true;
    JOIN(thread_handler);
}

int FFVideoEncoder::NV21_TO_yuv420P(uint8_t *dst, uint8_t *nv21, int w, int h) {
    int framesize = w * h;
    int i = 0, j = 0;
    auto *nv12 = new uint8_t[w * h * 3 / 2];
    memcpy(nv12, nv21, framesize);

    for (i = 0; i < framesize; i++) {
        nv12[i] = nv21[i];
    }

    for (j = 0; j < framesize / 2; j += 2) {
        nv12[framesize + j - 1] = nv21[j + framesize];
    }

    for (j = 0; j < framesize / 2; j += 2) {
        nv12[framesize + j] = nv21[j + framesize - 1];
    }


    int ySize = w * h;

//y
    for (i = 0; i < ySize; i++) {
        dst[i] = nv12[i];
    }

//u
    i = 0;
    for (j = 0; j < ySize / 2; j += 2) {
        dst[ySize + i] = nv12[ySize + j];
        i++;
    }

//v
    i = 0;
    for (j = 1; j < ySize / 2; j += 2) {
        dst[ySize * 5 / 4 + i] = nv12[ySize + j];
        i++;
    }

    delete[] nv12;

    return 0;

}

void FFVideoEncoder::rotateYUV420Degree270(uint8_t *dst, uint8_t *src, int width, int height) {


}

FFVideoEncoder::~FFVideoEncoder() {
    DELETEOBJ(data_queue)
}
