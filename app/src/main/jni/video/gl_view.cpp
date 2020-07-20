//
// Created by Lvlingling on 2020/6/25.
//

#include <unistd.h>
#include "gl_view.h"
#include <sstream>

void GlView::createSurface(JNIEnv *env, jobject surface, int w, int h) {
    window = ANativeWindow_fromSurface(env, surface);
    this->width = w;
    this->height = h;
    msg_queue = new LockFreeQueue<int, QUEUE_SIZE>();
    msg_queue->push(MSG_CTEATE);
    future = std::async(std::launch::async, &GlView::guardedRun, this);
}

void GlView::guardedRun() {
    bool is_exit = false;
    std::stringstream sin;
    sin << std::this_thread::get_id();
    LOGI("gl thread id %s", sin.str().c_str());
    while (!is_exit) {
        std::unique_lock<std::mutex> lock(mutex_);
        cond.wait(lock, [this] {
            return msg_queue->size() != 0;
        });
        int msg;
        msg_queue->pop(msg);
        switch (msg) {
            case MSG_CTEATE:
                eglCore = new EglCore();
                eglCore->init();
                eglSurface = eglCore->createEGLSurface(window);
                eglCore->makeCurrent(eglSurface);
                render->onSurfaceCreate(width, height);
                break;
            case MSG_DRAW:
                if (eglCore && render) {
                    eglCore->makeCurrent(eglSurface);
                    render->onDraw();
                    eglCore->swapBuffer(eglSurface);
                }
                break;
            case MSG_DESTROY:
                if (eglCore && render) {
                    render->onSurfaceDestroy();
                    eglCore->destroyEGLSurface(eglSurface);
                    eglCore->destroy();
                    DELETEOBJ(eglCore)
                }
                is_exit = true;
                break;
            default:
                break;
        }
    }
}

void GlView::destroySurface() {
    msg_queue->push(MSG_DESTROY);
    cond.notify_all();
    JOIN(future);
    if (window) {
        ANativeWindow_release(window);
    }
}

void GlView::setRender(Render *render_) {
    this->render = render_;
}

EglCore * GlView::getEglCore() {
    return eglCore;
}

void GlView::requestRender() {
    int msg;
    if (msg_queue->peek(msg)) {
        if (msg != MSG_DRAW) {
            msg_queue->push(MSG_DRAW);
        }
    } else {
        msg_queue->push(MSG_DRAW);
    }
    cond.notify_all();
}