#include "autoadjust.h"


int minamongpixel(struct RGB *array,int i){   // return minumum pixel value in between (rgb) in 1 pixel
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
int find_min_pixel(struct RGB *array,int npixel){      // return min value pixel among all pixels
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
int maxamongpixel(struct RGB *array, int i){     // return max value pixel in between (rgb) in  pixel
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
int find_max_pixel(struct RGB *array, int npixel){   // return max value pixel among ll pixels
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
void change_pixels(int n,struct RGB *array, int npixel,float coef,int minumum_pixel, int maximum_pixel,struct BITMAP_header header, struct DIB_header dibheader,char*filename){  
    for(int i=0;i<npixel;i++){                 //changing value of each pixel  
        array[i].blue=round((array[i].blue-minumum_pixel)*coef);   
        array[i].green=round((array[i].green-minumum_pixel)*coef);
        array[i].red=round((array[i].red-minumum_pixel)*coef);               
    }
    FILE *ptr=fopen(filename,"w");        // open new file for output bmp file
    fwrite(header.name,2,1,ptr);                //writing header information into new file  
    fwrite(&header.size,3*sizeof(int),1,ptr);
    fwrite(&dibheader,sizeof(struct DIB_header),1,ptr);
    fseek(ptr,header.image_offset,SEEK_SET);   // moving pointer to offset point in which pixel starts
    if(n==4){                 //writing pixel information for 32 byte bmp file 
        for(int i=0;i<npixel;i++){     
        fwrite(&array[i].alpha,1,1,ptr);
        fwrite(&array[i].blue,1,1,ptr);
        fwrite(&array[i].green,1,1,ptr);
        fwrite(&array[i].red,1,1,ptr);
    } 
    } 
    else{                          //for 24 byte bmp file
        for(int i=0;i<npixel;i++){     
        fwrite(&array[i].blue,1,1,ptr);
        fwrite(&array[i].green,1,1,ptr);
        fwrite(&array[i].red,1,1,ptr);
    }
    }
}
void openfile(char *filenamesrc,char *filenamedest){
     FILE *file=fopen(filenamesrc,"rb"); 
     struct BITMAP_header header;
     struct DIB_header dibheader;
    
     fread(header.name,2,1,file);      //reads first 2 bytes  which is (BM)
     fread(&header.size,3*sizeof(int),1,file);      //reads 12 bytes
     fread(&dibheader,sizeof(struct DIB_header),1,file);
     fseek(file,header.image_offset, SEEK_SET);  //this function will move pointer to offset in which pixels will start
     int n=dibheader.bitsperpixels/8;     // to get size in bytes of 1 pixel
     int npixel=dibheader.width*dibheader.height;
     struct RGB *array=malloc(npixel*sizeof(struct RGB));
     int value;      // for getting value of pixel and inserting to array
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
    float coef;    // for getting 255/(maxpixel-minpixel)
    coef=(float)255/(maximum_pixel-minumum_pixel);   // finding coefficient we have to multiple all pixels
    change_pixels(n,array,npixel,coef,minumum_pixel,maximum_pixel,header,dibheader,filenamedest);  // adjusting pixel values  
     fclose(file);
     free(array);   //flushing memory 
}
int main(int argc, char **argv){
    if(argc == 4){
        if(strstr(argv[1],".bmp") && strstr(argv[3],".bmp")){
            openfile(argv[1],argv[3]);
        }
        else{
            printf("ERROR : you must provide .bmp file name\n");
            printf("Usage: %s sourceFileName.bmp -o destinationFileName.bmp\n",argv[0]);
        }
    }
    else if(argc > 4){
        printf("ERROR: Too many arguments\n");
        printf("Usage: %s sourceFileName.bmp -o destinationFileName.bmp\n",argv[0]);
    }
    else{
        printf("ERROR: Too few arguments\n");
        printf("Usage: %s sourceFileName.bmp -o destinationFileName.bmp\n",argv[0]);
    }
    return 0;
}