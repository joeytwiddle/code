/* 
   readppm - read in a pgm file to 2D array of unsigned chars
   
   [Adapted from adc's `readpgm' code c by stef 9/2 96]
   
*/
#include <math.h>
#include <stdio.h>
#include "imagproc.h"

unsigned char ***readppm(char *filename,int *width,int *height) {
  
  unsigned char ***p;
  unsigned char *data;
  int x,y,i,datasize,c;
  int typeno,maxvalue;
  char dumchar;
  FILE *fp;
  
  if(!(fp=fopen(filename,"r"))){
    fprintf(stderr,"[readppm] error opening $s\n",filename);
    exit(1);
  }
  
  if(fscanf(fp,"P%d\n",&typeno)!=1) {
    fprintf(stderr,"[readppm] error reading typeno\n");
    exit(1);
  }
  
  while(fscanf(fp,"#%c",&dumchar)) 
  while(fscanf(fp,"%c",&dumchar),dumchar!='\n');
  
  if(fscanf(fp,"%d %d\n",width,height)!=2) {
    fprintf(stderr,"[readppm] error reading width and height\n");
    exit(1);
  }
  
  if(fscanf(fp,"%d",&maxvalue)!=1) {
    fprintf(stderr,"[readppm] error reading maxvalue\n");
    exit(1);
  }
  /* fix 14.10.97: read newline separately to avoid skipping bytes of
     image data corresponding to form-feeds
  */
  fgetc(fp);
  
  datasize=(*width)*(*height)*3;
  
  switch(typeno) {
    case 6:        
    p = uchar_array3D(*width, *height);
    if(!(data=(unsigned char *)calloc(datasize,sizeof(unsigned char)))){
      fprintf(stderr,"[readppm] calloc error 3\n");
      exit(1);
    }
    if(fread(data,sizeof(unsigned char),datasize,fp)!=datasize) {
      fprintf(stderr,"[readppm] error reading data\n");
      exit(0);
    }
    
    c=0;
    for(y=0;y<(*height);y++)
    for(x=0;x<(*width);x++)
    for(i=0;i<3;i++)
    p[y][x][i]=data[c++];
    
    free(data);
    break;
    
    default:
    fprintf(stderr,"[readppm] sorry ppm type not implemented\n");
    exit(1);
  }
  
  fclose(fp);
  
  return p;
}

