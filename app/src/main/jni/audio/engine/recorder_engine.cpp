//
// Created by ldr on 2020/5/27.
//

#include "recorder_engine.h"

RecorderEngine::RecorderEngine(int sampleRate) : AudioEngine(sampleRate) {}

DataCallbackResult RecorderEngine::onAudioReady(AudioStream *stream, void *data, int32_t numFrames) {
    if (!mIsThreadAffinitySet) setThreadAffinity();
    memset(data, 0, numFrames * out_stream->getBytesPerFrame());
    RecordState state = oboe_recorder->getState(record_buffer.get(), numFrames);
    if (state == RecordState::preparing) {
        return DataCallbackResult::Continue;
    } else if (state == RecordState::success) {
        disposeReadyState(1);
    } else if (state == RecordState::fail) {
        observer->onError();
        oboe_recorder->stop();
        return DataCallbackResult::Stop;
    } else {
        int32_t framesRead = oboe_recorder->read(record_buffer.get(), numFrames);
        if (mix_source) {
            mix_source->getMixData((short *) data, numFrames, record_buffer.get(), framesRead);
        }
        observer->onProduceData(record_buffer.get(), framesRead);
    }
    return DataCallbackResult::Continue;
}

void RecorderEngine::prepare(SourceFactory *factory) {
    source_factory = factory;
    mix_source = new MixSource();
    source = source_factory->createStreamSource();
    for (auto &s : source) {
        s->setObserver(std::bind(&RecorderEngine::onSourceReady, this, _1, _2));
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
    record_valid = false;
    mix_source->pause();
}

void RecorderEngine::start() {
    record_valid = true;
    mix_source->resume();
}

void RecorderEngine::onSourceReady(long ms, int index) {
    if (index == 1) {
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

void RecorderEngine::stop() {
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
    }
    if (oboe_recorder) {
        oboe_recorder->stop();
        DELETEOBJ(oboe_recorder)
    }
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