//
// Created by Lvlingling on 2020/8/11.
//

#ifndef SUPERPLAYER_VIDEO_SOURCE_H
#define SUPERPLAYER_VIDEO_SOURCE_H


#include <jni.h>
#include <functional>
#include "source.h"
#include <mutex>

class VideoSource : public Source {

public:

    VideoSource(JNIEnv *env, std::function<void(void)> cb);

    void prepare(JNIEnv *env, const char *path);

    void open(int w, int h, jobject surface) override;

    void close() override;

    VideoFrame *produceFrame() override;

    void stop();

    virtual ~VideoSource();

private:

    JNIEnv *env{nullptr};
    JavaVM *javaVm{nullptr};
    jobject j_decoder{nullptr};
    jmethodID close_id{nullptr};
    jmethodID init_id{nullptr};
    const char *path{nullptr};
    std::mutex mutex_;


};


#endif //SUPERPLAYER_VIDEO_SOURCE_H
