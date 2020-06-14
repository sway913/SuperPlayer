//
// Created by ldr on 2020/5/27.
//

#include "player_engine.h"

PlayerEngine::PlayerEngine(int sample_rate) : AudioEngine(sample_rate) {
}

DataCallbackResult PlayerEngine::onAudioReady(AudioStream *stream, void *data, int32_t numFrames) {
    memset(data, 0, numFrames * out_stream->getBytesPerFrame());
    if (is_pause) {
        return DataCallbackResult::Continue;
    }
    if (mix_source && !stopped) {
        mix_source->getMixData((short *) data, numFrames, nullptr, 0);
        if (getCurrentMs() > getTotalMs()) {
            observer->onCompleted();
        }
        return DataCallbackResult::Continue;
    } else {
        return DataCallbackResult::Stop;
    }
}

void PlayerEngine::prepare(SourceFactory *factory) {
    source_factory = factory;
}

void PlayerEngine::start() {
    mix_source = new MixSource(sample_rate);
    source = source_factory->createPcmSource();
    for (auto &s : source) {
        s->setObserver(std::bind(&PlayerEngine::onSourceReady, this, _1, _2));
        s->start();
        mix_source->addSource(s);
    }
    mix_source->start();
    stopped = false;
    is_pause = false;
    openOutputStream();
}

void PlayerEngine::onSourceReady(long ms, int index) {
    if (index == 1) {
        this->total_ms = ms;
    }
}

void PlayerEngine::stop() {
    stopped = true;
    if (out_stream) {
        out_stream->requestStop();
        out_stream = nullptr;
    }
    mix_source->stop();
    DELETEOBJ(mix_source)
    DELETEOBJ(source_factory)
    for (auto &s : source) {
        s->stop();
        DELETEOBJ(s)
    }


}