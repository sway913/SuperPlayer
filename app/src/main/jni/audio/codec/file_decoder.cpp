//
// Created by ldr on 2019-11-24.
//

#include "file_decoder.h"
#include <utility>

FileDecoder::FileDecoder(const char *srcPath, const char *dstPath, int outSample)
        : src_path(srcPath), dst_path(dstPath), out_sample(outSample) {}


void FileDecoder::start(std::function<void(void)> callback) {
    thread_result = std::async(std::launch::async, &FileDecoder::decode, this, callback);
}

void FileDecoder::decode(const std::function<void(void)> &callback) {
    unlink(dst_path);
    auto *out_stream = new std::ofstream(dst_path, std::ios::out | std::ios::binary);
    auto *audioDecoder = new AudioDecoder();
    audioDecoder->prepare(src_path, out_sample);
    int64_t total_bytes = audioDecoder->getTotalMs() /  1000 * 44100 * 2 * sizeof(short);
    long decode_bytes = 0;
    AudioBuffer *audio_buffer = nullptr;
    int try_conut = 0;
    while (decode_bytes < total_bytes && !is_exit && try_conut < 100) {
        if (!((audio_buffer = audioDecoder->decodeFrame())->isEmpty())) {
            AudioFrame *frame = nullptr;
            while ((frame = audio_buffer->pop())) {
                if (frame->audio_data && frame->len > 0) {
                    out_stream->write(frame->audio_data, frame->len);
                    decode_bytes += frame->len;
                } else {
                    try_conut++;
                }
                DELETEOBJ(frame)
            }
            DELETEOBJ(audio_buffer)
        } else {
            try_conut++;
        }
    }

    out_stream->flush();
    out_stream->close();
    audioDecoder->close();

    if (callback && !is_exit) {
        callback();
    }

    DELETEOBJ(out_stream)
    DELETEOBJ(audioDecoder)
}

void FileDecoder::stop() {
    is_exit = true;
    JOIN(thread_result);
}

FileDecoder::~FileDecoder() = default;
