//
// Created by ldr on 2019-11-05.
//

#include "RecorderEngine.h"
#include "../../stream/VocalStream.h"


IStream *RecorderEngine::setupStream() {
    auto *vocalStream = new VocalStream();
    vocalStream->init(inputStream, outputStream);
    return vocalStream;
}

RecorderEngine::RecorderEngine() : BaseEngine(true) {}

void RecorderEngine::prepare(int sample, const char *musicPath, const char *guidePath, const char *vocalPath) {
    BaseEngine::prepare(sample, musicPath, guidePath, vocalPath);
    if (status == INIT) {
        if (outputStream != nullptr) {
            outputStream->requestStart();
        }
        if (inputStream != nullptr) {
            inputStream->requestStart();
        }
    }
}

void RecorderEngine::start() {
    if (status == INIT && sourceStream) {
        status = PLAYING;
        sourceStream->start();
    }
}

RecorderEngine::~RecorderEngine() = default;


