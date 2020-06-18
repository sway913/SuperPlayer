//
// Created by Lvlingling on 2020-06-03.
//

#ifndef SUPERPLAYER_PCM_SOURCE_H
#define SUPERPLAYER_PCM_SOURCE_H


#include <fstream>
#include "isource.h"
#include "../../common/lock_free_queue.h"


class PcmSource : public ISource {

public:

    PcmSource(const char *file, int out_sample, int index);

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

    virtual ~PcmSource();

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
    ifstream *file_stream{nullptr};
    long total_size{0};
    long read_size{0};
    long total_ms{0};
    char *read_buffer{nullptr};
    int buffer_size = 1024;
    ResampleHelper *resampleHelper{nullptr};
    int index{0};
    int sample_rate{0};
    FilterPackage *filterPackage{nullptr};

};


#endif //SUPERPLAYER_PCM_SOURCE_H
