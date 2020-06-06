//
// Created by ldr on 2019-11-18.
//

#include "VocalStream.h"
#include "../datasource/StreamDataSource.h"
#include "../common/Mix.hpp"

VocalStream::VocalStream() {
    pcmWriter = new PcmWriter();
    mix = new Mix();
}

bool VocalStream::onAudioDataReady(int16_t *data, int32_t numFrames) {
    if(!mIsThreadAffinitySet) {
        setThreadAffinity();
    }
    if (status != STOP) {

        int channelCount = outputStream->getChannelCount();

        if (status == PAUSE) {
            for (int j = 0; j < numFrames * channelCount; ++j) {
                data[j] = 0;
            }
            return true;
        }

        if (callbackCountToDrain > 0) {
            callbackCountToDrain--;
            if (observer && callbackCountToDrain == 0) {
                observer->onReady();
            }
            return true;
        }

        ResultWithValue<int32_t> result = inputStream->read(inputBuffer, numFrames, 0);

        if (!result) return false;

        if (status != PLAYING) {
            for (int j = 0; j < numFrames * channelCount; ++j) {
                data[j] = 0;
            }
            return true;
        }

        int32_t waitCount = inputStream->getXRunCount().value();
        int32_t frameRead = result.value();
        while (waitCount-- > 0 && frameRead < numFrames) {
            frameRead = inputStream->read(inputBuffer, numFrames, 0).value();
            //LOGI("record num %d,player num %d", result.value(), numFrames);
        }

        double time = ((double) numFrames / (double) outProperty.sampleRate) * 1000;
        currentMs += (int64_t) time;
        if (frameRead > 0) {
            pcmWriter->write(inputBuffer, inputStream->getChannelCount() * result.value());

            int size = inputStream->getChannelCount() * result.value() * sizeof(int16_t);
            if (volume != 1 || filter != 0) {
                audioFilter->process(inputBuffer, size, numFrames);
            }
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
                    data[j] = 0;
                }
            }
            if (isEcho) {
                data[j] = mix->mixData(data[j], inputBuffer[j]);
            }
        }

        for (auto &source : sources) {
            if (source->isEmpty()) {
                if (observer) {
                    observer->onCompleted();
                }
                return false;
            }
            source->requestData();
        }

        return true;
    }
    return false;
}

void VocalStream::init(AudioStream *inputStream, AudioStream *outputStream) {
    this->inputStream = inputStream;
    this->outputStream = outputStream;
    inputBuffer = new int16_t[outputStream->getBufferCapacityInFrames() * outputStream->getChannelCount()];
}


void VocalStream::prepare(const char *musicPath, const char *guidePath, const char *vocalPath) {
    status = INIT;
    callbackCountToDrain = COUNT_CALLBACK_PREPARE;
    AudioProperty outProperty;
    outProperty.sampleRate = outputStream->getSampleRate();
    outProperty.channelCount = outputStream->getChannelCount();
    this->outProperty = outProperty;

    pcmWriter->open(vocalPath, AudioProperty(), outProperty);
    sources.push_back(new StreamDataSource(outProperty, musicPath));
    //sources.push_back(new StreamDataSource(outProperty, guidePath));
    audioFilter = new AudioFilter(outProperty);
    for (auto &source : sources) {
        source->prepare();
    }
}

void VocalStream::start() {
    status = PLAYING;
}


void VocalStream::resume() {
    status = PLAYING;
}

void VocalStream::pause() {
    status = PAUSE;
}

void VocalStream::stop() {
    status = STOP;
    pcmWriter->close();
}


void VocalStream::setEcho(bool isEcho) {
    this->isEcho = isEcho;
}

void VocalStream::seek(int64_t millis) {
    for (auto &source : sources) {
        source->seek(millis);
    }
    pcmWriter->seek(millis);
    currentMs = millis;
    LOGI("current millis %d", currentMs);
}


void VocalStream::setFilter(int type) {
    audioFilter->setFilter(type);
    this->filter = type;
}

void VocalStream::setVolume(float volume, int track) {
    if (track == 0) {
        audioFilter->setVolume(volume);
        this->volume = volume;
    } else {
        sources[0]->setVolume(volume);
    }
}

void VocalStream::setPitch(int pitch) {
    sources[0]->setPitch(pitch);
}

void VocalStream::onDataReady() {

}

int64_t VocalStream::getTotalMs() {
    return sources[0]->getTotalMs();
}

int64_t VocalStream::getCurrentMs() {
    return currentMs;
}

void VocalStream::setThreadAffinity() {
    pid_t current_thread_id = gettid();
    cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);

    // If the callback cpu ids aren't specified then bind to the current cpu
    if (mCpuIds.empty()) {
        int current_cpu_id = sched_getcpu();
        LOGV("Current CPU ID is %d", current_cpu_id);
        CPU_SET(current_cpu_id, &cpu_set);
    } else {

        for (size_t i = 0; i < mCpuIds.size(); i++) {
            int cpu_id = mCpuIds.at(i);
            LOGV("CPU ID %d added to cores set", cpu_id);
            CPU_SET(cpu_id, &cpu_set);
        }
    }

    int result = sched_setaffinity(current_thread_id, sizeof(cpu_set_t), &cpu_set);
    if (result == 0) {
        LOGV("Thread affinity set");
    } else {
        LOGW("Error setting thread affinity. Error no: %d", result);
    }

    mIsThreadAffinitySet = true;
}

VocalStream::~VocalStream() {
    delete pcmWriter;
    pcmWriter = nullptr;
    delete mix;
    mix = nullptr;
    if (audioFilter) {
        delete audioFilter;
        audioFilter = nullptr;
    }
}
