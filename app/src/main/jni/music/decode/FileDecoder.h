//
// Created by ldr on 2019-11-24.
//

#ifndef SUPERPLAYER_FILEDECODER_H
#define SUPERPLAYER_FILEDECODER_H

#include <fstream>
#include <future>
#include "FFDecoder.h"


class FileDecoder {

private:
    std::future<void> decodeResult;
    int decodeLength;
    const char *fileName;
    const char *decodePath;
    AudioProperty outProperty;

    std::function<void(void)> decodeCallback;

    void decodeFun();

public:

    void init(const char *fileName, const char *decodePath, AudioProperty outProperty, int decodeLength = 0);

    void startDecode(std::function<void(void)> decodeCallback);

    void stop();

    virtual ~FileDecoder();
};


#endif //SUPERPLAYER_FILEDECODER_H
