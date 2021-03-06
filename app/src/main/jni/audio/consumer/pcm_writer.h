//
// Created by Lvlingling on 2020/6/7.
//

#ifndef SUPERPLAYER_PCM_WRITER_H
#define SUPERPLAYER_PCM_WRITER_H

#include "../../common/common_tools.h"
#include "../../common/circle_buffer.h"
#include "../codec/audio_encoder.h"

#include <future>
#include <fstream>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define CIRCLE_BUFFER_CAPBILATY 4800

class PcmWriter {

private:
    short *tmp_buffer{nullptr};
    CircleBuffer<short> *circle_Buffer{nullptr};
    int bufferReadKey{0};
    bool valid{false};
    int in_sample_rate{0};
    long write_byte_size{0};
    const char *file_name{nullptr};

    AudioEncoder *audioEncoder{nullptr};

    std::future<void> writeResult{};
    std::mutex mutex{};
    std::condition_variable cond{};
    void writeToFile();

public:

    PcmWriter();

    virtual ~PcmWriter();

    void write(short *data, int count);

    void start(const char *fileName, int in_sample_rate);

    void seek(int64_t millis);

    void stop();

};


#endif //SUPERPLAYER_PCM_WRITER_H
