//
// Created by ldr on 2019-11-10.
//

#ifndef SUPERPLAYER_COMMON_H
#define SUPERPLAYER_COMMON_H

#define CACHE_FRAME_NUM 5
#define READ_SIZE 4096
#define DEFAULT_SAMPLE 44100
#define DEFAULT_CHANNEL_COUNT 2
#define COUNT_CALLBACK_PREPARE 50
constexpr int MAX_QUEUE_NUM = 2 * 1024 * 1024;
constexpr int MAX_QUEUE_PKTS =  2 * 1024;


enum STATUS {
    INIT, PLAYING, PAUSE, STOP, ERROR
};

#endif

