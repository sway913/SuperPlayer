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

    virtual ~PcmSource();

protected:

    void produceData() override final;


private:

    LockFreeQueue<short, DATA_QUEUE_SIZE> *data_queue{nullptr};
    future<void> thread_result{};
    std::mutex mutex;
    std::condition_variable cond;
    bool is_exit{false};
    bool is_pause{false};
    int read_packet_count{0};
    ifstream *file_stream{nullptr};
    int total_size{0};
    int read_size{0};
    char *read_buffer{nullptr};
    int buffer_size = 1024;
    ResampleHelper *resampleHelper{nullptr};
    int index{0};
    int sample_rate{0};

};


#endif //SUPERPLAYER_PCM_SOURCE_H
