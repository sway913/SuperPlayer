//
// Created by ldr on 2019-11-18.
//

#include "PcmWriter.h"
#include "../../common/AndroidLog.h"


PcmWriter::PcmWriter() {
    inputBuffer = new CircleBuffer<short>(CIRCLE_BUFFER_CAPBILATY);
    bufferReadKey = inputBuffer->getReadKey();
    buffer = new short[BUFFER_SIZE];
}

void PcmWriter::open(const char *fileName, AudioProperty outProperty, AudioProperty inProperty) {
    this->inProperty = inProperty;
    this->outProperty = outProperty;
    isWriting = true;
    resample = new Resample(inProperty, outProperty);
    pcmFileStream = new std::ofstream(fileName, std::ios::out | std::ios::binary);
    writeResult = std::async(std::launch::async, &PcmWriter::writeToFile, this);
}


void PcmWriter::writeToFile() {
    while (isWriting) {
        std::unique_lock<std::mutex> lock(mutex);
        cond.wait(lock);

        int copied = 0;
        int writeCount = 0;
        int total_length = inputBuffer->length(bufferReadKey);
        //total_length = total_length - (total_length % 2);

        while (copied < total_length) {
            int copyCount = MIN(total_length - copied, BUFFER_SIZE);
            copied += copyCount;
            int count = inputBuffer->read(bufferReadKey, buffer, copyCount);
            if (count > 0) {
                int nb_sample = count * 2 / (inProperty.channelCount * av_get_bytes_per_sample(AVSampleFormat::AV_SAMPLE_FMT_S16));
                int out_sample = static_cast<int>((float) nb_sample * ((float) outProperty.sampleRate / (float) inProperty.sampleRate * 1.2f));
                int out_size = outProperty.channelCount * out_sample * av_get_bytes_per_sample(AVSampleFormat::AV_SAMPLE_FMT_S16);
                uint8_t *out = new uint8_t[out_size];
                int len = resample->resample(out, out_sample, reinterpret_cast<const uint8_t *>(buffer), nb_sample);
                if (len > 0) {
                    int actualSize = len * outProperty.channelCount * sizeof(int16_t);
                    pcmFileStream->write(reinterpret_cast<const char *>(out), actualSize);
                    writeCount += actualSize / sizeof(short);
                }
                delete[] out;
            }
        }
    }
}

void PcmWriter::seek(int64_t millis) {
    int fileSize = outProperty.sampleRate * outProperty.channelCount * sizeof(int16_t) / 1000 * millis;
    if (pcmFileStream && pcmFileStream->is_open())
        pcmFileStream->seekp(fileSize);
}


void PcmWriter::close() {
    isWriting = false;
    cond.notify_all();
    std::future_status status;
    do {
        status = writeResult.wait_for(std::chrono::milliseconds(100));
    } while (status != std::future_status::ready);
    if (pcmFileStream) {
        pcmFileStream->seekp(0, std::ios::end);
        pcmFileStream->flush();
        pcmFileStream->close();
    }
}

void PcmWriter::write(short *data, int count) {
    inputBuffer->write(data, count);
    cond.notify_all();
}

PcmWriter::~PcmWriter() {
    if (pcmFileStream) {
        delete pcmFileStream;
        pcmFileStream = nullptr;
    }

    delete inputBuffer;
    inputBuffer = nullptr;

    if (resample) {
        delete resample;
        resample = nullptr;
    }
}
