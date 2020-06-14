//
// Created by ldr on 2019-12-02.
//

#ifndef SUPERPLAYER_MERGEENGINE_H
#define SUPERPLAYER_MERGEENGINE_H


#include "PcmMixer.h"
#include "../../audio/codec/aac_encoder.h"
#include "../filter/FilterPackage.h"
#include <fstream>
#include <future>
#include <jni.h>

class MergeEngine {

private:
    PcmMixer *pcmMixer;
    std::ifstream *vocalStream;
    std::ifstream *musicStream;
    std::ofstream *mixStream;

    std::future<void> mergeResult;

    int bufferSize = 1024;
    int mergeLength = 0;
    int fileLength = 0;

    char *vocalBuffer = nullptr;
    char *musicBuffer = nullptr;
    char *mixBuffer = nullptr;
    char *encoderBuffer = nullptr;

    AacEncoder *aacEncoder{nullptr};
    FilterPackage *vocalFilter{nullptr};
    FilterPackage *musicFilter{nullptr};

    bool fail = false;
    volatile bool complete = false;

public:

    MergeEngine();

    void init(const char *voclaPath, const char *musicPath, const char *mixPath);

    void startMerge(int vocalVolume, int vocalFilter, int musicVolume, float musicPitch);

    void stopMerge();

    void mergeFun();

    int getMergeProgress();

    virtual ~MergeEngine();
};


#endif //SUPERPLAYER_MERGEENGINE_H
