//
// Created by ldr on 2020/7/7.
//

#ifndef SUPERPLAYER_PNG_UTILS_H
#define SUPERPLAYER_PNG_UTILS_H

#include "../thirdpart/libpng/png.h"
#include "../thirdpart/libpng/pnginfo.h"
#include <stdio.h>

#define PNG_BYTES_TO_CHECK 4

struct ImageInfo {
    png_byte *pixel;
    png_size_t w;
    png_size_t h;

};

ImageInfo decodePng(const char *path) {
    ImageInfo info = {};
    FILE *fp = fopen(path, "rb");
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, nullptr, nullptr);
        fclose(fp);
        return info;
    }

    char buf[PNG_BYTES_TO_CHECK];
    // 读取 buffer
    if (fread(buf, 1, PNG_BYTES_TO_CHECK, fp) != PNG_BYTES_TO_CHECK) {
        fclose(fp);
        return info;
    }
    // 判断
    if (!png_sig_cmp(reinterpret_cast<png_const_bytep>(buf), 0, PNG_BYTES_TO_CHECK)) {
        // 返回值不等于 0 则是 png 文件格式
    }

    png_infop infop = png_create_info_struct(png);

    // 设置图像数据源
    png_init_io(png, fp);
    png_read_info(png, infop);

    png_read_png(png, infop, (PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND), nullptr);
    png_uint_32 w;
    png_uint_32 h;
    int format;
    int bit_depth;

    png_get_IHDR(png, infop, &w, &h, &bit_depth, &format, nullptr, nullptr, nullptr);

    if (format == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);//要求转换索引颜色到RGB
    if (format == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);//要求位深度强制8bit
    if (bit_depth == 16)
        png_set_strip_16(png);//要求位深度强制8bit
    if (png_get_valid(png, infop, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);
    if (format == PNG_COLOR_TYPE_GRAY || format == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);//灰度必须转换成RGB

//    png_bytep *row_pointers;
//    row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * info.h);
//    for (int y = 0; y < info.h; y++) {
//        row_pointers[y] = (png_byte *) malloc(png_get_rowbytes(png, infop));
//    }
//    png_read_image(png, row_pointers);

    const png_size_t row_size = png_get_rowbytes(png, infop);
    const int data_length = row_size * infop->height;
    info.pixel = (png_byte *) malloc(data_length);

    png_byte *row_ptrs[infop->height];

    info.w = row_size;
    info.h = infop->height;

    png_uint_32 i;
    for (i = 0; i < infop->height; i++) {
        row_ptrs[i] = info.pixel + i * row_size;
    }

    png_read_image(png, &row_ptrs[0]);

    png_read_end(png, infop);

    png_destroy_read_struct(&png, &infop, nullptr);

    fclose(fp);
    return info;
}

#endif //SUPERPLAYER_PNG_UTILS_H
