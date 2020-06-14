//
// Created by Lvlingling on 2020/6/12.
//

#include "audio_merger.h"

void AudioMerger::start(std::shared_ptr<MergerParam> param) {
    this->sp_param = std::move(param);
    acc_buffer = new CircleBuffer<short>(MERGER_BUFFER_CAPBILATY);
    vocal_buffer = new CircleBuffer<short>(MERGER_BUFFER_CAPBILATY);
    acc_key = acc_buffer->getReadKey();
    vocal_key = vocal_buffer->getReadKey();
    std::future<void> thread = std::async(std::launch::async, &AudioMerger::mergerFun, this);
}

void AudioMerger::mergerFun() {
    unlink(sp_param->getOutPath());
    audioEncoder = new AudioEncoder();
    audioEncoder->init(44100, 2, sp_param->getOutPath());

    accDecoder = new AudioDecoder();
    accDecoder->prepare(sp_param->getAccPath(), 44100);

    vocalDecoder = new AudioDecoder();
    vocalDecoder->prepare(sp_param->getVocalPath(), 44100);

    total_bytes = vocalDecoder->getTotalMs() * 2.f * (44100 / 1000) * sizeof(short);

    AudioBuffer *tmp_buffer = nullptr;
    while (merger_bytes < total_bytes) {
        if (!(tmp_buffer = vocalDecoder->decodeFrame())->isEmpty()) {
            AudioFrame *vocal_frame = nullptr;
            while ((vocal_frame = tmp_buffer->pop())) {
                if (vocal_frame->audio_data && vocal_frame->len > 0) {
                    vocal_buffer->write((short *) (vocal_frame->audio_data), vocal_frame->len / sizeof(short));
                }
                DELETEOBJ(vocal_frame)
            }
            DELETEOBJ(tmp_buffer)
        }
        if (!(tmp_buffer = accDecoder->decodeFrame())->isEmpty()) {
            AudioFrame *acc_frame = nullptr;
            while ((acc_frame = tmp_buffer->pop())) {
                if (acc_frame->audio_data && acc_frame->len > 0) {
                    acc_buffer->write((short *) (acc_frame->audio_data), acc_frame->len / sizeof(short));
                }
                DELETEOBJ(acc_frame)
            }
            DELETEOBJ(tmp_buffer)
        }
        int len = min(acc_buffer->length(acc_key), vocal_buffer->length(vocal_key));

//        if (acc_buffer->length(acc_key) == 0 && vocal_buffer->length(vocal_key) == 0) {
//            break;
//        }

        auto *tmp_acc = new short[len];
        auto *tmp_vocal = new short[len];
        int acc_len = acc_buffer->read(acc_key, tmp_acc, len);
        int vocal_len = vocal_buffer->read(vocal_key, tmp_vocal, len);
        len = min(vocal_len, acc_len);
        mixAudioBuffer(tmp_acc, tmp_vocal, len);
        audioEncoder->encode(tmp_acc, len);
        DELETEARR(tmp_acc)
        DELETEARR(tmp_vocal)
        merger_bytes += len * sizeof(short);
    }

    merger_bytes = total_bytes;
    audioEncoder->close();
    accDecoder->close();
    vocalDecoder->close();

    DELETEOBJ(audioEncoder)
    DELETEOBJ(accDecoder)
    DELETEOBJ(vocalDecoder)
    sp_param = nullptr;
}


int AudioMerger::getProgress() {
    if (merger_bytes < total_bytes) {
        return (int) (merger_bytes * 100 / total_bytes);
    } else {
        return 100;
    }
}