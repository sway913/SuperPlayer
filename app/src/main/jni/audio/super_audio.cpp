#include "super_audio.h"

//#define TEST_OBOE

SuperAudio::SuperAudio(JNIEnv *jniEnv, jobject obj) {
    this->jniEnv = jniEnv;
    this->obj = jniEnv->NewGlobalRef(obj);
    jniEnv->GetJavaVM(&javaVm);
}

void SuperAudio::prepare(std::shared_ptr<AudioParam> &param) {
    this->audio_param = param;
    thread_future = std::async(std::launch::async, &SuperAudio::callLooper, this);
#ifndef TEST_OBOE
    audio_engine = AudioEngine::getEngine(param->isRecorder(), param->getOutSample());
    audio_engine->setObserver(this);
    auto *factory = new SourceFactory(param->getVocalPath(),
                                      param->getAccPath(),
                                      param->getGuidePath(),
                                      param->getDecodePath(),
                                      param->getOutSample());
    audio_engine->prepare(factory);
    if (param->isRecorder()) {
        pcmWriter = new PcmWriter2();
        pcmWriter->start(param->getVocalPath(), param->getOutSample());
    } else {
        file_decoder = new FileDecoder(param->getAccPath(), param->getDecodePath(), 44100);
        file_decoder->start(std::bind(&SuperAudio::onReady, this));
    }
#else
    testOboe = new TestOboe();
    onReady();
#endif
}

void SuperAudio::start() {
#ifndef TEST_OBOE
    if (audio_engine) {
        audio_engine->start();
    }
#else
    if (testOboe) {
        testOboe->start();
    }
#endif
}

void SuperAudio::resume() {
    if (audio_engine) {
        audio_engine->resume();
    }
}

void SuperAudio::pause() {
    if (audio_engine) {
        audio_engine->pause();
    }
}

void SuperAudio::stop() {
#ifndef TEST_OBOE
    if (audio_engine) {
        audio_engine->stop();
    }
#else
    if (testOboe) {
        testOboe->stop();
    }
#endif
    setMessage(MsgState::MsgStop);
    JOIN(thread_future);
    while (!msg_queue.empty()) msg_queue.pop();
    if (file_decoder) {
        file_decoder->stop();
        DELETEOBJ(file_decoder);
    }
    if (pcmWriter) {
        pcmWriter->stop();
        DELETEOBJ(pcmWriter)
    }
    audio_engine = nullptr;
    audio_param = nullptr;
}

void SuperAudio::seek(int64_t millis) {
    if (audio_engine) {
        audio_engine->seek(millis);
    }
    if (pcmWriter) {
        pcmWriter->seek(millis);
    }
}

void SuperAudio::setFilter(int type) {
    if (audio_engine) {
        audio_engine->setFilter(type);
    }
}

void SuperAudio::setCustomFilter(float *arr) {
    if (audio_engine) {
        audio_engine->setCustomFilter(arr);
    }
}

void SuperAudio::setEcho(bool isEcho) {
    if (audio_engine) {
        audio_engine->setEcho(isEcho);
    }
}

void SuperAudio::setPitch(float pitch) {
    if (audio_engine) {
        audio_engine->setPitch(pitch);
    }
}

void SuperAudio::setVolume(float volume, int track) {
    if (audio_engine) {
        audio_engine->setVolume(volume, track);
    }
}

void SuperAudio::callLooper() {
    int status = javaVm->GetEnv((void **) &jniEnv, JNI_VERSION_1_6);
    if (status == JNI_EDETACHED || jniEnv == nullptr) {
        javaVm->AttachCurrentThread(&jniEnv, nullptr);
    }

    jclass clazz = jniEnv->GetObjectClass(obj);
    jmethodID prepare_id = jniEnv->GetMethodID(clazz, "onPrepared", "()V");
    jmethodID complete_id = jniEnv->GetMethodID(clazz, "onCompleted", "()V");
    jmethodID error_id = jniEnv->GetMethodID(clazz, "onError", "()V");

    if (clazz) {
        jniEnv->DeleteLocalRef(clazz);
    }

    bool isPlaying = true;
    while (isPlaying) {
        std::unique_lock<std::mutex> lock(mutex);
        cond.wait(lock, [this] {
            return !msg_queue.empty();
        });

        MsgState msg = msg_queue.front();
        msg_queue.pop();

        switch (msg) {
            case MsgState::MsgPrepared:
                if (prepare_id) {
                    jniEnv->CallVoidMethod(obj, prepare_id);
                }
                break;
            case MsgState::MsgCompleted:
                if (complete_id) {
                    jniEnv->CallVoidMethod(obj, complete_id);
                }
                break;
            case MsgState::MsgError:
                if (error_id) {
                    jniEnv->CallVoidMethod(obj, error_id);
                }
                break;
            case MsgState::MsgStop:
                isPlaying = false;
                break;
        }
    }

    if (obj) {
        jniEnv->DeleteGlobalRef(obj);
    }

    if (nullptr != javaVm) {
        if (javaVm->GetEnv((void **) &jniEnv, JNI_VERSION_1_6) == 0) {
            javaVm->DetachCurrentThread();
        }
    }
}

int64_t SuperAudio::getTotalMs() {
    if (audio_engine) {
        return audio_engine->getTotalMs();
    }
    return 0;
}

int64_t SuperAudio::getCurrentMs() {
    if (audio_engine) {
        return audio_engine->getCurrentMs();
    }
    return 0;
}

void SuperAudio::onProduceData(short *data, int count) {
    if (pcmWriter) {
        pcmWriter->write(data, count);
    }
}

void SuperAudio::onReady() {
    setMessage(MsgState::MsgPrepared);
}


void SuperAudio::onCompleted() {
    if (audio_engine) {
        audio_engine->pause();
    }
    setMessage(MsgState::MsgCompleted);
}

void SuperAudio::onError() {
    setMessage(MsgState::MsgError);
}

void SuperAudio::setMessage(SuperAudio::MsgState msg) {
    std::lock_guard<std::mutex> lock(mutex);
    msg_queue.push(msg);
    cond.notify_all();
}

SuperAudio::~SuperAudio() {
    javaVm = nullptr;
    jniEnv = nullptr;
    obj = nullptr;
}
