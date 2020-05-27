//
// Created by ldr on 2019-11-18.
//

#ifndef SUPERPLAYER_PCMWRITER_H
#define SUPERPLAYER_PCMWRITER_H

#define BUFFER_SIZE 1024


#include <fstream>
#include "../../common/CircleBuffer.h"
#include "../../common/Resample.hpp"
#include <future>

#define BUFFER_SIZE 1024
#define CIRCLE_BUFFER_CAPBILATY 96000

class PcmWriter {

private:
    short *buffer;
    CircleBuffer<short> *inputBuffer;
    std::ofstream *pcmFileStream;
    int bufferReadKey;

    std::future<void> writeResult;
    std::mutex mutex;
    std::condition_variable cond;
    Resample *resample;

    AudioProperty inProperty;
    AudioProperty outProperty;
    bool isWriting = false;

    void writeToFile();

public:

    PcmWriter();

    virtual ~PcmWriter();

    void write(short *data, int count);

    void open(const char *fileName, AudioProperty outProperty, AudioProperty inProperty);

    void seek(int64_t millis);

    void close();

};


#endif //SUPERPLAYER_PCMWRITER_H
