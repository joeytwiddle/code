/* 
   writepgm.c write out a pgm file
   
   10/94 cardiff
   
   [ANSI c by stef 9/2 96]
*/

#include <math.h>
#include <stdio.h>
#include <string.h>

void writepgm(unsigned char **p, int width, int height, char *filename) {
  
  unsigned char *data,*dp;
  int c,i,j,datasize,level;
  FILE *fp;
  
  if(!(fp=fopen(filename,"w"))) {
    fprintf(stderr,"writepgm: error opening output file\n");
    exit(0);
  }
  
  fprintf(fp,"P5\n");
  
  fprintf(fp,"%d %d\n",width,height);
  
  fprintf(fp,"%d\n",255);
  
  datasize=width*height;
  
  if(!(data=(unsigned char *)calloc(sizeof(char),datasize))){
    fprintf(stderr,"writepgm: calloc error\n");
    exit(0);
  }
  
  dp=data;
  for(i=0;i<height;i++)
  for(j=0;j<width;j++)
  *dp++=p[i][j];
  
  if(!fwrite(data,sizeof(unsigned char),datasize,fp)) {
    fprintf(stderr,"writepgm: write error\n");
    exit(0);
  }
  
  cfree(data);
  
  fclose(fp);
}

