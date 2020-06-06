//
// Created by ldr on 2020/5/29.
//

#include "source_factory.h"
#include <utility>

SourceFactory::SourceFactory(const char *vocalPath, const char *accPath, const char *funPath, int outSample)
        : vocalPath(vocalPath), accPath(accPath), funPath(funPath), out_sample(outSample) {}

vector<ISource *> SourceFactory::createPcmSource() {
    auto source = vector<ISource *>();
//    source.push_back(new PcmSource(vocalPath, out_sample, 1));
    source.push_back(new PcmSource(funPath, out_sample, 2));
    return source;
}


vector<ISource *> SourceFactory::createStreamSource() {
    return vector<ISource *>();
}

