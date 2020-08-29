//
// Created by Lvlingling on 2020/8/23.
//

#ifndef SUPERPLAYER_VIDEO_FRAME_H
#define SUPERPLAYER_VIDEO_FRAME_H

class VideoFrame {

public:

    GLuint textureId;
    uint32_t width;
    uint32_t height;
    long timestamp;

    VideoFrame() {}

    VideoFrame(GLuint textureId, uint32_t width, uint32_t height, long timestamp) : textureId(textureId), width(width), height(height), timestamp(timestamp) {}
};

#endif //SUPERPLAYER_VIDEO_FRAME_H
