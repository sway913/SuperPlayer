//
// Created by Lvlingling on 2020/9/1.
//

#ifndef SUPERPLAYER_I_AUDIO_ENCODER_H
#define SUPERPLAYER_I_AUDIO_ENCODER_H

class IAudioEncoder {

public:

    virtual void init(int in_sample, int in_channels, const char *file_name) = 0;

    virtual void encode(short *data, int len) = 0;

    virtual void close() = 0;

    virtual ~IAudioEncoder() {

    }
};

#endif //SUPERPLAYER_I_AUDIO_ENCODER_H
