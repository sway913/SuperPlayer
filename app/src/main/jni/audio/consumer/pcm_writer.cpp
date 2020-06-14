//
// Created by Lvlingling on 2020/6/7.
//

#include "pcm_writer.h"

PcmWriter::PcmWriter() {
    circle_Buffer = new CircleBuffer<short>(CIRCLE_BUFFER_CAPBILATY);
    bufferReadKey = circle_Buffer->getReadKey();
    tmp_buffer = new short[BUFFER_SIZE];
    audioEncoder = new AudioEncoder();
}

void PcmWriter::start(const char *fileName, int in_sample) {
    valid = true;
    this->in_sample_rate = in_sample;
    unlink(fileName);
    this->file_name = fileName;
    writeResult = std::async(std::launch::async, &PcmWriter::writeToFile, this);
}


void PcmWriter::writeToFile() {
    audioEncoder->init(in_sample_rate, 1, file_name);
    while (valid) {
        std::unique_lock<std::mutex> lock(mutex);
        cond.wait(lock);

        int copied = 0;
        int total_length = circle_Buffer->length(bufferReadKey);
        total_length = total_length - (total_length % 2);

        while (copied < total_length) {
            int copyCount = MIN(total_length - copied, BUFFER_SIZE);
            copied += copyCount;
            int count = circle_Buffer->read(bufferReadKey, tmp_buffer, copyCount);
            if (count > 0) {
                audioEncoder->encode(tmp_buffer, count);
            }
        }
    }
    audioEncoder->close();
}

void PcmWriter::seek(int64_t millis) {
    int64_t fileSize = 44100 * 2 * sizeof(int16_t) / 1000 * millis;

}


void PcmWriter::stop() {
    valid = false;
    cond.notify_all();
    JOIN(writeResult);
}

void PcmWriter::write(short *data, int count) {
    circle_Buffer->write(data, count);
    cond.notify_all();
}

PcmWriter::~PcmWriter() {
    DELETEOBJ(circle_Buffer)
    DELETEOBJ(audioEncoder)
}