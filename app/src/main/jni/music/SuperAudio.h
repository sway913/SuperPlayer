//
// Created by Lvlingling on 2019-11-06.
//
#include <jni.h>
#include "engine/BaseEngine.h"
#include "engine/player/PlayerEngine.h"
#include "engine/recorder/RecorderEngine.h"
#include <queue>
#include <future>


#ifndef SUPERPLAYER_MUSICPLAYER_H
#define SUPERPLAYER_MUSICPLAYER_H

using namespace std::placeholders;

class SuperAudio : public Observer {

private:
    std::unique_ptr<BaseEngine> baseEngine;
    JNIEnv *jniEnv;
    JavaVM *javaVm;
    jobject obj;

    std::mutex mutex;
    std::condition_variable cond;
    std::queue<int> messages;
    std::future<void> callResult;

public:

    SuperAudio(JNIEnv *jniEnv, jobject obj, bool isRecordMode);

    virtual ~SuperAudio();

    void prepare(const char *musicPath, const char *guidePath, const char *vovalPath, int sample = 44100);

    void start();

    void stop();

    void resume();

    void pause();

    void setEcho(bool isEcho);

    void seek(int64_t millis);

    void setVolume(float volume, int track);

    void setFilter(int type);

    void setPitch(int pitch);

    void callLooper();

    void onDataReady() override;

    void onCompleted() override;

    void onError() override;

    int64_t getTotalMs();

    int64_t getCurrentMs();

};


#endif //SUPERPLAYER_MUSICPLAYER_H
