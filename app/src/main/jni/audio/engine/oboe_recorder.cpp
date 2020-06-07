//
// Created by ldr on 2020/6/6.
//

#include "oboe_recorder.h"

int OboeRecorder::start(int sample_rate) {
    AudioStreamBuilder builder;
    (&builder)->setCallback(nullptr)
            ->setDeviceId(kUnspecified)
            ->setDirection(Direction::Input)
            ->setChannelConversionAllowed(true)
            ->setFormatConversionAllowed(true)
            ->setChannelCount(Mono)
            ->setSampleRateConversionQuality(oboe::SampleRateConversionQuality::Medium)
            ->setSampleRate(sample_rate)
            ->setAudioApi(AudioApi::Unspecified)
            ->setFormat(AudioFormat::I16)
            ->setSharingMode(SharingMode::Exclusive)
            ->setPerformanceMode(PerformanceMode::LowLatency);
    if (getSdkVersion() >= 29) {
        (&builder)->setInputPreset(oboe::InputPreset::VoicePerformance);
    } else {
        (&builder)->setInputPreset(oboe::InputPreset::Generic);
    }

    Result result = builder.openManagedStream(inputStream);
    if (result == Result::OK) {
        result = inputStream->requestStart();
    }
    return result == Result::OK ? 0 : -1;
}


void OboeRecorder::stop() {
    if (inputStream) {
        inputStream->requestStop();
        inputStream = nullptr;
    }
}

int OboeRecorder::read(short *buffer, int numFrames) {
    ResultWithValue<int32_t> result = inputStream->read(buffer, numFrames, 0);
    if (!result) {
        return 0;
    } else {
        int32_t framesRead = result.value();
        auto xRunCountResultInput = inputStream->getXRunCount();
        int32_t xRunCnt = xRunCountResultInput.value();
        while (xRunCnt > 0 && framesRead <= 0) {
            framesRead = inputStream->read(buffer, numFrames, 0).value();
            xRunCnt--;
        }
        return framesRead;
    }
}

RecordState OboeRecorder::getState(int16_t *data, int32_t numFrames) {
    int32_t actualFramesRead = 0;
    if (count_callback_to_drain > 0) {
        int32_t totalFramesRead = 0;
        do {
            ResultWithValue<int32_t> result = inputStream->read(data, numFrames, 0);
            if (!result) { break; }
            actualFramesRead = result.value();
            totalFramesRead += actualFramesRead;
        } while (actualFramesRead > 0);

        if (totalFramesRead > 0) {
            count_callback_to_drain--;
        }
    } else if (count_callback_to_discard > 0) {
        ResultWithValue<int32_t> result = inputStream->read(data, numFrames, 0);
        if (!result) {
            return RecordState::fail;
        }
        count_callback_to_discard--;
        if (count_callback_to_discard == 0) {
            return RecordState::success;
        }
    }
    if (count_callback_to_discard == 0) {
        return RecordState::ready;
    } else {
        return RecordState::preparing;
    }
}