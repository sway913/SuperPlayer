//
// Created by ldr on 2019-11-08.
//

#include "../common/AndroidLog.h"
#include "BaseEngine.h"
#include <utility>

BaseEngine::BaseEngine(bool isRecordMode) {
    status = STOP;
    this->isRecordMode = isRecordMode;
};

bool BaseEngine::initOboe(int sample) {
    int channelCount;
    if (isRecordMode) {
        channelCount = Mono;
    } else {
        channelCount = Stereo;
    }
    AudioStreamBuilder outBuilder;
    outBuilder.setCallback(this);
    outBuilder.setAudioApi(AudioApi::Unspecified);
    outBuilder.setSampleRate(sample);
    outBuilder.setFormat(AudioFormat::I16);
    outBuilder.setChannelCount(channelCount);
    outBuilder.setPerformanceMode(PerformanceMode::LowLatency);
    outBuilder.setSharingMode(SharingMode::Exclusive);
    outBuilder.setDirection(Direction::Output);
    Result outResult = outBuilder.openStream(&outputStream);
    outputStream->setBufferSizeInFrames(outputStream->getFramesPerBurst() * 2);
    if (!isRecordMode) {
        return outResult == Result::OK;
    } else {
        AudioStreamBuilder inBuilder;
        inBuilder.setCallback(nullptr);
        inBuilder.setAudioApi(AudioApi::Unspecified);
        inBuilder.setSampleRate(sample);
        inBuilder.setFormat(AudioFormat::I16);
        inBuilder.setChannelCount(channelCount);
        inBuilder.setPerformanceMode(PerformanceMode::LowLatency);
        inBuilder.setSharingMode(SharingMode::Exclusive);
        inBuilder.setDirection(Direction::Input);
        Result inResult = inBuilder.openStream(&inputStream);
        inputStream->setBufferSizeInFrames(inputStream->getFramesPerBurst() * 2);
        return inResult == Result::OK;
    }
}

void BaseEngine::prepare(int sample, const char *musicPath, const char *guidePath, const char *vocalPath) {
    if (initOboe(sample)) {
        status = INIT;
        sourceStream = setupStream();
        if (sourceStream) {
            sourceStream->setObserver(observer);
            sourceStream->prepare(musicPath, guidePath, vocalPath);
        }
    } else {
        status = ERROR;
    }
}

void BaseEngine::setObserver(Observer *observer) {
    this->observer = observer;
}


DataCallbackResult BaseEngine::onAudioReady(AudioStream *stream, void *data, int32_t numFrames) {
    if (!sourceStream) {
        return DataCallbackResult::Stop;
    }
    if (status != STOP) {
        if (sourceStream->onAudioDataReady(static_cast<int16_t *>(data), numFrames)) {
            return DataCallbackResult::Continue;
        } else {
            return DataCallbackResult::Stop;
        }
    } else {
        return DataCallbackResult::Stop;
    }
}

void BaseEngine::start() {
    if (status == INIT) {
        status = PLAYING;

        if (sourceStream) {
            sourceStream->start();
        }

        if (outputStream != nullptr) {
            outputStream->requestStart();
        }
        if (inputStream != nullptr) {
            inputStream->requestStart();
        }
    }
}

void BaseEngine::resume() {
    if (status == PAUSE) {
        status = PLAYING;
        if (sourceStream)
            sourceStream->resume();
    }
}

void BaseEngine::pause() {
    if (status == PLAYING) {
        status = PAUSE;
        if (sourceStream)
            sourceStream->pause();
    }
}

void BaseEngine::stop() {
    status = STOP;
    if (sourceStream) {
        sourceStream->stop();
    }
    if (outputStream != nullptr) {
        outputStream->requestStop();
        outputStream->close();
    }
    if (inputStream != nullptr) {
        inputStream->requestStop();
        inputStream->close();
    }
}

void BaseEngine::setFilter(int type) {
    if (sourceStream) {
        sourceStream->setFilter(type);
    }
}

void BaseEngine::setEcho(bool isEcho) {
    if (sourceStream) {
        sourceStream->setEcho(isEcho);
    }
}

void BaseEngine::setVolume(float volume, int track) {
    if (sourceStream) {
        sourceStream->setVolume(volume, track);
    }
}

void BaseEngine::setPitch(int pitch) {
    if (sourceStream) {
        sourceStream->setPitch(pitch);
    }
}

void BaseEngine::seek(int64_t millis) {
    if (sourceStream) {
        sourceStream->seek(millis);
    }
}

int64_t BaseEngine::getTotalMs() {
    return sourceStream->getTotalMs();
}

int64_t BaseEngine::getCurrentMs() {
    if (sourceStream) {
        return sourceStream->getCurrentMs();
    } else {
        return 0;
    }
}

void BaseEngine::onErrorBeforeClose(AudioStream *oboeStream, Result error) {

}

void BaseEngine::onErrorAfterClose(AudioStream *oboeStream, Result error) {
    if (observer) {
        observer->onError();
    }
}

BaseEngine::~BaseEngine() {
    if (!outputStream) {
        delete outputStream;
        outputStream = nullptr;
    }

    if (!inputStream) {
        delete inputStream;
        inputStream = nullptr;
    }

    if (sourceStream) {
        delete sourceStream;
        sourceStream = nullptr;
    }
}
