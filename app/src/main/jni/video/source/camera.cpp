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
    update_id = env->GetMethodID(clazz, "updateImage", "()V");
    switch_id = env->GetMethodID(clazz, "switchCamera", "()[I");
    start_id = env->GetMethodID(clazz, "startPreview", "(I)V");

    env->DeleteLocalRef(clazz);

    sourceFilter = new SourceFilter();
    parameter = new int[3];
    matrix = new float[16];
}

void Camera::open(int w, int h) {
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

    matrixSetIdentityM(matrix);
    if (parameter[0] == 1) {
        rotateZd(matrix, 270);
    } else {
        matrixScaleM(matrix, -1, 1, 1);
        rotateZd(matrix, 270);
    }
    Transform::cropViewport(matrix, parameter[1], parameter[2], out_width, out_height);
    sourceFilter->setMatirx(matrix);

    Cmd c = cmd;
    switch (c) {
        case Close: {
            env->CallVoidMethod(jCamera, close_id);
            env->DeleteGlobalRef(jCamera);
            jCamera = nullptr;
            sourceFilter->destroy();
            DELETEOBJ(sourceFilter)
            cmd = None;
            break;
        }
        case Switch_: {
            auto p = (jintArray) env->CallObjectMethod(jCamera, switch_id);
            jint *param = env->GetIntArrayElements(p, nullptr);
            memcpy(parameter, param, sizeof(int) * 3);
            env->ReleaseIntArrayElements(p, param, 0);
            env->CallVoidMethod(jCamera, start_id, textureId);
            cmd = None;
            break;
        }
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
    DELETEARR(parameter)
    DELETEARR(matrix)
}
