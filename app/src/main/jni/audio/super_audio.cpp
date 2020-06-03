#include "super_audio.h"

SuperAudio::SuperAudio(JNIEnv *jniEnv, jobject obj) {
    this->jniEnv = jniEnv;
    this->obj = jniEnv->NewGlobalRef(obj);
    jniEnv->GetJavaVM(&javaVm);
}

void SuperAudio::prepare() {
    thread_future = std::async(std::launch::async, &SuperAudio::callLooper, this);
    audio_engine = std::make_unique<PlayerEngine>(48000);
    audio_engine->setObserver(this);
    audio_engine->prepare(nullptr);
}

void SuperAudio::start() {
    if (audio_engine) {
        audio_engine->start();
    }
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
    if (audio_engine) {
        audio_engine->stop();
    }
    setMessage(MsgState::MsgStop);
    JOIN(thread_future);
    while (!msg_queue.empty()) msg_queue.pop();
    audio_engine = nullptr;
}

void SuperAudio::seek(int64_t millis) {
    if (audio_engine) {
        audio_engine->seek(millis);
    }
}

void SuperAudio::setFilter(int type) {
    if (audio_engine) {
        audio_engine->setFilter(type);
    }
}

void SuperAudio::setEcho(bool isEcho) {
    if (audio_engine) {
        audio_engine->setEcho(isEcho);
    }
}

void SuperAudio::setPitch(int pitch) {
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
            default:
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

void SuperAudio::onEngineReady() {
    setMessage(MsgState::MsgPrepared);
}


void SuperAudio::onCompleted() {
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
