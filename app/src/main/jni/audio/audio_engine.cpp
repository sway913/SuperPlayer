//
// Created by ldr on 2020/5/27.
//

#include "audio_engine.h"

AudioEngine::AudioEngine(int sample_rate) : sample_rate(sample_rate) {}


DataCallbackResult AudioEngine::onAudioReady(AudioStream *stream, void *data, int32_t numFrames) {
    return DataCallbackResult ::Continue;
}


void AudioEngine::onErrorAfterClose(AudioStream *oboeStream, Result error) {

}

void AudioEngine::onErrorBeforeClose(AudioStream *oboeStream, Result error) {

}

void AudioEngine::prepare(SourceFactory *factory) {

}

void AudioEngine::start() {

}

void AudioEngine::resume() {

}

void AudioEngine::pause() {

}

void AudioEngine::stop() {

}

void AudioEngine::setObserver(Observer *observer) {

}

void AudioEngine::setEcho(bool isEcho) {

}

void AudioEngine::setVolume(float volume, int track) {

}

void AudioEngine::setPitch(int pitch) {

}

void AudioEngine::setFilter(int type) {

}

void AudioEngine::seek(int64_t millis) {

}

int64_t AudioEngine::getCurrentMs() {
    return 0;
}

int64_t AudioEngine::getTotalMs() {
    return 0;
}