//Real Data formats with base offset
//What is BMP?
/*
  BMP = Bitmap Image File
  A raster graphics image formate used to store digital format
  used to store digital images.
  Think of it like grid of tiny colored dots (pixels). BMP
  stores each dot's color information in order, row by row.
*/

#include <stdio.h>
#include <stdint.h>

#pragma pack(push, 1)
typedef struct{
    uint16_t signature;
    uint32_t file_size;
    uint32_t reserved;
    uint32_t data_offset;
} BMPHEADER;

typedef struct{
    uint32_t header_size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size;
    int32_t x_pixels_per_m;
    int32_t y_pixels_per_m;
    uint32_t colors_used;
    uint32_t important_colors;
} DIBHEADER;
#pragma pack(pop)

void CreateSampleBMP(const char* fileName){
    FILE *fp = fopen(fileName, "wb");
    if(!fp){
        printf("Failed to open file: %s\n", fileName);
        return;
    }
    //step1 : Define image
    int width = 20;
    int height = 20;
    int bytes_per_pixel = 3;

    //Calculate sizes
    int row_size = width * bytes_per_pixel;
    //Each row must be multiple of 4 bytes
    int padding = (4 - (row_size % 4)) % 4;
    int image_size = (row_size + padding) * height;
    int file_size = 54 + image_size; //54 bytes headers = 10 from BMPHEADER + 40 from DIBHEADER

    // Step 2: Fill in headers
    BMPHEADER bmp = {
        .signature = 0x4D42, // BM in little endian
        .file_size = file_size,
        .reserved = 0,
        .data_offset = 54
    };

    DIBHEADER dib = {
        .header_size = 40,
        .width = width,
        .height = height,
        .planes = 1,
        .bits_per_pixel = 24,
        .compression = 0,
        .image_size = image_size,
        /*
          //DPI = Dot per inch but BMP stores pixels Per Meter
          Since 1 inch = 0.0254 meters
          DPI = pixels_per_m * 0.0254
         */
        .x_pixels_per_m = 2835, 
        .y_pixels_per_m = 2835,
        .colors_used = 0,
        .important_colors = 0
    };

    //Step 3: Write headers
    fwrite(&bmp, sizeof(BMPHEADER), 1, fp);
    fwrite(&dib, sizeof(DIBHEADER), 1, fp);

    //Step 4: Create Pattern 
    for(int y = height - 1; y >= 0; y--){
        for(int x = 0; x < width; x++){
            uint8_t pixel[3]; //B, G, R
            
            if(x < 10 && y < 10){
                pixel[0] = 0;
                pixel[1] = 0;
                pixel[2] = 255;
            }
            else if(x >= 10 && y < 10){
                pixel[0] = 0;
                pixel[1] = 255;
                pixel[2] = 0;
            }
            else if(x < 10 && y >= 10){
                pixel[0] = 255;
                pixel[1] = 0;
                pixel[2] = 0;
            }
            else{
                pixel[0] = 255;
                pixel[1] = 255;
                pixel[2] = 255;
            }
            fwrite(pixel, 3, 1, fp);
        }
        //writting padding
        for(int p = 0; p < padding; p++){
            fputc(0, fp);
        }
    }

    fclose(fp);
    printf("Created BMP file: %s\n", fileName);
    printf(" File size: %d bytes\n", file_size);
    printf(" Image: %dx%d, 24-bit color\n", width, height);
    printf(" Pixels start at offset: %d\n", bmp.data_offset);
}

void ReadBmpPixel(const char* fileName, int x, int y){
    FILE *fp = fopen(fileName, "rb");
    if(!fp){
        printf("Failed to open file: %s\n", fileName);
        return;
    }
    //read headers
    BMPHEADER bmp;
    DIBHEADER dib;

    fread(&bmp, sizeof(BMPHEADER), 1, fp);
    fread(&dib, sizeof(DIBHEADER), 1, fp);

    if(x < 0 || x >= dib.width || y < 0 || y >= dib.height){
        printf("Coordinates (%d, %d) out of range!\n", x, y);
        fclose(fp);
        return;
    }
    //Calculate where our pixel is
    int bytes_per_pixel = dib.bits_per_pixel / 8;
    int row_size = dib.width * bytes_per_pixel;
    int padding = (4 - (row_size % 4)) % 4;

    /*
      "BMP files are stored upside down. The First pixel in
      the file is the bottom-left of your image."
     */
    int flipped_y = dib.height - 1 - y;

    /*
      Calculate offset:
      headers + (row * (width_size + padding)) + (x * bytes_per_pixel)
      [Note: In this code: flipped_y is row of formula and row_size is width_size of formula]
     */ 
    long pixel_offset = bmp.data_offset +
        (flipped_y * (row_size + padding)) +
        (x * bytes_per_pixel);

    fseek(fp, pixel_offset, SEEK_SET);
    
    uint8_t blue = fgetc(fp);
    uint8_t green = fgetc(fp);
    uint8_t red = fgetc(fp);

    printf("Pixel at (%d, %d): RGB(%d, %d, %d)\n",
           x, y, red, green, blue);

    fclose(fp);
}

int main(){
    puts("\nBMP");
    CreateSampleBMP("sample.bmp");
    //BMPHEADER bmp;
    DIBHEADER dib;
    FILE *fp = fopen("sample.bmp", "rb");
    if(!fp){
        puts("Failed to open file: sample.bmp");
        return 1;
    }
    //fread(&bmp, sizeof(BMPHEADER), 1, fp);
    fseek(fp, 14, SEEK_SET);
    fread(&dib, sizeof(DIBHEADER), 1, fp);
    int32_t widthNeed = dib.width;
    int32_t heightNeed = dib.height;
    printf("Width = %d\nHeight = %d\n", widthNeed, heightNeed);
    fclose(fp);

    ReadBmpPixel("sample.bmp", 0, 0);
    ReadBmpPixel("sample.bmp", widthNeed - 1, 0);
    ReadBmpPixel("sample.bmp", 0, heightNeed - 1);
    ReadBmpPixel("sample.bmp", widthNeed - 1 , heightNeed - 1);
    
    return 0;
}
