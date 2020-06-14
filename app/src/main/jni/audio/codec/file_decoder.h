//
// Created by ldr on 2019-11-24.
//

#ifndef SUPERPLAYER_FILE_DECODER_H
#define SUPERPLAYER_FILE_DECODER_H

#include <fstream>
#include <future>
#include <unistd.h>
#include "audio_decoder.h"
#include "../../common/common_tools.h"

class FileDecoder {

private:
    std::future<void> thread_result;
    const char *src_path;
    const char *dst_path;
    int out_sample{0};

    void decode(const std::function<void(void)>& callback);

public:

    FileDecoder(const char *srcPath, const char *dstPath, int outSample);

    void start(std::function<void(void)> callback);

    void stop();

    virtual ~FileDecoder();
};


#endif //SUPERPLAYER_FILE_DECODER_H
