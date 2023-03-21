#ifndef MAIN_H
#define MAIN_H

#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stddef.h>

struct BITMAP_header{
     char name[2];
     unsigned int size;
     int reserved;
     unsigned int image_offset;  
};
struct DIB_header{
     unsigned int header_size;
     unsigned  int width;
     unsigned  int height;
     unsigned short int colornplanes;   //2bytes
     unsigned short int bitsperpixels;  //2bytes
     unsigned int compression;
     unsigned  int image_size;
     unsigned int temp[7];         
};

struct RGB{
        int alpha;
        int blue;
        int green;
        int red;
          
};

int minamongpixel(struct RGB *array,int i);
int find_min_pixel(struct RGB *array,int npixel);
int maxamongpixel(struct RGB *array, int i);
int find_max_pixel(struct RGB *array, int npixel);
void change_pixels(int n,struct RGB *array, int npixel,float coef,int minumum_pixel, int maximum_pixel,struct BITMAP_header header, struct DIB_header dibheader);
void openfile(char *filename);

#endif