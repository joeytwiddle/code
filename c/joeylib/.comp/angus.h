#ifndef angus_H
  #define angus_H

// This was written by Angus Clark
// Slight mods by Joey

void putshort(FILE *fp, int i); // Method


void putint(FILE *fp, int i); // Method



unsigned int getshort(FILE *fp); // Method


//

unsigned int getint(FILE *fp); // Method





RGBmp *angusreadbmp8(char *fn_image); // Method


template <class Object>
void anguswritebmp8(String fn_image,Map2d<Object> *map,float low,float scale); // Method



RGBmp *angusreadbmp24(char *fn_image); // Method



void anguswritebmp24(char *fn_image,RGBmp *byte_rgb_img); // Method


//


#endif
