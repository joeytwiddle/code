/* 
   image processing mini library
   
   uchar_array - allocate an unsigned char array
   
   adc 11/94
   
   [rehacked 09/02 by stefan for colour support]
   [and ANSIfied it !!                         ]
   
*/
#include <math.h>
#include <stdio.h>
#include "imagproc.h"

/* -----------------------------------------------*/
unsigned char **uchar_array(int width,int height); // Method


/* -----------------------------------------------*/
unsigned char ***uchar_array3D(int width, int height); // Method

