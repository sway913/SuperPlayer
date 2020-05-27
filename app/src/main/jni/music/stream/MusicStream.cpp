//
// Created by ldr on 2019-11-18.
//

#include "MusicStream.h"
#include "../datasource/PcmDataSource.h"
#include "../common/Mix.hpp"
#include "../datasource/SimplePcmDataSource.h"

MusicStream::MusicStream() {
    fileDecoder = new FileDecoder();
    mix = new Mix();
}


bool MusicStream::onAudioDataReady(int16_t *data, int32_t numFrames) {
    if (status != STOP) {
        int channelCount = outputStream->getChannelCount();
        if (status == PAUSE) {
            for (int j = 0; j < numFrames * channelCount; ++j) {
                data[j] = 0;
            }
            LOGI("pausing");
            return true;
        }

        for (int j = 0; j < numFrames * channelCount; ++j) {
            for (int i = 0; i < sources.size(); ++i) {
                if (sources[i]->isReady()) {
                    int16_t value;
                    if (sources[i]->getData(value)) {
                        if (i == 0) {
                            data[j] = value;
                        } else {
                            data[j] = mix->mixData(data[j], value);
                        }
                    } else {
                        data[j] += 0;
                    }
                } else {
                    data[j] += 0;
                }
            }
        }

        double time = ((double) numFrames / (double) outputStream->getSampleRate()) * 1000;
        currentMs += (int64_t) time;

        int emptyCount = 0;
        for (auto &source : sources) {
            if (source->isEmpty()) {
                emptyCount++;
                LOGI("data %d is empty",emptyCount);
            } else {
                source->requestData();
            }
        }
        if(emptyCount == sources.size()){
            if (observer) {
                observer->onCompleted();
            }
            return false;
        }

        return true;
    }
    return false;
}

void MusicStream::prepare(const char *musicPath, const char *decodeFilePath, const char *vocalPath) {
    status = INIT;
    decodePath = const_cast<char *>(decodeFilePath);
    auto *file = new ifstream(vocalPath, std::ios::binary);
    file->seekg(0, std::ios::end);
    vocalFileLength = (int) file->tellg();
    file->close();
    delete file;
    totalMs = static_cast<int64_t>(vocalFileLength / (double) (DEFAULT_SAMPLE * DEFAULT_CHANNEL_COUNT * sizeof(int16_t)) * 1000);

    AudioProperty outProperty;
    outProperty.sampleRate = outputStream->getSampleRate();
    outProperty.channelCount = outputStream->getChannelCount();

    AudioProperty inProperty = AudioProperty();

    //sources.push_back(new PcmDataSource(outProperty, AudioProperty(), vocalPath));
    //sources.push_back(new PcmDataSource(outProperty, inProperty, decodeFilePath));
    sources.push_back(new SimplePcmDataSource(outProperty, AudioProperty(), vocalPath));
    sources.push_back(new SimplePcmDataSource(outProperty, inProperty, decodeFilePath));
    fileDecoder->init(musicPath, decodeFilePath, inProperty, vocalFileLength);
    fileDecoder->startDecode(std::bind(&MusicStream::onDecodeCompleted, this));
}

void MusicStream::setOutputStream(AudioStream *outputStream) {
    this->outputStream = outputStream;
}

void MusicStream::start() {
    status = PLAYING;
}

void MusicStream::resume() {
    status = PLAYING;
}

void MusicStream::pause() {
    status = PAUSE;
}

void MusicStream::stop() {
    status = STOP;
    for (auto &source : sources) {
        source->stop();
    }
    fileDecoder->stop();
}

void MusicStream::seek(int64_t millis) {
    for (auto &source : sources) {
        source->seek(millis);
    }
    currentMs = millis;
}


void MusicStream::setVolume(float volume, int track) {
    if (track == 0) {
        sources[0]->setVolume(volume);
    } else {
        sources[1]->setVolume(volume);
    }
}


void MusicStream::setFilter(int type) {
    sources[0]->setFilter(type);
}

void MusicStream::setEcho(bool isEcho) {

}

void MusicStream::setPitch(int pitch) {
    sources[1]->setPitch(pitch);
}

void MusicStream::onDataReady() {

}

void MusicStream::onDecodeCompleted() {
    truncateFile();
    for (auto &source : sources) {
        source->prepare();
    }
    if (observer) {
        observer->onDataReady();
    }
    LOGI("decode complete");
}

int64_t MusicStream::getTotalMs() {
    return totalMs;
}

int64_t MusicStream::getCurrentMs() {
    return currentMs;
}

void MusicStream::truncateFile() {
    if (vocalFileLength == 0) {
        return;
    }
    auto *file = new ifstream(decodePath, std::ios::binary);
    file->seekg(0, std::ios::end);
    int musicLength = (int) file->tellg();
    file->close();
    delete file;

    if (musicLength > vocalFileLength) {
        truncate(decodePath, vocalFileLength);
    }

}

MusicStream::~MusicStream() {
    delete fileDecoder;
    delete mix;
}
