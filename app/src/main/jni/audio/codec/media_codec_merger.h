//
// Created by Lvlingling on 2020/9/1.
//

#ifndef SUPERPLAYER_MEDIA_CODEC_MERGER_H
#define SUPERPLAYER_MEDIA_CODEC_MERGER_H


#include <jni.h>
#include "i_video_merger.h"

class MediaCodecMerger : public IVideoMerger {

private:

    JNIEnv *env{nullptr};
    JavaVM *javaVm{nullptr};
    jobject j_merger{nullptr};
    jmethodID start_id{nullptr};

public:

    MediaCodecMerger(JNIEnv *env);

    void start(const char *aacPath, const char *h264Path, const char *outPath) override;


};


#endif //SUPERPLAYER_MEDIA_CODEC_MERGER_H
