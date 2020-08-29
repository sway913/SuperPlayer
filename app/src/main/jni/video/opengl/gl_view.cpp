//
// Created by Lvlingling on 2020/6/25.
//

#include <unistd.h>
#include "gl_view.h"
#include <sstream>

GlView::GlView(int mode_) {
    this->mode = mode_;
}

void GlView::createSurface(JNIEnv *env, jobject surface, int w, int h) {
    if (mode == 1) {
        if (render) {
            render->onSurfaceCreate(surface, w, h);
        }
        return;
    }
    msg_queue = new LockFreeQueue<int, QUEUE_SIZE>();
    window = ANativeWindow_fromSurface(env, surface);
    this->width = w;
    this->height = h;
    is_exit = false;
    future = std::async(std::launch::async, &GlView::guardedRun, this);
    LOGI("GlView mode %d", mode);
}

void GlView::guardedRun() {
    std::stringstream sin;
    sin << std::this_thread::get_id();
    LOGI("Gl thread id %s", sin.str().c_str());

    if (window != nullptr) {
        eglCore = new EglCore();
        eglCore->init();
        eglSurface = eglCore->createEGLSurface(window);
        eglCore->makeCurrent(eglSurface);
    }
    if (render && window) {
        render->onSurfaceCreate(nullptr, width, height);
        render->onDraw();
    }

    while (!is_exit) {
        std::unique_lock<std::mutex> lock(mutex_);
        cond.wait(lock, [this] {
            if (!is_exit) {
                return true;
            }
            return msg_queue->size() != 0;
        });
        if (is_exit) {
            break;
        }
        int msg;
        msg_queue->pop(msg);
        if (msg == MSG_DRAW) {
            if (eglCore) {
                eglCore->makeCurrent(eglSurface);
            }
            if (render) {
                render->onDraw();
            }
            if (eglCore) {
                eglCore->swapBuffer(eglSurface);
            }
        }
    }
    if (eglCore) {
        eglCore->makeCurrent(eglSurface);
    }
    if (render) {
        render->onSurfaceDestroy();
    }
    if (eglCore) {
        eglCore->destroyEGLSurface(eglSurface);
        eglCore->destroy();
        DELETEOBJ(eglCore)
    }
    LOGI("GL thread quit");
}

void GlView::destroySurface() {
    if (mode == 1) {
        if (render) {
            render->onSurfaceDestroy();
        }
        return;
    }
    LOGI("Gl Surface Destroying");
    is_exit = true;
    cond.notify_all();
    JOIN(future);
    if (window) {
        ANativeWindow_release(window);
    }
    LOGI("Gl Surface Destroyed");
}

void GlView::setRender(IRender *render_) {
    this->render = render_;
}

EglCore *GlView::getEglCore() {
    return eglCore;
}

void GlView::requestRender() {
    if (mode == 1) {
        return;
    }
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

GlView::~GlView() {
    LOGI("~ GlView");
}