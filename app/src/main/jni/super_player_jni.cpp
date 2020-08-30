#include <jni.h>
#include <memory>
#include "audio/super_audio.h"
#include "common/audio_param.h"
#include "common/papram_factory.h"
#include "audio/merger/audio_merger2.h"
#include "common/merger_param.h"
#include "video/super_video.h"

extern "C" {

std::unique_ptr<SuperAudio> super_audio = nullptr;
std::unique_ptr<AudioMerger2> audio_merger = nullptr;
std::unique_ptr<SuperVideo> super_video = nullptr;
std::shared_ptr<GlView> gl_view = nullptr;
int mode_ = -1;

JNIEXPORT void JNICALL
Java_com_smzh_superplayer_player_PlayerJni_prepare(JNIEnv *env, jobject clazz, jobject audio_param) {
    super_audio = std::make_unique<SuperAudio>(env, clazz);
    std::shared_ptr<AudioParam> spParam = ParamFactory::generalAudioParam(env, audio_param);
    if (super_audio) {
        super_audio->prepare(spParam);
    }
    if (spParam->isWithVideo()) {
        if (super_video == nullptr) {
            mode_ = spParam->isRecorder() ? 0 : 1;
            super_video = std::make_unique<SuperVideo>(mode_);
        }
        super_video->prepare(env, spParam->getVideoPath());
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_start(JNIEnv *jniEnv, jobject clazz) {
    if (super_audio) {
        super_audio->start();
    }
    if (super_video) {
        super_video->start();
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_resume(JNIEnv *jniEnv, jobject clazz) {
    if (super_audio) {
        super_audio->resume();
    }
    if (super_video) {
        super_video->resume();
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_pause(JNIEnv *jniEnv, jobject clazz) {
    if (super_audio) {
        super_audio->pause();
    }
    if (super_video) {
        super_video->pause();
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_stop(JNIEnv *env, jobject clazz) {
    if (super_audio) {
        super_audio->stop();
    }
    super_audio = nullptr;

    if (super_video) {
        super_video->stop();
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_setEcho(JNIEnv *env, jobject clazz, jboolean isEcho) {
    if (super_audio) {
        super_audio->setEcho(isEcho);
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_setFilter(JNIEnv *env, jobject clazz, jfloatArray array) {
    if (super_audio) {
        jfloat *param = env->GetFloatArrayElements(array, nullptr);
        super_audio->setFilter(param);
        env->ReleaseFloatArrayElements(array, param, 0);
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_setPitch(JNIEnv *env, jobject clazz, jfloat pitch) {
    if (super_audio) {
        super_audio->setPitch(pitch);
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_setVolume(JNIEnv *env, jobject clazz, jfloat volume, jint track) {
    if (super_audio) {
        super_audio->setVolume(volume, track);
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_seek(JNIEnv *env, jobject clazz, jlong millis) {
    if (super_audio) {
        super_audio->seek(millis);
    }
}

JNIEXPORT jlong JNICALL Java_com_smzh_superplayer_player_PlayerJni_getTotalMs(JNIEnv *env, jobject clazz) {
    if (super_audio) {
        return super_audio->getTotalMs();
    }
    return 0;
}

JNIEXPORT jlong JNICALL Java_com_smzh_superplayer_player_PlayerJni_getCurrentMs(JNIEnv *env, jobject clazz) {
    int64_t ms = 0;
    if (super_audio) {
        ms = super_audio->getCurrentMs();
    }
    return ms;
}

JNIEXPORT jlong JNICALL Java_com_smzh_superplayer_player_PlayerJni_getRealMs(JNIEnv *env, jobject clazz) {
    long real_ms = 0;
    if (super_audio) {
        real_ms = super_audio->getRealMs();
    }
    return real_ms;
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_startMerge(JNIEnv *env, jobject clazz, jobject param) {
    audio_merger = std::make_unique<AudioMerger2>();
    std::shared_ptr<MergerParam> sp_param = ParamFactory::generalMergerParam(env, param);
    audio_merger->start(sp_param);
}

JNIEXPORT jint JNICALL Java_com_smzh_superplayer_player_PlayerJni_getMergeProgress(JNIEnv *env, jobject clazz) {
    int progress = 0;
    if (audio_merger) {
        progress = audio_merger->getProgress();
    }
    if (progress >= 100) {
        audio_merger = nullptr;
    }
    return progress;
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_onSurfaceCreate(JNIEnv *env, jobject clazz, jobject surface, jint width, jint height, jint mode) {
    mode_ = mode;
    if (gl_view == nullptr) {
        gl_view = std::make_shared<GlView>(mode);
    }
    if (super_video == nullptr) {
        super_video = std::make_unique<SuperVideo>(mode_);
    }
    super_video->init(env, gl_view);
    gl_view->createSurface(env, surface, width, height);
}


JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_onSurfaceDestroy(JNIEnv *env, jobject clazz) {
    if (gl_view) {
        gl_view->destroySurface();
    }
    if (super_audio == nullptr) {
        super_video = nullptr;
        gl_view = nullptr;
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_onFrameAvailable(JNIEnv *env, jobject clazz) {
    if (gl_view) {
        gl_view->requestRender();
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_switchCamera(JNIEnv *env, jobject clazz) {
    if (super_video) {
        super_video->switchCamera();
    }
}

JNIEXPORT void JNICALL Java_com_smzh_superplayer_player_PlayerJni_setVideoEffect(JNIEnv *env, jobject clazz, jobject effect) {
    if (super_video) {
        auto param = ParamFactory::generalVideoEffect(env, effect);
        super_video->setEffect(param);
    }
}

}

