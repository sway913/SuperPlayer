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
    auto *out_stream = new std::ofstream(dst_path, std::ios::out | std::ios::binary);
    auto *ff_decoder = new FFDecoder();
    ff_decoder->prepare(src_path, out_sample);
    AudioBuffer *audio_buffer = nullptr;
    while (!((audio_buffer = ff_decoder->decodeFrame())->isEmpty())) {
        AudioFrame *frame = audio_buffer->pop();
        if (frame && frame->audio_data && frame->len > 0) {
            out_stream->write(frame->audio_data, frame->len);
        }
        DELETEOBJ(frame)
        DELETEOBJ(audio_buffer)
    }

    out_stream->flush();
    out_stream->close();
    ff_decoder->close();

    if (callback) {
        callback();
    }

    DELETEOBJ(out_stream)
    DELETEOBJ(ff_decoder)
}

void FileDecoder::stop() {
    JOIN(thread_result);
}

FileDecoder::~FileDecoder() = default;
