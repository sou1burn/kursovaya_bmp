#pragma once
#ifndef kurs_h
#define kurs_h
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#pragma pack(push, 1)
struct BITMAPFILEHEADER {
	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;
};

struct BITMAPINFOHEADER {
	uint32_t biSize;
	int32_t biWidth;
	int32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	int32_t biXPelsPerMeter;
	int32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
};
#pragma pack(pop)
void resizeBMP(const char* inputFileName, const char* outputFileName);
void downsizeBMP(const char* input_file_name, const char* output_file_name);

#endif