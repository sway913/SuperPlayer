//
// Created by Lvlingling on 2020/6/25.
//

#ifndef SUPERPLAYER_CAMERA_H
#define SUPERPLAYER_CAMERA_H


#include <jni.h>
#include "../opengl/opengl_utils.h"
#include "../filter/source_filter.h"
#include "../../common/common_tools.h"
#include "source.h"

class Camera : public Source {

    enum Cmd {
        None,
        Close,
        Switch_,
    };

public:

    Camera(JNIEnv *env);

    void open(int w, int h) override;

    void close() override;

    void switchCamera();

    GLuint produceFrame() override;

    virtual ~Camera();

private:

    void updateImage();

    int textureId{-1};
    JNIEnv *env{nullptr};
    JavaVM *javaVm{nullptr};
    jobject jCamera{nullptr};
    jmethodID update_id{nullptr};
    jmethodID open_id{nullptr};
    jmethodID close_id{nullptr};
    jmethodID switch_id{nullptr};
    jmethodID start_id{nullptr};

    bool start_preview{false};
    Cmd cmd{None};
    SourceFilter *sourceFilter{nullptr};
    int out_width{0};
    int out_height{0};
    int *parameter{nullptr};
    float *matrix{nullptr};

};


#endif //SUPERPLAYER_CAMERA_H
