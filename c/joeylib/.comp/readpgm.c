#ifndef readpgm_C
  #define readpgm_C

  #include <readpgm.h>

/* 
   readpgm - read in a pgm file to 2D array of unsigned chars
   
   cardiff 10/94
   
   [ANSI c by stef 9/2 96]
   
*/
#include <math.h>
#include <stdio.h>

unsigned char **readpgm(char *filename,int *width,int *height) {
  
  unsigned char **p;
  unsigned char *data;
  int i,j,datasize,c;
  int typeno,maxvalue;
  char dumchar;
  FILE *fp;
  
  fprintf(stderr,"readpgm: opening %s\n",filename);
  
  fp=fopen(filename,"r");
  if(!fp) {
    fprintf(stderr,"readpgm: error opening file\n");
    exit(0);
  }
  
  if(fscanf(fp,"P%d\n",&typeno)!=1) {
    fprintf(stderr,"readpgm: error reading typeno\n");
    exit(0);
  }
  
  fprintf(stderr,"readpgm: ppm type P%d\n",typeno);
  
  while(fscanf(fp,"#%c",&dumchar)) {
    while(fscanf(fp,"%c",&dumchar),dumchar!='\n') printf("%c",dumchar);
    printf("\n");
  }
  
  if(fscanf(fp,"%d %d\n",width,height)!=2) {
    fprintf(stderr,"readpgm: error reading width,height\n");
    exit(0);
  }
  
  if(fscanf(fp,"%d",&maxvalue)!=1) {
    fprintf(stderr,"readpgm: error reading maxvalue\n");
    exit(0);
  }
  /* fix 14.10.97: read newline separately to avoid skipping bytes of
     image data corresponding to form-feeds
  */
  fgetc(fp);
  
  fprintf(stderr,"readpgm: w %d  h %d  mv %d\n",*width,*height,maxvalue);
  
  datasize=(*width)*(*height);
  
  switch(typeno) {
    
    case 5:        /*  rawbits pgm type */
    
    p=(unsigned char **)calloc((*height),sizeof(unsigned char *));
    if(!p) {
      fprintf(stderr,"readppm: calloc error 1\n");
      exit(0);
    }
    for(i=0;i<(*height);i++) {
      p[i]=(unsigned char *)calloc((*width),sizeof(unsigned char));
      if(!p[i]) {
        fprintf(stderr,"readppm: calloc error 2\n");
        exit(0);
      }
    }
    data=(unsigned char *)calloc(datasize,sizeof(unsigned char));
    if(!data) {
      fprintf(stderr,"readppm: calloc error 3\n");
      exit(0);
    }
    if(fread(data,sizeof(unsigned char),datasize,fp)!=datasize) {
      fprintf(stderr,"readppm: error reading data\n");
      exit(0);
    }
    
    c=0;
    for(i=0;i<(*height);i++)
    for(j=0;j<(*width);j++)
    p[i][j]=data[c++];
    
    cfree(data);
    break;
    default:
    fprintf(stderr,"readppm: sorry ppm type not implemented\n");
    exit(0);
  }
  
  fclose(fp);
  
  return p;
}


#endif
