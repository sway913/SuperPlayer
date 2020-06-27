//
// Created by Lvlingling on 2020/6/25.
//

#include "egl_core.h"
#include "../common/android_log.h"

bool EglCore::init(ANativeWindow *window) {
    GLint majorVersion;
    GLint minorVersion;
    EGLint width;
    EGLint height;

    _display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (EGL_NO_DISPLAY == _display) {
        LOGI("egl get no display");
        return false;
    }

    if (!eglInitialize(_display, &majorVersion, &minorVersion)) {
        LOGI("egl initialize fail");
        return false;
    }

    EGLint config_attrs[] = {
//            EGL_BLUE_SIZE, 8,
//            EGL_GREEN_SIZE, 8,
//            EGL_RED_SIZE, 8,
//            EGL_ALPHA_SIZE, 8,
//            EGL_DEPTH_SIZE, 0,
//            EGL_STENCIL_SIZE, 0,
//            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
//            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
//            EGL_NONE
            EGL_BUFFER_SIZE, 32,
            EGL_ALPHA_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_NONE
    };


    int num_configs = 0;
    EGLConfig eglConfig;
    if (!eglChooseConfig(_display, config_attrs, &eglConfig, 1, &num_configs)) {
        LOGI("egl choose config error");
        return -1;
    }

    _surface = eglCreateWindowSurface(_display, eglConfig, window, nullptr);
    if (EGL_NO_SURFACE == _surface) {
        LOGI("egl create surface error");
        return false;
    }

    if (!eglQuerySurface(_display, _surface, EGL_WIDTH, &width) ||
        !eglQuerySurface(_display, _surface, EGL_HEIGHT, &height)) {
        LOGI("egl query surface fail");
        return false;
    }

    EGLint context_attrs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
    };

    _context = eglCreateContext(_display, eglConfig, EGL_NO_CONTEXT, context_attrs);
    if (EGL_NO_CONTEXT != _context) {
        LOGI("egl create context error");
    }
    return EGL_NO_CONTEXT != _context;
}


void EglCore::makeCurrent() {
    if (!eglMakeCurrent(_display, _surface, _surface, _context)) {
        LOGI("egl make current error");
    }
}


void EglCore::swapBuffer() {
    if (!eglSwapBuffers(_display, _surface)) {
        LOGI("egl swap error");
    }
}


void EglCore::destroy() {
    eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(_display, _context);
    eglDestroySurface(_display, _surface);
    eglTerminate(_display);
    _display = EGL_NO_DISPLAY;
    _surface = EGL_NO_SURFACE;
    _context = EGL_NO_CONTEXT;
}
