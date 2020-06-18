//
// Created by ldr on 2020/5/29.
//

#include "source_factory.h"
#include <utility>

SourceFactory::SourceFactory(const char *vocalPath, const char *accPath, const char *guidePath, const char *decodePath, int outSample) {
    this->vocalPath = vocalPath;
    this->accPath = accPath;
    this->guidePath = guidePath;
    this->decodePath = decodePath;
    this->out_sample = outSample;
}

vector<ISource *> SourceFactory::createPcmSource() {
    auto source = vector<ISource *>();
    source.push_back(new PcmSource(vocalPath, out_sample, 1));
    source.push_back(new PcmSource(decodePath, out_sample, 2));
    return source;
}


vector<ISource *> SourceFactory::createStreamSource() {
    auto source = vector<ISource *>();
    source.push_back(new StreamSource(accPath, out_sample, 2));
    return source;
}

SourceFactory::~SourceFactory() {
    vocalPath = nullptr;
    accPath = nullptr;
    guidePath = nullptr;
    decodePath = nullptr;
}

