//
// Created by Lvlingling on 2020/6/25.
//

#include "egl_core.h"
#include "../common/android_log.h"

bool EglCore::init(EGLContext share_context) {
    if (_display != EGL_NO_DISPLAY) {
        return false;
    }
    if (!share_context) {
        share_context = EGL_NO_DISPLAY;
    }

    _display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (EGL_NO_DISPLAY == _display) {
        LOGI("egl get no display");
        return false;
    }

    GLint majorVersion, minorVersion;

    if (!eglInitialize(_display, &majorVersion, &minorVersion)) {
        LOGI("egl initialize fail");
        return false;
    }

    EGLConfig config = getEGLConfig();

    if (config) {
        EGLint context_attrs[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2,
                EGL_NONE
        };

        EGLContext context = eglCreateContext(_display, config, share_context, context_attrs);
        if (eglGetError() == EGL_SUCCESS) {
            _config = config;
            _context = context;
        }

        GLint value;
        eglQueryContext(_display, _context, EGL_CONTEXT_CLIENT_VERSION, &value);
    }
    return EGL_NO_CONTEXT != _context;
}

EGLSurface EglCore::createEGLSurface(ANativeWindow *window) {
    EGLint width, height;
    EGLSurface surface = eglCreateWindowSurface(_display, _config, window, nullptr);
    if (EGL_NO_SURFACE == surface) {
        LOGI("egl create surface error");
        return nullptr;
    }

    if (!eglQuerySurface(_display, surface, EGL_WIDTH, &width) ||
        !eglQuerySurface(_display, surface, EGL_HEIGHT, &height)) {
        LOGI("egl query surface fail");
        return nullptr;
    }
    return surface;
}

void EglCore::destroyEGLSurface(EGLSurface surface) {
    eglDestroySurface(_display, surface);
}

EGLConfig EglCore::getEGLConfig() {
    EGLint config_attrs[] = {
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
        return nullptr;
    }
    return eglConfig;
}


void EglCore::makeCurrent(EGLSurface surface) {
    if (!eglMakeCurrent(_display, surface, surface, _context)) {
        LOGI("egl make current error");
    }
}


void EglCore::swapBuffer(EGLSurface surface) {
    if (!eglSwapBuffers(_display, surface)) {
        LOGI("egl swap error");
    }
}

EGLContext EglCore::getContext() {
    return _context;
}

EGLConfig EglCore::getConfig() {
    return _config;
}


void EglCore::destroy() {
    eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(_display, _context);
    eglTerminate(_display);
    _display = EGL_NO_DISPLAY;
    _context = EGL_NO_CONTEXT;
}
