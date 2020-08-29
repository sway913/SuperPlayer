//
// Created by ldr on 2020/5/27.
//

#include "recorder_engine.h"

RecorderEngine::RecorderEngine(int sampleRate) : AudioEngine(sampleRate) {}

DataCallbackResult RecorderEngine::onAudioReady(AudioStream *stream, void *data, int32_t numFrames) {
    if (!mIsThreadAffinitySet) setThreadAffinity();
    memset(data, 0, numFrames * out_stream->getBytesPerFrame());
    if (is_pause) {
        return DataCallbackResult::Continue;
    }
    if (stopped) {
        return DataCallbackResult::Stop;
    }
    RecordState state = oboe_recorder->getState(record_buffer.get(), numFrames);
    if (state == RecordState::preparing) {
        return DataCallbackResult::Continue;
    } else if (state == RecordState::success) {
        disposeReadyState(1);
        is_pause = true;
    } else if (state == RecordState::fail) {
        observer->onError();
        oboe_recorder->stop();
        return DataCallbackResult::Stop;
    } else {
        int32_t framesRead = oboe_recorder->read(record_buffer.get(), numFrames);
        if (vocal_filter) {
            vocal_filter->process(record_buffer.get(), framesRead);
        }
        if (mix_source) {
            ResultWithValue<FrameTimestamp> tv = stream->getTimestamp(CLOCK_MONOTONIC);
            if(start_time == 0) {
                start_time = tv.value().timestamp;
            }
            real_time = (tv.value().timestamp - start_time) / 1000 / 1000;
            mix_source->getMixData((short *) data, numFrames, isEcho ? record_buffer.get() : nullptr, isEcho ? framesRead : 0);
        }
        observer->onProduceData(record_buffer.get(), framesRead);
        if (getCurrentMs() >= getTotalMs()) {
            observer->onCompleted();
        }
    }
    return DataCallbackResult::Continue;
}

void RecorderEngine::prepare(SourceFactory *factory) {
    source_factory = factory;
    mix_source = new MixSource(sample_rate);
    vocal_filter = new FilterPackage();
    vocal_filter->init(Vocal, sample_rate, 1);
    acc_filter = new FilterPackage();
    acc_filter->init(Acc, sample_rate);
    source = source_factory->createStreamSource();
    for (auto &s : source) {
        s->setObserver(std::bind(&RecorderEngine::onSourceReady, this, _1, _2));
        if (s->getIndex() == 2) {
            s->setFilter(acc_filter);
        }
        s->start();
        mix_source->addSource(s);
    }
    mix_source->start();
    oboe_recorder = new OboeRecorder();
    oboe_recorder->start(sample_rate);
    openOutputStream();
    int size = out_stream->getBufferCapacityInFrames() * out_stream->getChannelCount();
    record_buffer = make_unique<short[]>(size);

    stopped = false;
    is_pause = false;
}

void RecorderEngine::start() {
    if (mix_source) {
        mix_source->resume();
    }
    is_pause = false;
}

void RecorderEngine::onSourceReady(long ms, int index) {
    if (index == 2) {
        this->total_ms = ms;
        disposeReadyState(2);
    }
}

int RecorderEngine::getMaxOutNumFrames() {
    if (!out_stream) {
        return 0;
    }
    if (out_stream->getAudioApi() == AudioApi::AAudio) {
        return out_stream->getFramesPerCallback() > 0 ? out_stream->getFramesPerCallback() : out_stream->getFramesPerBurst();
    } else {
        return out_stream->getFramesPerCallback();
    }
}

void RecorderEngine::setEcho(bool echo) {
    this->isEcho = echo;
}

void RecorderEngine::stop() {
    stopped = true;
    if (out_stream) {
        out_stream->requestStop();
    }
    if (oboe_recorder) {
        oboe_recorder->stop();
        DELETEOBJ(oboe_recorder)
    }
    if (mix_source) {
        mix_source->stop();
    }
    DELETEOBJ(mix_source)

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
    DELETEOBJ(source_factory)

    record_buffer = nullptr;
    recorder_ready = false;
    source_ready = false;
}

void RecorderEngine::disposeReadyState(int index) {
    LOGI("record ready state index %d", index);
    std::lock_guard<std::mutex> lock(mutex);
    if (index == 1) {
        recorder_ready = true;
    }
    if (index == 2) {
        source_ready = true;
    }
    if (recorder_ready && source_ready) {
        if (observer) {
            observer->onReady();
        }
    }
}

void RecorderEngine::setThreadAffinity() {
    pid_t current_thread_id = gettid();
    cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);

    if (mCpuIds.empty()) {
        int current_cpu_id = sched_getcpu();
        LOGV("Current CPU ID is %d", current_cpu_id);
        CPU_SET(current_cpu_id, &cpu_set);
    } else {
        for (int cpu_id : mCpuIds) {
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