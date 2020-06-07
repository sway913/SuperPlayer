//
// Created by ldr on 2020/5/27.
//

#include "player_engine.h"

PlayerEngine::PlayerEngine(int sample_rate) : AudioEngine(sample_rate) {
}

DataCallbackResult PlayerEngine::onAudioReady(AudioStream *stream, void *data, int32_t numFrames) {
    if (mix_source && !stopped) {
        mix_source->getMixData((short *) data, numFrames, nullptr, 0);
        return DataCallbackResult::Continue;
    } else {
        return DataCallbackResult::Stop;
    }
}

void PlayerEngine::prepare(SourceFactory *factory) {
    source_factory = factory;
    mix_source = new MixSource();
}

void PlayerEngine::start() {
    source = source_factory->createPcmSource();
    for (auto &s : source) {
        s->setObserver(std::bind(&PlayerEngine::onSourceReady, this, _1, _2));
        s->start();
        mix_source->addSource(s);
    }
    mix_source->start();
    stopped = false;
    openOutputStream();
}

void PlayerEngine::onSourceReady(long ms, int index) {

}

void PlayerEngine::stop() {
    stopped = true;
    DELETEOBJ(source_factory)
    for (auto &s : source) {
        s->stop();
        DELETEOBJ(s)
    }
    mix_source->stop();
    DELETEOBJ(mix_source)
    if (out_stream) {
        out_stream->requestStop();
        out_stream = nullptr;
    }
}