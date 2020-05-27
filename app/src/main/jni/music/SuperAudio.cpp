//
// Created by Lvlingling on 2019-11-06.
//

#include "SuperAudio.h"
#include "common/AndroidLog.h"

SuperAudio::SuperAudio(JNIEnv *jniEnv, jobject obj, bool isRecordMode) {
    this->jniEnv = jniEnv;
    this->obj = jniEnv->NewGlobalRef(obj);
    jniEnv->GetJavaVM(&javaVm);

    if (isRecordMode) {
        baseEngine = std::unique_ptr<RecorderEngine>(new RecorderEngine());
    } else {
        baseEngine = std::unique_ptr<PlayerEngine>(new PlayerEngine());
    }
}

void SuperAudio::prepare(const char *musicPath, const char *guidePath, const char *vocalPath, int sample) {
    callResult = std::async(std::launch::async, &SuperAudio::callLooper, this);
    baseEngine->setObserver(this);
    baseEngine->prepare(sample, musicPath, guidePath, vocalPath);
}

void SuperAudio::start() {
    baseEngine->start();
}

void SuperAudio::pause() {
    baseEngine->pause();
}

void SuperAudio::resume() {
    baseEngine->resume();
}

void SuperAudio::stop() {
    baseEngine->stop();
    messages.push(0);
    cond.notify_all();
    std::future_status status;
    do {
        status = callResult.wait_for(std::chrono::milliseconds(100));
        if (status == std::future_status::deferred) {
            std::cout << "deffered" << std::endl;
        } else if (status == std::future_status::timeout) {
            std::cout << ".";
        } else {
            std::cout << "ready" << std::endl;
        }
    } while (status != std::future_status::ready);
}

void SuperAudio::seek(int64_t millis) {
    baseEngine->seek(millis);
}

void SuperAudio::setFilter(int type) {
    baseEngine->setFilter(type);
}

void SuperAudio::setEcho(bool isEcho) {
    baseEngine->setEcho(isEcho);
}

void SuperAudio::setPitch(int pitch) {
    baseEngine->setPitch(pitch);
}

void SuperAudio::setVolume(float volume, int track) {
    baseEngine->setVolume(volume, track);
}

void SuperAudio::callLooper() {
    int status = javaVm->GetEnv((void **) &jniEnv, JNI_VERSION_1_6);
    if (status == JNI_EDETACHED || jniEnv == nullptr) {
        javaVm->AttachCurrentThread(&jniEnv, nullptr);
    }

    jclass clazz = jniEnv->GetObjectClass(obj);

    jmethodID prepareMethodId = jniEnv->GetMethodID(clazz, "onDataReady", "()V");

    jmethodID completedMethodId = jniEnv->GetMethodID(clazz, "onCompleted", "()V");

    jmethodID errorMethodId = jniEnv->GetMethodID(clazz, "onError", "()V");

    if (clazz) {
        jniEnv->DeleteLocalRef(clazz);
    }

    bool isPlaying = true;
    while (isPlaying) {
        std::unique_lock<std::mutex> lock(mutex);
        cond.wait(lock, [this] {
            return !messages.empty();
        });

        int msg = messages.front();
        messages.pop();

        switch (msg) {
            case 1:
                if (clazz) {
                    if (prepareMethodId) {
                        jniEnv->CallVoidMethod(obj, prepareMethodId);
                        LOGI("call prepared");
                    }
                }
                break;

            case 2:
                if (clazz) {
                    if (completedMethodId) {
                        jniEnv->CallVoidMethod(obj, completedMethodId);
                        LOGI("call complete");
                    }
                }
                break;
            case 3:
                if (clazz) {
                    if (errorMethodId) {
                        jniEnv->CallVoidMethod(obj, errorMethodId);
                        LOGI("call complete");
                    }
                }
                break;
            case 0:
                isPlaying = false;
                LOGI("looper exit");
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
    if (baseEngine == nullptr) {
        return 0;
    }
    return baseEngine->getTotalMs();
}

int64_t SuperAudio::getCurrentMs() {
    if (baseEngine == nullptr) {
        return 0;
    }
    return baseEngine->getCurrentMs();
}

void SuperAudio::onDataReady() {
    messages.push(1);
    cond.notify_all();
    LOGI("dataPrepared");
}


void SuperAudio::onCompleted() {
    messages.push(2);
    cond.notify_all();
}

void SuperAudio::onError() {
    messages.push(3);
    cond.notify_all();
}

SuperAudio::~SuperAudio() {
    javaVm = nullptr;
    jniEnv = nullptr;
    obj = nullptr;
};
