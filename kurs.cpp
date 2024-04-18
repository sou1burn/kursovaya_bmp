#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "kurs.h"

void resizeBMP(const char* input_file_name, const char* output_file_name) {

    FILE* input_file = fopen(input_file_name, "rb");

    if (!input_file) {
        perror("Error opening input file");
        return;
    }

    FILE* output_file = fopen(output_file_name, "wb");

    if (!output_file) {
        perror("Error creating output file");
        fclose(input_file);
        return;
    }

    struct BITMAPFILEHEADER fileHeader;
    struct BITMAPINFOHEADER infoHeader;

    fread(&fileHeader, sizeof(struct BITMAPFILEHEADER), 1, input_file);
    fread(&infoHeader, sizeof(struct BITMAPINFOHEADER), 1, input_file);

    if (fileHeader.bfType != 0x4D42) {
        fprintf(stderr, "Not a BMP file \n");
        fclose(input_file);
        fclose(output_file);
        return;
    }

    int original_width = infoHeader.biWidth;
    int original_height = infoHeader.biHeight;
    int new_width = original_width * 2;
    int new_height = original_height * 2;

    infoHeader.biWidth = new_width;
    infoHeader.biHeight = new_height;
    infoHeader.biSizeImage = new_width * new_height * 3;

    fwrite(&fileHeader, sizeof(struct BITMAPFILEHEADER), 1, output_file);
    fwrite(&infoHeader, sizeof(struct BITMAPINFOHEADER), 1, output_file);

    uint8_t* pixel_data = (uint8_t*)malloc(original_width * original_height * 3);
    if (!pixel_data) {
        perror("Error allocating memory for pixel_data");
        fclose(input_file);
        fclose(output_file);
        return;
    }

    fread(pixel_data, 1, original_width * original_height * 3, input_file);

    uint8_t* new_pixel_data = (uint8_t*)malloc(infoHeader.biSizeImage);
    if (!new_pixel_data) {
        perror("Error allocating memory for new_pixel_data");
        free(pixel_data);
        fclose(input_file);
        fclose(output_file);
        return;
    }
    /*
    for (uint32_t i = 0; i < srcHeader.imageHeight; ++i) {
        for (uint32_t j = 0; j < srcHeader.imageWidth; ++j) {
            dstData[i * 2][j * 6] = srcData[i][j * 3];
            dstData[i * 2][j * 6 + 1] = srcData[i][j * 3 + 1];
            dstData[i * 2][j * 6 + 2] = srcData[i][j * 3 + 2];

            if (j < srcHeader.imageWidth - 1) {
                dstData[i * 2][j * 6 + 3] = (srcData[i][j * 3] + srcData[i][j * 3 + 3]) / 2;
                dstData[i * 2][j * 6 + 4] = (srcData[i][j * 3 + 1] + srcData[i][j * 3 + 4]) / 2;
                dstData[i * 2][j * 6 + 5] = (srcData[i][j * 3 + 2] + srcData[i][j * 3 + 5]) / 2;
            }

            if (i < srcHeader.imageHeight - 1) {
                for (uint32_t k = 0; k < 6; ++k) {
                    dstData[i * 2 + 1][j * 6 + k] = (dstData[i * 2][j * 6 + k] + dstData[i * 2 + 2][j * 6 + k]) / 2;
                }
            }
        }
    }*/

    for (uint32_t y = 0; y < original_height; ++y) {
        for (uint32_t x = 0; x < original_width; ++x) {
            int new_x = x * 2;
            int new_y = y * 2;

            for (int c = 0; c < 3; ++c) {
                new_pixel_data[((new_y)*new_width + new_x) * 3 + c] = pixel_data[(y * original_width + x) * 3 + c];

                new_pixel_data[((new_y)*new_width + new_x + 1) * 3 + c] = (pixel_data[(y * original_width + x) * 3 + c]
                    + pixel_data[(y * original_width + (x + 1)) * 3 + c]) / 2;

                new_pixel_data[((new_y + 1) * new_width + new_x) * 3 + c] = (pixel_data[(y * original_width + x) * 3 + c]
                    + pixel_data[((y + 1) * original_width + x) * 3 + c]) / 2;

                new_pixel_data[((new_y + 1) * new_width + new_x + 1) * 3 + c] = (pixel_data[(y * original_width + x) * 3 + c]
                    + pixel_data[((y + 1) * original_width + (x + 1)) * 3 + c]) / 2;
            }
        }
    }

    fwrite(new_pixel_data, 1, infoHeader.biSizeImage, output_file);

    free(pixel_data);
    free(new_pixel_data);

    fclose(input_file);
    fclose(output_file);

    printf("BMP resized");
}

void downsizeBMP(const char* input_file_name, const char* output_file_name) {

    FILE* input_file = fopen(input_file_name, "rb");

    if (!input_file) {
        perror("Error opening input file");
        return;
    }

    FILE* output_file = fopen(output_file_name, "wb");

    if (!output_file) {
        perror("Error creating output file");
        fclose(input_file);
        return;
    }

    struct BITMAPFILEHEADER fileHeader;
    struct BITMAPINFOHEADER infoHeader;

    fread(&fileHeader, sizeof(struct BITMAPFILEHEADER), 1, input_file);
    fread(&infoHeader, sizeof(struct BITMAPINFOHEADER), 1, input_file);

    if (fileHeader.bfType != 0x4D42) {
        fprintf(stderr, "Not a BMP file \n");
        fclose(input_file);
        fclose(output_file);
        return;
    }

    int original_width = infoHeader.biWidth;
    int original_height = infoHeader.biHeight;
    int new_width = original_width / 2;
    int new_height = original_height / 2;

    infoHeader.biWidth = new_width;
    infoHeader.biHeight = new_height;

    infoHeader.biSizeImage = new_width * new_height * 3;

    fwrite(&fileHeader, sizeof(struct BITMAPFILEHEADER), 1, output_file);
    fwrite(&infoHeader, sizeof(struct BITMAPINFOHEADER), 1, output_file);

    uint8_t* pixel_data = (uint8_t*)malloc(original_width * original_height * 3);
    if (!pixel_data) {
        perror("Error allocating memory for pixel_data");
        fclose(input_file);
        fclose(output_file);
        return;
    }

    fread(pixel_data, 1, original_width * original_height * 3, input_file);

    uint8_t* new_pixel_data = (uint8_t*)malloc(infoHeader.biSizeImage);
    if (!new_pixel_data) {
        perror("Error allocating memory for new_pixel_data");
        free(pixel_data);
        fclose(input_file);
        fclose(output_file);
        return;
    }

    for (int y = 0; y < new_height; ++y) {
        for (int x = 0; x < new_width; ++x) {
            int original_x = x * 2;
            int original_y = y * 2;

            for (int c = 0; c < 3; ++c) {
                new_pixel_data[(y * new_width + x) * 3 + c] = (
                    pixel_data[(original_y * original_width + original_x) * 3 + c] +
                    pixel_data[((original_y + 1) * original_width + original_x) * 3 + c] +
                    pixel_data[(original_y * original_width + (original_x + 1)) * 3 + c] +
                    pixel_data[((original_y + 1) * original_width + (original_x + 1)) * 3 + c]
                    ) / 4;
            }
        }
    }

    fwrite(new_pixel_data, 1, infoHeader.biSizeImage, output_file);

    free(pixel_data);
    free(new_pixel_data);

    fclose(input_file);
    fclose(output_file);

    printf("\nBMP downsized\n");
}
