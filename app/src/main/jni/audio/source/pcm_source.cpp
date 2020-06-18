//
// Created by Lvlingling on 2020-06-03.
//

#include "pcm_source.h"

PcmSource::PcmSource(const char *file, int out_sample, int index) {
    data_queue = new LockFreeQueue<short, kDataQueueSize>();
    file_stream = new ifstream(file, std::ios::binary);
    file_stream->seekg(0, std::ios::beg);
    file_stream->seekg(0, std::ios::end);
    total_size = (int) file_stream->tellg();
    file_stream->seekg(0, std::ios::beg);
    read_buffer = new char[buffer_size];
    resampleHelper = new ResampleHelper(2, 44100, 2, out_sample);
    this->index = index;
    this->sample_rate = out_sample;
    total_ms = (double) total_size * 1000 / (44100 * 2 * sizeof(short));
}

void PcmSource::setObserver(std::function<void(long, int)> observer) {
    this->callBack = observer;
}

void PcmSource::start() {
    is_exit = false;
    thread_result = std::async(std::launch::async, &PcmSource::produceData, this);
}

void PcmSource::resume() {
    is_pause = false;
    cond.notify_all();

}

void PcmSource::pause() {
    is_pause = true;
}

void PcmSource::stop() {
    if (!is_exit) {
        is_exit = true;
        cond.notify_all();
        JOIN(thread_result);
    }
}

bool PcmSource::getData(short &value) {
    return data_queue->pop(value);
}

void PcmSource::requestData() {
    cond.notify_all();
}

bool PcmSource::isEmpty() {
    return data_queue->size() == 0;
}

bool PcmSource::isReady() {
    return read_packet_count > 0;
}

void PcmSource::produceData() {
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

        if (read_size < 0) {
            read_size = 0;
        }
        file_stream->seekg(read_size, std::ios::beg);

        int32_t remain_size = total_size - read_size;
        int32_t frame_size = 0;
        if (remain_size >= buffer_size) {
            frame_size = buffer_size;
        } else {
            frame_size = remain_size;
        }
        file_stream->read(read_buffer, frame_size);
        frame_size = static_cast<int32_t>(file_stream->gcount());
        read_size += frame_size;

        int in_sample = frame_size / (float) (2.0f * sizeof(short));
        int out_sample = in_sample * ((float) sample_rate / 44100) * 1.1f;
        int out_size = (int) (out_sample * 2 * sizeof(short));
        auto *out = new uint8_t[out_size];
        int len = resampleHelper->resample(out, out_sample, (const uint8_t *) read_buffer, in_sample);
        if (len > 0) {
            int32_t size = len * 2;
            auto *tmp_buffer = (short *) (out);
            if (filterPackage) {
                filterPackage->process(tmp_buffer, size);
            }
            for (int j = 0; j < size; j++) {
                data_queue->push(tmp_buffer[j]);
            }
            if (read_packet_count++ == 1) {
                callBack(total_ms, index);
            }
        }
        DELETEARR(out)
    }
    file_stream->close();
}

void PcmSource::seek(int64_t ms) {
    LOGI("pcm source seek ms %ld", ms);
    data_queue->reset();
    read_size = ms / 1000.0f * 44100 * 2 * sizeof(short);
}

int PcmSource::getIndex() {
    return index;
}

void PcmSource::setFilter(FilterPackage *filter) {
    this->filterPackage = filter;
}

PcmSource::~PcmSource() {
    DELETEOBJ(data_queue)
    DELETEOBJ(file_stream)
    DELETEOBJ(read_buffer)
    DELETEOBJ(resampleHelper)
}
