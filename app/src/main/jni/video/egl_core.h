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

    bool init(ANativeWindow *window);

    void makeCurrent();

    void swapBuffer();

    void destroy();

private:

    EGLDisplay _display{nullptr};
    EGLSurface _surface{nullptr};
    EGLContext _context{nullptr};


};


#endif //SUPERPLAYER_EGL_CORE_H
