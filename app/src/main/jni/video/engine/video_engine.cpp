//
// Created by Lvlingling on 2020/8/2.
//

#include "video_engine.h"
#include "record_engine.h"
#include "play_engine.h"

VideoEngine *VideoEngine::getVideoEngine(int mode) {
    if (mode == 0) {
        return new RecordEngine();
    } else {
        return new PlayEngine();
    }
}


void VideoEngine::switchCamera() {

}

void VideoEngine::setEffect(std::shared_ptr<VideoEffect> &effect) {

}

VideoEngine::~VideoEngine() {
    glView = nullptr;
    LOGI("~ Video Engine");
};
