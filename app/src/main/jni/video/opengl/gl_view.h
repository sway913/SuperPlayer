//
// Created by Lvlingling on 2020/6/25.
//

#ifndef SUPERPLAYER_GL_VIEW_H
#define SUPERPLAYER_GL_VIEW_H

#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <jni.h>
#include "egl_core.h"
#include <future>
#include "../../common/common_tools.h"
#include "../../common/lock_free_queue.h"
#include "../render/irender.h"
#include <queue>
constexpr int MSG_DRAW = 2;
constexpr int QUEUE_SIZE = 4;


class GlView {

public:

    GlView(int mode);

    void createSurface(JNIEnv *env, jobject surface, int width, int height);

    void destroySurface();

    void requestRender();

    void setRender(IRender *render_);

    EglCore *getEglCore();

    virtual ~GlView();

private:

    void guardedRun();

    ANativeWindow *window{nullptr};
    EGLSurface eglSurface{nullptr};
    int width{0};
    int height{0};
    EglCore *eglCore{nullptr};
    std::future<void> future;
    IRender *render{nullptr};
    std::mutex mutex_{};
    std::condition_variable cond{};
    LockFreeQueue<int, QUEUE_SIZE> *msg_queue{nullptr};

    int mode{0};
    bool is_exit{false};

};


#endif //SUPERPLAYER_GL_VIEW_H
