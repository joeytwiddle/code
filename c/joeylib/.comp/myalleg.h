#ifndef myalleg_H
  #define myalleg_H

  #include <myrgb.h>

extern int allegrostarted; // Variable initialised in .c file

void allegrosetup(int scrwid,int scrhei,int cd); // Method


void allegrosetup(int scrwid,int scrhei); // Method


void allegroexit(); // Method


#ifndef ALLEGRO
class RGB {
public:
  uchar r,g,b; // Exists
};
#endif

RGB mynewRGB(float r,float g,float b); // Method


RGB greypalette(float f); // Method


RGB redpalette(float f); // Method


RGB greenpalette(float f); // Method


RGB redtowhitepalette(float f); // Method


RGB greentowhitepalette(float f); // Method


void makepalette(RGB (*fn)(float)); // Method


void makesplitpalette(RGB (*fna)(float),RGB (*fnb)(float)); // Method


void makepaletterange(int a,int b,RGB c,RGB d); // Method


void makestereopalette(); // Method


void mypalette(int c,float r,float g,float b); // Method


void mypalette(int c,myRGB m); // Method


void makepalette(myRGB (*fn)(float)); // Method
 

// - eg. makepalette(&myRGB::hue);

void savescreen(String fname); // Method


void psychedelicpalette(); // Method


RGB newallegRGB(float x,float y,float z); // Method


RGB newallegRGB(myRGB r); // Method


RGB *makegreyallegpalette(); // Method


void waitforkeypress(); // Method


#endif
