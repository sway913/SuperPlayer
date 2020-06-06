//
// Created by ldr on 2020/5/27.
//

#include "player_engine.h"

PlayerEngine::PlayerEngine(int sample_rate) : AudioEngine(sample_rate) {

}

DataCallbackResult PlayerEngine::onAudioReady(AudioStream *stream, void *data, int32_t numFrames) {
    if (mix_source) {
        mix_source->getData((short *) data, numFrames);
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
        s->start();
        mix_source->addSource(s);
    }
    mix_source->start();
    openStream();
}