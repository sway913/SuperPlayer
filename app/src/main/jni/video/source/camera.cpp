//
// Created by Lvlingling on 2020/6/25.
//

#include "camera.h"
#include "../../common/matrix.h"
#include "../transform.h"

Camera::Camera(JNIEnv *env) : env(env) {
    env->GetJavaVM(&javaVm);
    jclass clazz = env->FindClass("com/smzh/superplayer/video/Camera1");
    jmethodID general_id = env->GetMethodID(clazz, "<init>", "()V");
    jobject camera = env->NewObject(clazz, general_id);
    jCamera = env->NewGlobalRef(camera);

    open_id = env->GetMethodID(clazz, "open", "()[I");
    close_id = env->GetMethodID(clazz, "close", "()V");
    update_id = env->GetMethodID(clazz, "updateImage", "()J");
    switch_id = env->GetMethodID(clazz, "switchCamera", "()[I");
    start_id = env->GetMethodID(clazz, "startPreview", "(I)V");

    env->DeleteLocalRef(clazz);
    env->DeleteLocalRef(camera);

    sourceFilter = new SourceFilter();
    parameter = new int[3];
    matrix = new float[16];
}

void Camera::open(int w, int h, jobject surface) {
    this->out_width = w;
    this->out_height = h;
    int needAttach = javaVm->GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_EDETACHED;
    if (needAttach) {
        javaVm->AttachCurrentThread(&env, nullptr);
    }
    auto p = (jintArray) env->CallObjectMethod(jCamera, open_id);
    jint *param = env->GetIntArrayElements(p, nullptr);
    memcpy(parameter, param, sizeof(int) * 3);
    env->ReleaseIntArrayElements(p, param, 0);
    if (needAttach) {
        javaVm->DetachCurrentThread();
    }

    matrixSetIdentityM(matrix);
    if (parameter[0] == 1) {
        rotateZd(matrix, 270);
    } else {
        matrixScaleM(matrix, -1, 1, 1);
        rotateZd(matrix, 270);
    }
    Transform::cropViewport(matrix, parameter[1], parameter[2], out_width, out_height);

}

long Camera::updateImage() {
    if (jCamera == nullptr) {
        return 0;
    }
    int needAttach = javaVm->GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_EDETACHED;
    if (needAttach) {
        javaVm->AttachCurrentThread(&env, nullptr);
    }
    long timestamp = env->CallLongMethod(jCamera, update_id);
    sourceFilter->setMatrix(matrix);

    if (cmd == Switch_) {
        auto p = (jintArray) env->CallObjectMethod(jCamera, switch_id);
        jint *param = env->GetIntArrayElements(p, nullptr);
        memcpy(parameter, param, sizeof(int) * 3);
        env->ReleaseIntArrayElements(p, param, 0);
        env->CallVoidMethod(jCamera, start_id, textureId);
        cmd = None;

        matrixSetIdentityM(matrix);
        if (parameter[0] == 1) {
            rotateZd(matrix, 270);
        } else {
            matrixScaleM(matrix, -1, 1, 1);
            rotateZd(matrix, 270);
        }
        Transform::cropViewport(matrix, parameter[1], parameter[2], out_width, out_height);
    }
    if (needAttach) {
        javaVm->DetachCurrentThread();
    }
    return timestamp;
}


void Camera::close() {
    sourceFilter->destroy();

    if (jCamera == nullptr) {
        return;
    }
    int needAttach = javaVm->GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_EDETACHED;
    if (needAttach) {
        javaVm->AttachCurrentThread(&env, nullptr);
    }
    env->CallVoidMethod(jCamera, close_id);
    LOGI("camera closed");

    if (needAttach) {
        javaVm->DetachCurrentThread();
    }
    start_preview = false;
}


void Camera::switchCamera() {
    cmd = Switch_;
}

VideoFrame *Camera::produceFrame() {
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
    long timestamp = updateImage();
    auto *frame = new VideoFrame(textureId, out_width, out_height, timestamp);
    sourceFilter->draw(frame);
    return frame;
}

Camera::~Camera() {
    if (jCamera == nullptr) {
        return;
    }
    int needAttach = javaVm->GetEnv((void **) &env, JNI_VERSION_1_6) == JNI_EDETACHED;
    if (needAttach) {
        javaVm->AttachCurrentThread(&env, nullptr);
    }
    env->DeleteGlobalRef(jCamera);
    jCamera = nullptr;
    env = nullptr;
    javaVm = nullptr;
    if (needAttach) {
        javaVm->DetachCurrentThread();
    }
    DELETEOBJ(sourceFilter)
    DELETEARR(parameter)
    DELETEARR(matrix)
}
