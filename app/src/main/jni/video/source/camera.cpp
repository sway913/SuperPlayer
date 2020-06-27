//
// Created by Lvlingling on 2020/6/25.
//

#include "camera.h"

Camera::Camera(JNIEnv *env) : env(env) {
    env->GetJavaVM(&javaVm);
    jclass clazz = env->FindClass("com/smzh/superplayer/video/Camera1");
    jmethodID general_id = env->GetMethodID(clazz, "<init>", "()V");
    jobject camera = env->NewObject(clazz, general_id);
    jCamera = env->NewGlobalRef(camera);

    open_id = env->GetMethodID(clazz, "open", "()V");
    close_id = env->GetMethodID(clazz, "close", "()V");
    update_id = env->GetMethodID(clazz, "updateImage", "()V");
    switch_id = env->GetMethodID(clazz, "switchCamera", "()V");
    start_id = env->GetMethodID(clazz, "startPreview", "(I)V");

    env->DeleteLocalRef(clazz);

    sourceFilter = new SourceFilter();
}

void Camera::open(int w, int h) {
    this->out_width = w;
    this->out_height = h;
    int needAttach = javaVm->GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_EDETACHED;
    if (needAttach) {
        javaVm->AttachCurrentThread(&env, nullptr);
    }
    env->CallVoidMethod(jCamera, open_id);
    if (needAttach) {
        javaVm->DetachCurrentThread();
    }
}

void Camera::updateImage() {
    if (jCamera == nullptr) {
        return;
    }
    int needAttach = javaVm->GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_EDETACHED;
    if (needAttach) {
        javaVm->AttachCurrentThread(&env, nullptr);
    }
    env->CallVoidMethod(jCamera, update_id);

    switch (cmd) {
        case Close:
            env->CallVoidMethod(jCamera, close_id);
            env->DeleteGlobalRef(jCamera);
            jCamera = nullptr;
            sourceFilter->destroy();
            DELETEOBJ(sourceFilter)
            break;
        case Switch_:
            env->CallVoidMethod(jCamera, switch_id);
            break;
        default:
            break;
    }

    if (needAttach) {
        javaVm->DetachCurrentThread();
    }
}


void Camera::close() {
    cmd = Close;
}


void Camera::switchCamera() {
    cmd = Switch_;
}

GLuint Camera::produceFrame() {
    if (!start_preview) {
        textureId = OpenGLUtils::createOEXTexture();
        LOGI("camera oex id is %d", textureId);
        int needAttach = javaVm->GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_EDETACHED;
        if (needAttach) {
            javaVm->AttachCurrentThread(&env, nullptr);
        }
        env->CallVoidMethod(jCamera, start_id, textureId);
        if (needAttach) {
            javaVm->DetachCurrentThread();
        }
        start_preview = true;
    }
    updateImage();
    return sourceFilter ? sourceFilter->draw(textureId, out_width, out_height) : textureId;
}

Camera::~Camera() {
    env = nullptr;
    javaVm = nullptr;
}
