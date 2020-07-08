#ifndef PNG_DECODER_H
#define PNG_DECODER_H

extern "C" {
#include "./image.h"
}

#include "./rgba_frame.h"

/**
 * PNG图像解码
 */
class PngDecoder {
private:
    FILE *pngFile;

    char *compressedData;
    int actualSize;
public:
    PngDecoder();

    /** 开启解码线程 **/
    virtual ~PngDecoder();

    /** 打开本地文件不需要传递 探针的参数以及重试策略 **/
    int openFile(char *pngFilePath);

    RGBAFrame *getRGBAFrame();

    RGBAFrame *getRGBAFrameFromBuffer(uint8_t *buffer, size_t size);

    void closeFile();

};

#endif //PNG_DECODER_H

