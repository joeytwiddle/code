/*

Joey's PGM to ASCII converter.
Nov 98.

Deciding the order of the rendering characters is not easy.  I have found
large differences between the intensity of a character at different
screen fonts, in postscript viewers, and once printed.

Therefore, I have included a test option, which displays a palette above
the picture, so you can see if the palette order works well for your display
device.  If not, alter it in the source code or using the palette option.

*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float drand48() {
  return rand()/RAND_MAX;
}

unsigned char **readpgm(char *filename,int *width,int *height);

char *rend=" .,:!+;iot76x0s&8%#@$";
int rendlen,dither=0;

char ditherchar(double i) {
  int rendint;
  double rendpos,rendd;
  
  rendpos=i*(rendlen-1);
  rendint=(int)rendpos;
  if (dither) {
  rendd=rendpos-rendint;
  if (rendd<0.5) {
    if (drand48()>rendd+0.5 && rendint>0)
    rendint--;
  } else {
    if (drand48()>1.5-rendd && rendint<rendlen-1)
      rendint++;
  }
  }
  return rend[rendint];
}

int main(int argc, char **argv) {

  unsigned char **p;
  int height,width,i,j;
  char *infile;
  int arg,c,r,cols=80,rows=80,inverse=0,k,l,x,y;
  double rendpos,intensity,use,gamma=1.0,contrast=1.0,bright=0;
  int test=0,quantise=0,html=0;
  char q;
  
  // Parse command line parameters
  if (argc==1) {
    printf("p2a : grayscale PGM to ASCII picture converter (Joey Nov 98)\n");
    printf("p2a <filename> [ inverse | test | size <wid> <hei> | palette <ascii> |\n");
    printf("                 gamma <0..2> | contrast <0..2> | bright <-1..1> |\n");
    printf("                 quantise | dither | html | > <filename> ]\n");
    exit(0);
  }
  infile=argv[1];
  
  // Read in file
  p=readpgm(infile,&width,&height);
  
  for (arg=2;arg<=argc-1;arg++) {
    if (!strcmp(argv[arg],"inverse"))
      inverse=1;
    if (!strcmp(argv[arg],"test"))
      test=1;
    if (!strcmp(argv[arg],"quantise"))
      quantise=1;
    if (!strcmp(argv[arg],"dither"))
      dither=1;
    if (!strcmp(argv[arg],"html"))
      html=1;
    if (!strcmp(argv[arg],"size")) {
      arg++;
      sscanf(argv[arg],"%i",&cols);
      arg++;
      if (!strcmp(argv[arg],"aspect"))
        rows=cols*height/width;
      else
        sscanf(argv[arg],"%i",&rows);
    }
    if (!strcmp(argv[arg],"palette")) {
      arg++;
      rend=argv[arg];
    }
    if (!strcmp(argv[arg],"gamma")) {
      arg++;
      sscanf(argv[arg],"%lf",&gamma);
    }
    if (!strcmp(argv[arg],"contrast")) {
      arg++;
      sscanf(argv[arg],"%lf",&contrast);
    }
    if (!strcmp(argv[arg],"bright")) {
      arg++;
      sscanf(argv[arg],"%lf",&bright);
    }
  }
  
  rendlen=strlen(rend);
  
  if (test) {
    for (r=0;r<rows/10;r++) {
      for (c=0;c<cols;c++) {
        printf("%c",ditherchar((double)c/cols));
      }
      printf("\n");
    }
  }
    
  if (html)
    printf("<tt><pre>");

  // Print ASCII picture*/
  for (r=0;r<rows;r++) {
  for (c=0;c<cols;c++) {

    // Quantisation*/
    i=r*height/rows;
    j=c*width/cols;
    if (quantise) {
    k=(r+1)*height/rows;
    if (k>=height)
      k=height-1;
    l=(c+1)*width/cols;
    if (l>=width)
      l=width-1;
    intensity=0;
    for (x=i;x<=k;x++) {
    for (y=j;y<=l;y++) {
      intensity=intensity+(double)p[x][y]/255;
    }
    }
    intensity=intensity/(double)((k-i+1)*(l-j+1));
    } else
      intensity=(double)p[i][j]/255;

    // Intensity processing*/
    use=0.5+contrast*(pow(intensity,gamma)-0.5)+bright/2;
    if (use<0)
    use=0;
    if (use>=1)
      use=1;
    if (inverse)
      use=1-use;
    
    // Dither
    q=ditherchar(use);
    
    printf("%c",q);
  
  }
  printf("\n");
  }
  
  if (html)
    printf("</pre></tt>\n");

}

/* From iplib (removed irrelevant outputs with //) */

unsigned char **readpgm(char *filename,int *width,int *height)
{
    unsigned char **p;
    unsigned char *data;
    int i,j,datasize,c,d,gotsize;
    int typeno,maxvalue;
    char dumchar;
    FILE *fp;
    
/*    fprintf(stderr,"readpgm: opening %s\n",filename);*/
    
    fp=fopen(filename,"r");
    if(!fp) {
	fprintf(stderr,"readpgm: error opening file\n");
	exit(0);
    }
    
    if(fscanf(fp,"P%d\n",&typeno)!=1) {
	fprintf(stderr,"readpgm: error reading typeno\n");
	exit(0);
    }
    
/*    fprintf(stderr,"readpgm: ppm type P%d\n",typeno);*/
    
    while(fscanf(fp,"#%c",&dumchar)) {
	while(fscanf(fp,"%c",&dumchar),dumchar!='\n') { } /* printf("%c",dumchar);*/
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
    
/*    fprintf(stderr,"readpgm: w %d  h %d  mv %d\n",*width,*height,maxvalue);*/
    
    datasize=(*width)*(*height)*sizeof(int);
    
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
printf("hei %d wid %d\n",*height,*width);
printf("char %d int %d uchar %d\n",sizeof(char),sizeof(int),sizeof(unsigned char));
//        gotsize=fread(data,1,datasize,fp);
//        gotsize=fread(data,1,datasize,fp);
//        printf("%i,%i\n",gotsize,datasize);
//        printf("%i\n",read(fp,data,datasize));
//exit(0);
/*        if(gotsize!=datasize) {
	    fprintf(stderr,"readppm: error reading data\n");
	    exit(0);
        }*/
	
	c=0;
	for(i=0;i<(*height);i++)
            for(j=0;j<(*width);j++) {
                d=fgetc(fp);
//                fscanf(fp,"%i",&dumchar);
//                d=(int)dumchar;
                p[i][j]=d;
                printf("%i,",d);
//                p[i][j]=data[c++];
//                printf("%i,",p[i][j]);
                }
	
	cfree(data);
	break;
    default:
	fprintf(stderr,"readppm: sorry ppm type not implemented\n");
	exit(0);
    }

    fclose(fp);
    
    return p;
}
	
