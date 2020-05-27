//
// Created by ldr on 2019-11-24.
//

#include "FileDecoder.h"
#include "../common/AndroidLog.h"
#include <utility>

void FileDecoder::init(const char *fileName, const char *decodePath, AudioProperty outProperty, int decodeLength) {
    this->fileName = fileName;
    this->decodePath = decodePath;
    this->decodeLength = decodeLength;
    this->outProperty = outProperty;
}

void FileDecoder::startDecode(std::function<void(void)> decodeCallback) {
    this->decodeCallback = std::move(decodeCallback);
    if (decodeLength == 0) {
        auto *file = new ifstream(fileName, std::ios::in | std::ios::binary);
        file->seekg(0, std::ios::end);
        decodeLength = (int) file->tellg();
        file->seekg(0, std::ios::beg);
        file->close();
        delete file;
    }
    decodeResult = std::async(std::launch::async, &FileDecoder::decodeFun, this);
}

void FileDecoder::decodeFun() {

    auto *decodeStream = new std::ofstream(decodePath, std::ios::out | std::ios::binary);
    auto *ffDecoder = new FFDecoder();
    ffDecoder->prepare(fileName, outProperty);

    int readLength = 0;

    uint8_t *outBuffer = nullptr;

    while (readLength < decodeLength) {
        int len = ffDecoder->decodeFrame(outBuffer);
        if (len > 0 && outBuffer != nullptr) {
            decodeStream->write((const char *) outBuffer, len);
            readLength += len;
            //LOGI("decode frame success");
        } else {
            LOGI("decode fail");
            break;
        }
    }

    if (outBuffer) {
        free(outBuffer);
    }

    decodeStream->flush();
    decodeStream->close();
    ffDecoder->close();

    if (decodeCallback) {
        decodeCallback();
    }

    delete decodeStream;
    delete ffDecoder;
}

void FileDecoder::stop() {
    decodeLength = -1;
}

FileDecoder::~FileDecoder() = default;
