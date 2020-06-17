//
// Created by Lvlingling on 2020/6/14.
//

#include "pcm_writer2.h"

PcmWriter2::PcmWriter2() {
    circle_Buffer = new CircleBuffer<short>(CIRCLE_BUFFER_CAPBILATY);
    bufferReadKey = circle_Buffer->getReadKey();
    tmp_buffer = new short[BUFFER_SIZE];
}

void PcmWriter2::start(const char *fileName, int in_sample) {
    valid = true;
    this->in_sample_rate = in_sample;
    unlink(fileName);
    this->file_name = fileName;
    pcmFileStream = new std::ofstream(fileName, std::ios::out | std::ios::binary);
    resampleHelper = new ResampleHelper(1, in_sample_rate, 2, 44100);
    writeResult = std::async(std::launch::async, &PcmWriter2::writeToFile, this);
}


void PcmWriter2::writeToFile() {
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
                int nb_sample = count * 2 / (1 * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16));
                int out_sample = (int) ((float) nb_sample * ((float) 44100 / (float) in_sample_rate * 1.2f));
                int out_size = 2 * out_sample * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
                auto *out = new uint8_t[out_size];
                int32_t len = resampleHelper->resample(out, out_sample, reinterpret_cast<const uint8_t *>(tmp_buffer), nb_sample);
                if (len > 0) {
                    int64_t byte_size = len * 2 * sizeof(int16_t);
                    pcmFileStream->write((const char *) out, byte_size);
                    write_byte_size += byte_size;
                }
                DELETEARR(out)
            }
        }
    }
}

void PcmWriter2::seek(int64_t millis) {
    int64_t fileSize = millis * 44100 * 2 * sizeof(int16_t) / 1000;
    write_byte_size = fileSize;
    if (pcmFileStream && pcmFileStream->is_open())
        pcmFileStream->seekp(fileSize);
}


void PcmWriter2::stop() {
    valid = false;
    cond.notify_all();
    JOIN(writeResult);
    if (pcmFileStream) {
        pcmFileStream->flush();
        pcmFileStream->seekp(0, std::ios::end);
        long file_len = pcmFileStream->tellp();
        pcmFileStream->close();
        if (file_len > write_byte_size) {
            truncate(file_name, write_byte_size);
        }
    }

}

void PcmWriter2::write(short *data, int count) {
    circle_Buffer->write(data, count);
    cond.notify_all();
}

PcmWriter2::~PcmWriter2() {
    DELETEOBJ(circle_Buffer)
    DELETEOBJ(resampleHelper)
    DELETEOBJ(pcmFileStream)

}