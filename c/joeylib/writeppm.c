/* 
   writepgm.c write out a pgm file
   
   [Adapted from adc's `writepgm' code by stef 9/2 96]
*/

void writeppm(char *filename,RGBmp *p) {
  
  unsigned char *data,*dp;
  int c,i,x,y,datasize,level;
  FILE *fp;
  
  if(!filename){
    fprintf(stderr,"[writeppm] given filename points to NULL. Bad idea.");
    exit(1);
  }
  
  if(!(fp=fopen(filename,"w"))) {
    fprintf(stderr,"[writeppm] error opening output file: %s\n",filename);
    exit(1);
  }
  
  fprintf(fp,"P6\n");
  fprintf(fp,"%d %d\n",p->width,p->height);
  fprintf(fp,"%d\n",255);
  
  datasize=p->width*p->height*3;
  
  if(!(data=(unsigned char *)calloc(sizeof(char),datasize))){
    fprintf(stderr,"[writeppm] can't malloc memory\n");
    exit(1);
  }
  
  dp=data;
  for(y=0;y<p->height;y++)
  for(x=0;x<p->width;x++) {
    *dp++=p->bmp[x][y].r;
    *dp++=p->bmp[x][y].g;
    *dp++=p->bmp[x][y].b;
  }
  
  if(!fwrite(data,sizeof(unsigned char),datasize,fp)) {
    fprintf(stderr,"[writeppm] write error: %s\n",filename);
    exit(1);
  }
  
  free(data);
  
  fclose(fp);
}

