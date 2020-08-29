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
        ResultWithValue<FrameTimestamp> tv = stream->getTimestamp(CLOCK_MONOTONIC);
        if(start_time == 0) {
            start_time = tv.value().timestamp;
        }
        real_time = (tv.value().timestamp - start_time) / 1000 / 1000;
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
    vocal_filter = new FilterPackage();
    vocal_filter->init(Vocal, sample_rate);
    acc_filter = new FilterPackage();
    acc_filter->init(Acc, sample_rate);
    source = source_factory->createPcmSource();
    for (auto &s : source) {
        s->setObserver(std::bind(&PlayerEngine::onSourceReady, this, _1, _2));
        if (s->getIndex() == 1) {
            s->setFilter(vocal_filter);
        }
        if (s->getIndex() == 2) {
            s->setFilter(acc_filter);
        }
        s->start();
        mix_source->addSource(s);
    }
    mix_source->start();
    stopped = false;
    is_pause = false;
    openOutputStream();
    LOGI("audio play engine start");
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
    if (mix_source) {
        mix_source->stop();
    }
    DELETEOBJ(mix_source)
    DELETEOBJ(source_factory)
    for (auto &s : source) {
        s->stop();
        s->setFilter(nullptr);
        DELETEOBJ(s)
    }
    if (vocal_filter) {
        vocal_filter->destroy();
        DELETEOBJ(vocal_filter)
    }
    if (acc_filter) {
        acc_filter->destroy();
        DELETEOBJ(acc_filter)
    }

}