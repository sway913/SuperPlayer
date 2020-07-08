#include "./png_decoder.h"
#include "../../common/common_tools.h"

#define LOG_TAG "PngDecoder"

PngDecoder::PngDecoder() {
}

PngDecoder::~PngDecoder() {
}

int PngDecoder::openFile(char *pngFilePath) {
	pngFile = fopen(pngFilePath, "rb");
	if(NULL != pngFile){
		fseek(pngFile, 0, SEEK_END);
		int data_length = ftell(pngFile);
		rewind(pngFile);
		compressedData = new char[data_length];
		actualSize = fread(compressedData, 1, data_length, pngFile);
//        LOGI("data_length is %d actual size is %d", data_length, actualSize);
		return 1;
	}
	return -1;
}

RGBAFrame* PngDecoder::getRGBAFrame(){
	RGBAFrame* frame = new RGBAFrame();
	RawImageData data = get_raw_image_data_from_png((void*) compressedData, actualSize);
	frame->width = data.width;
	frame->height = data.height;
	int expectLength = data.width * data.height * 4;
	uint8_t * pixels = new uint8_t[expectLength];
	memset(pixels, 0, sizeof(uint8_t) * expectLength);
	int pixelsLength = MIN(expectLength, data.size);
	memcpy(pixels, (char *) data.data, pixelsLength);
	frame->pixels = pixels;
	release_raw_image_data(&data);
	return frame;
}

void PngDecoder::closeFile(){
	delete[] compressedData;
	fclose(pngFile);
}

RGBAFrame *PngDecoder::getRGBAFrameFromBuffer(uint8_t *buffer, size_t size) {
	RGBAFrame* frame = new RGBAFrame();
	RawImageData data = get_raw_image_data_from_png((void*) buffer, size);
	frame->width = data.width;
	frame->height = data.height;
	int expectLength = data.width * data.height * 4;
	uint8_t * pixels = new uint8_t[expectLength];
	memset(pixels, 0, sizeof(uint8_t) * expectLength);
	int pixelsLength = MIN(expectLength, data.size);
	memcpy(pixels, (char *) data.data, static_cast<size_t>(pixelsLength));
	frame->pixels = pixels;
	release_raw_image_data(&data);
	return frame;
}
