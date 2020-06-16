//
// Created by ldr on 2020/6/15.
//

#ifndef POKEKARA_OPENSLRECORDER_H
#define POKEKARA_OPENSLRECORDER_H

extern "C" {
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
};

#include "GeneralRecorder.h"
#include "../utils/logging.h"
#include "../common/CommonTools.h"
#include "RecordBuffer.h"
#include "../common/CircleBuffer.h"

#define AUDIO_SAMPLE_CHANNELS               1
#define DEVICE_SHADOW_BUFFER_QUEUE_LEN      2
#define RECORD_DEVICE_KICKSTART_BUF_COUNT   2

#define SLASSERT(x)   do {\
    assert(SL_RESULT_SUCCESS == (x));\
    (void) (x);\
    } while (0)


class OpenSLRecorder : public GeneralRecorder {

private:

    //引擎接口
    SLObjectItf slEngineObj_{nullptr};
    //引擎对象
    SLEngineItf slEngineItf_{nullptr};
    //录音器接口
    SLObjectItf recObjectItf_{nullptr};
    //录音器对象
    SLRecordItf recItf_{nullptr};
    //缓冲队列
    SLAndroidSimpleBufferQueueItf recBufQueueItf_{nullptr};

    RecordBuffer *recordBuffer{nullptr};

    void onStop();

    volatile bool isFinish{false};

    int framesPerBurst{0};

    CircleBuffer<short> *circleBuffer{nullptr};
    int bufferKey{0};
    int sampleRate{0};
    int mCountCallbacksToDrain = 50;

public:

    OpenSLRecorder(int framesPerBurst);

    void prepare(JNIEnv *env, int inSampleRate, oboe::AudioApi api) override;

    int start() override;

    int read(short *audioData, int numFrames) override;

    void stop() override;

    int getSampleRate() override;

    int getChannels() override;

    RecordState readyState(int16_t *data, int32_t numFrames) override;

    void processSLCallback(SLAndroidSimpleBufferQueueItf bq);

};


#endif //POKEKARA_OPENSLRECORDER_H
