//
// Created by Lvlingling on 2020/6/14.
//

#include "audio_merger2.h"
#include "../codec/media_codec_aac_encoder.h"
#include "../codec/media_codec_merger.h"

void AudioMerger2::start(JNIEnv *env, std::shared_ptr<MergerParam> param) {
    this->sp_param = std::move(param);
    if (sp_param->isVideo()) {
        audioEncoder = new MediaCodecAACEncoder(env);
        if (sp_param->getMode() == 2) {
            videoMerger = new MediaCodecMerger(env);
        }
    } else {
        audioEncoder = new AudioEncoder();
    }
    std::future<void> thread = std::async(std::launch::async, &AudioMerger2::mergerFun, this);
}

void AudioMerger2::mergerFun() {
    unlink(sp_param->getOutPath());
    vocal_buffer = new short[buffer_size];
    acc_buffer = new short[buffer_size];
    std::ifstream vocal_file = std::ifstream(sp_param->getVocalPath(), std::ios::binary | std::ios::in);
    std::ifstream acc_file = std::ifstream(sp_param->getAccPath(), std::ios::binary | std::ios::in);
    audioEncoder->init(44100, 2, sp_param->getOutPath());
    vocal_file.seekg(0, std::ios::end);
    total_bytes = vocal_file.tellg();
    vocal_file.seekg(0, std::ios::beg);

    vocal_filter = new FilterPackage();
    vocal_filter->init(Vocal, 44100, 2);
    vocal_filter->setVolume(sp_param->getVocalVolume());
    vocal_filter->setEffect(sp_param->getEffect());

    acc_filter = new FilterPackage();
    acc_filter->init(Acc, 44100, 2);
    acc_filter->setVolume(sp_param->getAccVolume());
    acc_filter->setPitch(sp_param->getPitch());

    while (merger_bytes < total_bytes) {

        int64_t read_bytes = (merger_bytes + buffer_size * sizeof(short) > total_bytes) ? total_bytes - merger_bytes * sizeof(short) : buffer_size * sizeof(short);
        if (read_bytes < 0) {
            read_bytes = 0;
        }
        vocal_file.read((char *) vocal_buffer, read_bytes);
        vocal_filter->process(vocal_buffer, buffer_size);
        acc_file.read((char *) acc_buffer, read_bytes);
        acc_filter->process(acc_buffer, buffer_size);
        mixAudioBuffer(acc_buffer, vocal_buffer, buffer_size);
        audioEncoder->encode(acc_buffer, (int) (read_bytes / sizeof(short)));
        merger_bytes += buffer_size * sizeof(short);
    }

    merger_bytes = total_bytes;
    audioEncoder->close();

    DELETEOBJ(audioEncoder)
    DELETEARR(vocal_buffer)
    DELETEARR(acc_buffer)
    vocal_filter->destroy();
    acc_filter->destroy();
    DELETEOBJ(vocal_filter)
    DELETEOBJ(acc_filter)
    vocal_file.close();
    acc_file.close();

    if (videoMerger) {
        videoMerger->start(sp_param->getOutPath(), sp_param->getVideoPath(), sp_param->getOutVideoPath());
    }

    sp_param = nullptr;
    finish = true;
    DELETEOBJ(videoMerger)
}


int AudioMerger2::getProgress() {
    if (finish) {
        return 100;
    } else if (merger_bytes >= total_bytes) {
        return 99;
    } else {
        return (int) (merger_bytes * 100 / total_bytes);
    }
}