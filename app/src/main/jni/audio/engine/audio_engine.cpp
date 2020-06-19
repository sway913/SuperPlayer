//
// Created by ldr on 2020/5/27.
//

#include "audio_engine.h"
#include "recorder_engine.h"
#include "player_engine.h"

std::unique_ptr<AudioEngine> AudioEngine::getEngine(bool is_recorder, int sample_rate) {
    if (is_recorder) {
        return std::make_unique<RecorderEngine>(sample_rate);
    } else {
        return std::make_unique<PlayerEngine>(sample_rate);
    }
}

AudioEngine::AudioEngine(int sample_rate) : sample_rate(sample_rate) {}


DataCallbackResult AudioEngine::onAudioReady(AudioStream *stream, void *data, int32_t numFrames) {
    return DataCallbackResult::Continue;
}

bool AudioEngine::openOutputStream() {
    AudioStreamBuilder outBuilder;
    (&outBuilder)->setCallback(this)
            ->setAudioApi(AudioApi::Unspecified)
            ->setSampleRate(sample_rate)
            ->setFormat(AudioFormat::I16)
            ->setChannelCount(oboe::ChannelCount::Stereo)
            ->setPerformanceMode(PerformanceMode::LowLatency)
            ->setSharingMode(SharingMode::Exclusive)
            ->setDirection(Direction::Output);
    Result result = outBuilder.openManagedStream(out_stream);
    out_stream->setBufferSizeInFrames(out_stream->getFramesPerBurst() * 2);
    if (result == Result::OK) {
        result = out_stream->requestStart();
    }
    return result == Result::OK;
}


void AudioEngine::onErrorAfterClose(AudioStream *oboeStream, Result error) {

}

void AudioEngine::onErrorBeforeClose(AudioStream *oboeStream, Result error) {
    if (observer) {
        observer->onError();
    }
}

void AudioEngine::resume() {
    for (auto &s : source) {
      s->resume();
    }
    if (mix_source) {
        mix_source->resume();
    }
    is_pause = false;
}

void AudioEngine::pause() {
    for (auto &s : source) {
        s->pause();
    }
    if (mix_source) {
        mix_source->pause();
    }
    is_pause = true;
}

void AudioEngine::setObserver(Observer *obs) {
    this->observer = obs;
}

void AudioEngine::setEcho(bool isEcho) {

}

void AudioEngine::setVolume(float volume, int track) {
    if (vocal_filter) {
        if (track == 1) {
            vocal_filter->setVolume(volume);
        } else {
            acc_filter->setVolume(volume);
        }
    }
}

void AudioEngine::setPitch(float pitch) {
    if (acc_filter) {
        acc_filter->setPitch(pitch);
    }
}

void AudioEngine::setFilter(int type) {
    if (vocal_filter) {
        vocal_filter->setEffect(type);
    }
}

void AudioEngine::setCustomFilter(float *arr) {
    if(vocal_filter){

    }
}

void AudioEngine::seek(int64_t millis) {
    for (auto &s : source) {
        s->seek(millis);
    }
    if (mix_source) {
        mix_source->seek(millis);
    }
}

int64_t AudioEngine::getCurrentMs() {
    if (mix_source) {
        return mix_source->getCurrentMs();
    } else {
        return 0;
    }
}


int64_t AudioEngine::getTotalMs() {
    return total_ms;
}

AudioEngine::~AudioEngine() = default;
