//
// Created by Lvlingling on 2020/6/14.
//

#include "audio_merger2.h"

void AudioMerger2::start(std::shared_ptr<MergerParam> param) {
    this->sp_param = std::move(param);
    std::future<void> thread = std::async(std::launch::async, &AudioMerger2::mergerFun, this);
}

void AudioMerger2::mergerFun() {
    unlink(sp_param->getOutPath());
    vocal_buffer = new short[buffer_size];
    acc_buffer = new short[buffer_size];
    mix_buffer = new short[buffer_size];
    std::ifstream vocal_file = std::ifstream(sp_param->getVocalPath(), std::ios::binary | std::ios::in);
    std::ifstream acc_file = std::ifstream(sp_param->getAccPath(), std::ios::binary | std::ios::in);
    audioEncoder = new AudioEncoder();
    audioEncoder->init(44100, 2, sp_param->getOutPath());
    vocal_file.seekg(0, std::ios::end);
    total_bytes = vocal_file.tellg();
    vocal_file.seekg(0, std::ios::beg);

    while (merger_bytes < total_bytes) {

        int read_bytes = (merger_bytes + buffer_size * sizeof(short) > total_bytes) ? total_bytes - merger_bytes * sizeof(short) : buffer_size * sizeof(short);
        vocal_file.read((char *) vocal_buffer, read_bytes);
        acc_file.read((char *) acc_buffer, read_bytes);
        mixAudioBuffer(acc_buffer, vocal_buffer, read_bytes / sizeof(short));
        audioEncoder->encode(acc_buffer, read_bytes / sizeof(short));
        merger_bytes += buffer_size * sizeof(short);
    }

    merger_bytes = total_bytes;
    audioEncoder->close();

    DELETEOBJ(audioEncoder)
    DELETEARR(vocal_buffer)
    DELETEARR(acc_buffer)
    vocal_file.close();
    acc_file.close();
    sp_param = nullptr;
}


int AudioMerger2::getProgress() {
    if (merger_bytes < total_bytes) {
        return (int) (merger_bytes * 100 / total_bytes);
    } else {
        return 100;
    }
}