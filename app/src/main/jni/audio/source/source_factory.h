//
// Created by ldr on 2020/5/27.
//

#ifndef SUPERPLAYER_SOURCE_FACTORY_H
#define SUPERPLAYER_SOURCE_FACTORY_H

#include <string>
#include <vector>
#include "isource.h"
#include "pcm_source.h"
#include "stream_source.h"

using namespace std;

class SourceFactory {

private:

    const char * vocalPath{nullptr};
    const char * accPath{nullptr};
    const char * funPath{nullptr};
    int out_sample{0};


public:

    SourceFactory(const char *vocalPath, const char *accPath, const char *funPath, int outSample);

    vector<ISource *> createPcmSource();

    vector<ISource *> createStreamSource();
};

#endif //SUPERPLAYER_SOURCE_FACTORY_H
