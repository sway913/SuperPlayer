//
// Created by ldr on 2019-12-02.
//

#include "MergeEngine.h"
#include "../common/AndroidLog.h"

MergeEngine::MergeEngine() {
    pcmMixer = new PcmMixer();
    aacEncoder = new AacEncoder();
    AudioProperty audioProperty = AudioProperty();
    vocalFilter = new FilterPackage(audioProperty);
    musicFilter = new FilterPackage(audioProperty);
}

void MergeEngine::init(const char *vocalPath, const char *musicPath, const char *mixPath) {
    vocalStream = new std::ifstream(vocalPath, std::ios::binary | std::ios::in);
    musicStream = new std::ifstream(musicPath, std::ios::binary | std::ios::in);
    mixStream = new std::ofstream(mixPath, std::ios::out | std::ios::binary);

    if (!vocalStream || !musicStream || !mixStream) {
        fail = true;
        return;
    }
    vocalBuffer = new char[bufferSize];
    musicBuffer = new char[bufferSize];
    mixBuffer = new char[bufferSize];
    encoderBuffer = new char[bufferSize];

    aacEncoder->init(44100, 2, 128000);
    LOGI("merge init success");
}

void MergeEngine::startMerge(int vocalVolume, int filter, int musicVolume, float musicPitch) {
    vocalFilter->setVolume(vocalVolume);
    vocalFilter->setFilter(filter);
    musicFilter->setVolume(musicVolume);
    musicFilter->setPitch(static_cast<int>(musicPitch));
    mergeResult = std::async(std::launch::async, &MergeEngine::mergeFun, this);
}

void MergeEngine::mergeFun() {
    vocalStream->seekg(0, std::ios::beg);
    vocalStream->seekg(0, std::ios::end);
    fileLength = static_cast<int>(vocalStream->tellg());
    vocalStream->seekg(0, std::ios::beg);

    //LOGI("file length is %d", fileLength);
    while (mergeLength < fileLength) {

        memset(vocalBuffer, 0, static_cast<size_t>(bufferSize));
        memset(musicBuffer, 0, static_cast<size_t>(bufferSize));
        int readSize = bufferSize;

        // LOGI("read size is %d", readSize);

        if (fileLength - mergeLength < bufferSize) {
            readSize = fileLength - mergeLength;
        }
        vocalStream->read(vocalBuffer, readSize);
        readSize = (int) vocalStream->gcount();
        musicStream->read(musicBuffer, readSize);

        vocalFilter->process(vocalBuffer, readSize);
        musicFilter->process(musicFilter, readSize);

        mergeLength += readSize * sizeof(char);

        vocalStream->seekg(mergeLength, std::ios::beg);
        musicStream->seekg(mergeLength, std::ios::beg);

        pcmMixer->mix((int8_t *) vocalBuffer, (int8_t *) musicBuffer, readSize, (int8_t *) mixBuffer);
        int encodeSize = aacEncoder->encode((int8_t *) mixBuffer, (int8_t *) (encoderBuffer), readSize);
        if (encodeSize > 0) {
            mixStream->write(encoderBuffer, sizeof(char) * encodeSize);
            //LOGI("merge frame");
        } else if (encodeSize == 0) {

        } else {
            LOGI("merge fail");
            fail = true;
            break;
        }
    }

    if (vocalStream) {
        vocalStream->close();
    }

    if (musicStream) {
        musicStream->close();
    }

    if (mixStream) {
        mixStream->seekp(0, std::ios::end);
        mixStream->flush();
        mixStream->close();
    }

    LOGI("merge finish");
    complete = true;
}


void MergeEngine::stopMerge() {
    std::future_status status;
    do {
        status = mergeResult.wait_for(std::chrono::milliseconds(200));
    } while (status != std::future_status::ready);
}

int MergeEngine::getMergeProgress() {
    if (complete) {
        LOGI("merger progress 100");
        return 100;
    }
    if (fail) {
        return -1;
    }
    if (fileLength > 0) {
        int progress = static_cast<int>((float) mergeLength / fileLength * 100);
        if (progress >= 98) {
            return 100;
        } else {
            return progress;
        }
    } else {
        return 100;
    }
}

MergeEngine::~MergeEngine() {
    if (vocalStream) {
        delete vocalStream;
        vocalStream = nullptr;
    }

    if (musicStream) {
        delete musicStream;
        musicStream = nullptr;
    }

    if (mixStream) {
        delete mixStream;
        mixStream = nullptr;
    }

    if (vocalBuffer) {
        delete[] vocalBuffer;
        vocalBuffer = nullptr;
    }

    if (musicBuffer) {
        delete[] musicBuffer;
        musicBuffer = nullptr;
    }

    if (mixBuffer) {
        delete[] mixBuffer;
        mixBuffer = nullptr;
    }

    if (encoderBuffer) {
        delete[] encoderBuffer;
        encoderBuffer = nullptr;
    }

    if (aacEncoder) {
        delete aacEncoder;
        aacEncoder = nullptr;
    }
    delete vocalFilter;
    delete musicFilter;

}
