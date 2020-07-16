//
// Created by Lvlingling on 2020/6/25.
//

#ifndef SUPERPLAYER_EGL_CORE_H
#define SUPERPLAYER_EGL_CORE_H


#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <android/native_window.h>


class EglCore {

public:

    EglCore() = default;

    EglCore(const EglCore &) = delete;

    EglCore(EglCore &&) = delete;

    bool init(EGLContext share_context = nullptr);

    void makeCurrent(EGLSurface surface);

    void swapBuffer(EGLSurface surface);

    EGLSurface createEGLSurface(ANativeWindow *window);

    void destroyEGLSurface(EGLSurface surface);

    void destroy();

    EGLContext getContext();

    EGLConfig getConfig();

private:

    EGLConfig getEGLConfig();

    EGLDisplay _display{nullptr};
    EGLConfig _config{nullptr};
    EGLContext _context{nullptr};


};


#endif //SUPERPLAYER_EGL_CORE_H
