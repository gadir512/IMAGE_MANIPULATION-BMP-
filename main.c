#include "main.h"


int minamongpixel(struct RGB *array,int i){
    int min_number=array[i].blue;
    if(min_number<array[i].green && min_number<array[i].red){
        return min_number;
    }
    else if(array[i].green<array[i].red){
        min_number=array[i].green;
        return min_number; 
    }
    else{
        min_number=array[i].red;
        return min_number;
    }
}
int find_min_pixel(struct RGB *array,int npixel){
    int minumum_number=255;
    for(int i=0;i<npixel;i++){
        if(minumum_number<minamongpixel(array,i)){
            continue;
        }
        else{
            minumum_number=minamongpixel(array,i);
        }
    }
    return minumum_number;
}
int maxamongpixel(struct RGB *array, int i){
    int max_number=array[i].blue;
    if(max_number>array[i].green && max_number>array[i].red){
        return max_number;
    }
    else if(array[i].green>array[i].red){
        max_number=array[i].green;
        return max_number; 
         }
    else{
        max_number=array[i].red;
        return max_number;
    }
}
int find_max_pixel(struct RGB *array, int npixel){
    int maximum_pixel=0;
    for(int i=0;i<npixel;i++){
        if(maximum_pixel>maxamongpixel(array,i)){
            continue;
        }
        else{
            maximum_pixel=maxamongpixel(array,i);
        }
    }
    return maximum_pixel;
}
void change_pixels(int n,struct RGB *array, int npixel,float coef,int minumum_pixel, int maximum_pixel,struct BITMAP_header header, struct DIB_header dibheader){  
    for(int i=0;i<npixel;i++){       
        array[i].blue=round((array[i].blue-minumum_pixel)*coef);   
        array[i].green=round((array[i].green-minumum_pixel)*coef);
        array[i].red=round((array[i].red-minumum_pixel)*coef);               
    }
    FILE *ptr=fopen("new_image.bmp","w");
    fwrite(header.name,2,1,ptr);
    fwrite(&header.size,3*sizeof(int),1,ptr);
    fwrite(&dibheader,sizeof(struct DIB_header),1,ptr);
    fseek(ptr,header.image_offset,SEEK_SET);
    if(n==4){
        for(int i=0;i<npixel;i++){     
        fwrite(&array[i].alpha,1,1,ptr);
        fwrite(&array[i].blue,1,1,ptr);
        fwrite(&array[i].green,1,1,ptr);
        fwrite(&array[i].red,1,1,ptr);
    } 
    } 
    else{
        for(int i=0;i<npixel;i++){     
        fwrite(&array[i].blue,1,1,ptr);
        fwrite(&array[i].green,1,1,ptr);
        fwrite(&array[i].red,1,1,ptr);
    }
    }
}
void openfile(char *filename){
     FILE *file=fopen(filename,"rb"); 
     struct BITMAP_header header;
     struct DIB_header dibheader;
    
     fread(header.name,2,1,file);      //reads first 2 bytes
     fread(&header.size,3*sizeof(int),1,file);      //reads 12 bytes
     fread(&dibheader,sizeof(struct DIB_header),1,file);
     fseek(file,header.image_offset, SEEK_SET);  //this function will move pointer to offset in which pixels will start
     int n=dibheader.bitsperpixels/8;     // to get size in bytes of 1 pixel
     int npixel=dibheader.width*dibheader.height;
     struct RGB *array=malloc(npixel*sizeof(struct RGB));
     int value;      // for getting value of pixel and insertin to array
     if(n==3){
         int memo;
         fread(&value,1,1,file);
         printf("%d\t\n",memo);
     }
     
     if(n==4){
     for(int i=0;i<npixel;i++){
        fread(&value,1,1,file);  
        array[i].alpha=value;
        fread(&value,1,1,file);
        array[i].blue=value;
        fread(&value,1,1,file);
        array[i].green=value;
        fread(&value,1,1,file);
        array[i].red=value;
    }
     }
     else{
         for(int i=0;i<npixel;i++){
        fread(&value,1,1,file);
        array[i].blue=value;
        fread(&value,1,1,file);
        array[i].green=value;
        fread(&value,1,1,file);
        array[i].red=value;
    }
     }
    int minumum_pixel=find_min_pixel(array,npixel);  // minumum pixel-value among all pixels
    int maximum_pixel=find_max_pixel(array,npixel);  //maximum pixel-value among all pixels
    float coef;
    coef=(float)255/(maximum_pixel-minumum_pixel);   // finding coefficient we have to multiple all pixels
    change_pixels(n,array,npixel,coef,minumum_pixel,maximum_pixel,header,dibheader);  // adjusting pixel values  
     fclose(file);
     free(array);   //flushing memory 
}
int main(int argc, char **argv){
    if(argc < 2){
        fputs("Too few arguments\n",stderr);
        printf("ERROR: you must provide a .bmp file name\n");
        return -1;
    }
    else if(argc > 2){
        fputs("Too many arguments\n",stderr);
        printf("Usage: ./autoadjust  filename\n");
        return -1;
    }
    else if(argc == 2 && (!strcmp(argv[1],"-h") || !strcmp(argv[1],"--help"))){
        printf("Usage: ./autoadjust filename\n");
        return -1;
    }
    else if(strlen(argv[1]) > 4 && strcmp(&argv[1][strlen(argv[1])-4],(const char*)&".bmp")){
        fputs("ERROR: you must provide a .bmp file name\n",stderr);
        return -1;
    }
    else{
        openfile(argv[1]);
    }
    return 0;
}