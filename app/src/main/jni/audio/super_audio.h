#ifndef SUPERPLAYER_MUSICPLAYER_H
#define SUPERPLAYER_MUSICPLAYER_H


#include <jni.h>
#include "engine/audio_engine.h"
#include <queue>
#include <future>
#include "../common/android_log.h"
#include "engine/player_engine.h"
#include "../common/common_tools.h"


using namespace std::placeholders;

class SuperAudio : public Observer {

    enum class MsgState {
        MsgPrepared = 1,
        MsgError = 2,
        MsgCompleted = 3,
        MsgStop = 4
    };

private:

    std::unique_ptr<AudioEngine> audio_engine{nullptr};
    JNIEnv *jniEnv{nullptr};
    JavaVM *javaVm{nullptr};
    jobject obj{nullptr};

    std::mutex mutex;
    std::condition_variable cond;
    std::queue<MsgState> msg_queue;
    std::future<void> thread_future;

    void setMessage(MsgState msg);

public:

    SuperAudio(JNIEnv *jniEnv, jobject obj);

    virtual ~SuperAudio();

    void prepare();

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

    void onEngineReady() override;

    void onCompleted() override;

    void onError() override;

    int64_t getTotalMs();

    int64_t getCurrentMs();

};


#endif //SUPERPLAYER_MUSICPLAYER_H
