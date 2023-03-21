#ifndef AUTOADJUST_H
#define AUTOADJUST_H

#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stddef.h>

struct BITMAP_header{        // bitmap header file for information about bmp file
     char name[2];
     unsigned int size;
     int reserved;        // unimported information
     unsigned int image_offset;  
};
struct DIB_header{            // technical details about bmp file
     unsigned int header_size;
     unsigned  int width,height;
     unsigned short int colornplanes;   //2bytes
     unsigned short int bitsperpixels;  //2bytes
     unsigned int compression;
     unsigned  int image_size;
     unsigned int temp[7];           //unimportant information 
};

struct RGB{      // this will hold rgba values of each pixels
        int alpha,blue,green,red;      
};

int minamongpixel(struct RGB *array,int i);
int find_min_pixel(struct RGB *array,int npixel);
int maxamongpixel(struct RGB *array, int i);
int find_max_pixel(struct RGB *array, int npixel);
void change_pixels(int n,struct RGB *array, int npixel,float coef,int minumum_pixel, int maximum_pixel,struct BITMAP_header header, struct DIB_header dibheader,char*filename);
void openfile(char *filenamesrc,char *filenamedest);

#endif