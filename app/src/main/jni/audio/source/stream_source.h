//
// Created by Lvlingling on 2020-06-03.
//

#ifndef SUPERPLAYER_STREAM_SOURCE_H
#define SUPERPLAYER_STREAM_SOURCE_H

#include "isource.h"
#include "../../common/lock_free_queue.h"
#include "../codec/audio_decoder.h"
#include "../filter/filter_package.h"

class StreamSource : public ISource {

public:

    StreamSource(const char *file, int out_sample, int index);

    void start() override final;

    void resume() override final;

    void pause() override final;

    void stop() override final;

    bool getData(short &value) override final;

    void requestData() override final;

    bool isReady() override final;

    bool isEmpty() override final;

    void seek(int64_t ms) override final;

    int getIndex() override final;

    void setFilter(FilterPackage *filterPacage);

    void setObserver(std::function<void(long, int)>) override final;

    virtual ~StreamSource();

protected:

    void produceData() override final;

private:

    LockFreeQueue<short, kDataQueueSize> *data_queue{nullptr};
    future<void> thread_result{};
    std::mutex mutex;
    std::condition_variable cond;
    std::function<void(long, int)> callBack{nullptr};
    bool is_exit{true};
    bool is_pause{false};
    int read_packet_count{0};
    long total_ms{0};
    AudioDecoder *audio_decoder{nullptr};
    int index{0};
    int sample_rate{0};
    const char *file_path{nullptr};
    FilterPackage *filterPackage{nullptr};

};


#endif //SUPERPLAYER_STREAM_SOURCE_H
