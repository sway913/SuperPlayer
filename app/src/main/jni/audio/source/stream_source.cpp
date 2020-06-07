//
// Created by Lvlingling on 2020-06-03.
//

#include "stream_source.h"

StreamSource::StreamSource(const char *file, int out_sample, int index) {
    this->data_queue = new LockFreeQueue<short, kDataQueueSize>();
    this->index = index;
    this->sample_rate = out_sample;
    this->file_path = file;
}

void StreamSource::setObserver(std::function<void(long, int)> observer) {
    this->callBack = observer;
}

void StreamSource::start() {
    thread_result = std::async(std::launch::async, &StreamSource::produceData, this);
}

void StreamSource::resume() {
    is_pause = false;
    cond.notify_all();

}

void StreamSource::pause() {
    is_pause = true;
}

void StreamSource::stop() {
    is_exit = true;
    cond.notify_all();
    JOIN(thread_result);
}

bool StreamSource::getData(short &value) {
    return data_queue->pop(value);
}

void StreamSource::requestData() {
    cond.notify_all();
}

bool StreamSource::isEmpty() {
    return data_queue->size() == 0;
}

bool StreamSource::isReady() {
    return read_packet_count > 0;
}

void StreamSource::produceData() {
    ff_decoder = new FFDecoder();
    ff_decoder->prepare(file_path, sample_rate);
    total_ms = ff_decoder->getTotalMs();
    while (!is_exit) {
        unique_lock<std::mutex> lock(mutex);
        cond.wait(lock, [this] {
            if (is_exit) {
                return true;
            } else if (is_pause) {
                return false;
            } else {
                return data_queue->size() < kDataQueueSize / 2;
            }
        });

        if (is_exit) {
            break;
        }

        AudioBuffer *audio_buffer = ff_decoder->decodeFrame();
        AudioFrame *frame = nullptr;
        int count = 0;
        while ((frame = audio_buffer->pop())) {
            if (frame->audio_data && frame->len > 0) {
                int32_t size = frame->len / sizeof(short);
                auto *tmp_buffer = (short *) (frame->audio_data);
                for (int j = 0; j < size; j++) {
                    data_queue->push(tmp_buffer[j]);
                }
                if (count++ == 0) {
                    if (++read_packet_count == 1) {
                        callBack(total_ms, index);
                    }
                }
            }
            DELETEOBJ(frame)
        }
        DELETEOBJ(audio_buffer)
    }
    ff_decoder->close();
}

StreamSource::~StreamSource() {
    DELETEOBJ(data_queue)
    DELETEOBJ(ff_decoder)
    file_path = nullptr;
}