//
// Created by ldr on 2020/6/15.
//

#include "OpenSLRecorder.h"

static void bqRecorderCallback(SLAndroidSimpleBufferQueueItf bq, void *rec) {
    (static_cast<OpenSLRecorder *>(rec))->processSLCallback(bq);
}

OpenSLRecorder::OpenSLRecorder(int framesPerBurst) : framesPerBurst(framesPerBurst) {}


void OpenSLRecorder::processSLCallback(SLAndroidSimpleBufferQueueItf bq) {
    assert(bq == recBufQueueItf_);
    circleBuffer->write(recordBuffer->getNowBuffer(), framesPerBurst);
    if (isFinish) {
        onStop();
    } else {
        (*recBufQueueItf_)->Enqueue(recBufQueueItf_, recordBuffer->getRecordBuffer(), framesPerBurst * sizeof(short));
    }
}

void OpenSLRecorder::prepare(JNIEnv *env, int inSampleRate, oboe::AudioApi api) {

    recordBuffer = new RecordBuffer(framesPerBurst);
    circleBuffer = new CircleBuffer<short>(framesPerBurst * 4);
    bufferKey = circleBuffer->getReadKey();
    this->sampleRate = inSampleRate;

    SLresult result;

    result = slCreateEngine(&slEngineObj_, 0, nullptr, 0, nullptr, nullptr);
    SLASSERT(result);

    result = (*slEngineObj_)->Realize(slEngineObj_, SL_BOOLEAN_FALSE);
    SLASSERT(result);

    result = (*slEngineObj_)->GetInterface(slEngineObj_, SL_IID_ENGINE, &slEngineItf_);
    SLASSERT(result);

    SLDataLocator_IODevice loc_dev = {SL_DATALOCATOR_IODEVICE,
                                      SL_IODEVICE_AUDIOINPUT,
                                      SL_DEFAULTDEVICEID_AUDIOINPUT,
                                      nullptr};
    SLDataSource audioSrc = {&loc_dev, nullptr};

    SLDataLocator_AndroidSimpleBufferQueue loc_bq = {
            SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
            DEVICE_SHADOW_BUFFER_QUEUE_LEN};

    SLAndroidDataFormat_PCM_EX format_pcm = {SL_DATAFORMAT_PCM, 1, (SLmilliHertz) (inSampleRate * 1000),
                                             SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
                                             SL_SPEAKER_FRONT_CENTER, SL_BYTEORDER_LITTLEENDIAN};

    SLDataSink audioSnk = {&loc_bq, &format_pcm};

    const SLInterfaceID id[2] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
                                 SL_IID_ANDROIDCONFIGURATION};
    const SLboolean req[2] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    result = (*slEngineItf_)->CreateAudioRecorder(slEngineItf_,
                                                  &recObjectItf_,
                                                  &audioSrc,
                                                  &audioSnk,
                                                  sizeof(id) / sizeof(id[0]),
                                                  id, req);
    SLASSERT(result);

    SLAndroidConfigurationItf inputConfig;
    result = (*recObjectItf_)->GetInterface(recObjectItf_, SL_IID_ANDROIDCONFIGURATION, &inputConfig);
    if (SL_RESULT_SUCCESS == result) {
        SLuint32 presetValue = SL_ANDROID_RECORDING_PRESET_VOICE_RECOGNITION;
        (*inputConfig)->SetConfiguration(inputConfig,
                                         SL_ANDROID_KEY_RECORDING_PRESET,
                                         &presetValue,
                                         sizeof(SLuint32));
    }
    result = (*recObjectItf_)->Realize(recObjectItf_, SL_BOOLEAN_FALSE);
    SLASSERT(result);

    result = (*recObjectItf_)->GetInterface(recObjectItf_, SL_IID_RECORD, &recItf_);
    SLASSERT(result);

    result = (*recObjectItf_)->GetInterface(recObjectItf_, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &recBufQueueItf_);
    SLASSERT(result);

    result = (*recBufQueueItf_)->RegisterCallback(recBufQueueItf_, bqRecorderCallback, this);
    SLASSERT(result);
}

int OpenSLRecorder::start() {

    SLresult result;
    result = (*recItf_)->SetRecordState(recItf_, SL_RECORDSTATE_STOPPED);
    SLASSERT(result);
    result = (*recBufQueueItf_)->Clear(recBufQueueItf_);
    SLASSERT(result);

    result = (*recBufQueueItf_)->Enqueue(recBufQueueItf_, recordBuffer->getRecordBuffer(), framesPerBurst * sizeof(short));
    SLASSERT(result);

    result = (*recItf_)->SetRecordState(recItf_, SL_RECORDSTATE_RECORDING);
    SLASSERT(result);
    isFinish = false;
    return 0;
}

void OpenSLRecorder::stop() {
    isFinish = true;
}

void OpenSLRecorder::onStop() {
    SLuint32 curState;

    SLresult result = (*recItf_)->GetRecordState(recItf_, &curState);
    SLASSERT(result);
    if (curState == SL_RECORDSTATE_STOPPED) {
        return;
    }
    result = (*recItf_)->SetRecordState(recItf_, SL_RECORDSTATE_STOPPED);
    SLASSERT(result);
    result = (*recBufQueueItf_)->Clear(recBufQueueItf_);
    SLASSERT(result);

    if (recObjectItf_ != nullptr) {
        (*recObjectItf_)->Destroy(recObjectItf_);
    }

    if (slEngineObj_ != nullptr) {
        (*slEngineObj_)->Destroy(slEngineObj_);
        slEngineObj_ = nullptr;
        slEngineItf_ = nullptr;
    }

    DELETEOBJSAFE(recordBuffer)
    DELETEOBJSAFE(circleBuffer)
}

int OpenSLRecorder::read(short *audioData, int numFrames) {
    int len = circleBuffer->length(bufferKey);
    if (len <= 0) {
        return 0;
    } else if (len < numFrames) {
        return circleBuffer->read(bufferKey, audioData, len);
    } else {
        return circleBuffer->read(bufferKey, audioData, numFrames);;
    }
}

RecordState OpenSLRecorder::readyState(int16_t *data, int32_t numFrames) {
    if (mCountCallbacksToDrain > 0) {
        if (read(data, numFrames) > 0) {
            mCountCallbacksToDrain--;
        }
        if (mCountCallbacksToDrain == 0) {
            return RecordState::success;
        }
    }
    if (mCountCallbacksToDrain == 0) {
        return RecordState::ready;
    } else {
        return RecordState::preparing;
    }
}

int OpenSLRecorder::getSampleRate() {
    return sampleRate;
}

int OpenSLRecorder::getChannels() {
    return 1;
}