//
// Created by Lvlingling on 2020/9/1.
//

#ifndef SUPERPLAYER_I_VIDEO_MERGER_H
#define SUPERPLAYER_I_VIDEO_MERGER_H

class IVideoMerger {

public:

    virtual void start(const char *aacPath, const char *h264Path, const char *outPath) = 0;

};

#endif //SUPERPLAYER_I_VIDEO_MERGER_H
