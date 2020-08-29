//
// Created by Lvlingling on 2020/6/25.
//

#ifndef SUPERPLAYER_EGL_CORE_H
#define SUPERPLAYER_EGL_CORE_H


#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <android/native_window.h>

typedef EGLBoolean (EGLAPIENTRYP PFNEGLPRESENTATIONTIMEANDROIDPROC)(EGLDisplay display, EGLSurface surface, khronos_stime_nanoseconds_t time);

class EglCore {

public:

    EglCore();

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

    int setPresentationTime(EGLSurface surface, khronos_stime_nanoseconds_t nsecs);

private:

    EGLConfig getEGLConfig();

    EGLDisplay _display{nullptr};
    EGLConfig _config{nullptr};
    EGLContext _context{nullptr};

    PFNEGLPRESENTATIONTIMEANDROIDPROC pfneglPresentationTimeANDROID;

};


#endif //SUPERPLAYER_EGL_CORE_H
