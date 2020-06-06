//
// Created by ldr on 2020/6/6.
//

#include "test_oboe.h"

void TestOboe::start() {
    fileStream = std::ifstream("/storage/emulated/0/000/bridge_girl.pcm", std::ios::binary);
    AudioStreamBuilder outBuilder;
    (&outBuilder)->setCallback(this)
            ->setAudioApi(AudioApi::Unspecified)
            ->setSampleRate(48000)
            ->setFormat(AudioFormat::I16)
            ->setChannelCount(oboe::ChannelCount::Stereo)
            ->setPerformanceMode(PerformanceMode::LowLatency)
            ->setSharingMode(SharingMode::Exclusive)
            ->setDirection(Direction::Output);
    Result result = outBuilder.openManagedStream(outStream);
    outStream->setBufferSizeInFrames(outStream->getFramesPerBurst() * 2);
    result = outStream->requestStart();
}

void TestOboe::stop() {
    outStream->requestStop();
    fileStream.close();
}

DataCallbackResult TestOboe::onAudioReady(AudioStream *stream, void *data, int32_t numFrames) {
    if (!fileStream.eof()) {
        fileStream.read(static_cast<char *>(data), numFrames * 2 * sizeof(short));
        return DataCallbackResult::Continue;
    } else {
        return DataCallbackResult::Stop;
    }
}