#ifndef angus_C
  #define angus_C

  #include <angus.h>

// This was written by Angus Clark
// Slight mods by Joey

void putshort(FILE *fp, int i) {

  int c, c1;

  c = ((unsigned int ) i) & 0xff;  c1 = (((unsigned int) i)>>8) & 0xff;
  putc(c, fp);   putc(c1,fp);
}

void putint(FILE *fp, int i) {

  int c, c1, c2, c3;
  c  = ((unsigned int ) i)      & 0xff;
  c1 = (((unsigned int) i)>>8)  & 0xff;
  c2 = (((unsigned int) i)>>16) & 0xff;
  c3 = (((unsigned int) i)>>24) & 0xff;

  putc(c, fp);   putc(c1,fp);  putc(c2,fp);  putc(c3,fp);
}


unsigned int getshort(FILE *fp) {

  int c, c1;
  c = getc(fp);  c1 = getc(fp);
  return ((unsigned int) c) + (((unsigned int) c1) << 8);
}

/* ******************************************/
unsigned int getint(FILE *fp) {

  int c, c1, c2, c3;
  c = getc(fp);  c1 = getc(fp);  c2 = getc(fp);  c3 = getc(fp);
  return ((unsigned int) c) +
  (((unsigned int) c1) << 8) +
  (((unsigned int) c2) << 16) +
  (((unsigned int) c3) << 24);
}




RGBmp *angusreadbmp8(char *fn_image) {


  FILE *fp;
  int fp_type;

  int xdim, ydim;

  int          i, c, c1, rv, j, padb;
  unsigned int bfSize, bfOffBits, biSize, biWidth, biHeight, biPlanes;
  unsigned int biBitCount, biCompression, biSizeImage, biXPelsPerMeter;
  unsigned int biYPelsPerMeter, biClrUsed, biClrImportant;
  int cmaplen, mono_cmap;

  RGBmp *rgbmp;


  /* Open file */
  // fp = openImageIStream(fn_image, &fp_type);
  fp = fopen(fn_image,"rb");


  /* read the file type (first two bytes) */
  c = getc(fp);  c1 = getc(fp);
  if (c!='B' || c1!='M') {
    // fprintf(stderr, "[readBMP8] file type != 'BM'\n");
    return NULL;
  }

  bfSize = getint(fp);
  getshort(fp);         /* reserved and ignored */
  getshort(fp);
  bfOffBits = getint(fp);

  biSize          = getint(fp);
  biWidth         = getint(fp);
  biHeight        = getint(fp);
  biPlanes        = getshort(fp);
  biBitCount      = getshort(fp);
  biCompression   = getint(fp);
  biSizeImage     = getint(fp);
  biXPelsPerMeter = getint(fp);
  biYPelsPerMeter = getint(fp);
  biClrUsed       = getint(fp);
  biClrImportant  = getint(fp);

  xdim = biWidth;
  ydim = biHeight;

  if (ferror(fp)) {
    fprintf(stderr, "[readBMP8] EOF reached in file header\n");
    return NULL;
  }

  /* error checking */
  if (biBitCount!= 8) {
    // fprintf(stderr, "[readBMP8] BMP type(%d) not supported\n", biBitCount);
    return NULL;
  }

  rgbmp = new RGBmp(xdim, ydim);

  /* skip ahead to colormap, using biSize */

  c = biSize - 40;    /* 40 bytes read from biSize to biClrImportant */
  for (i=0; i<c; i++) getc(fp);

  /* cmaplen = 1 << biBitCount;*/
  cmaplen = (bfOffBits - 54)/4;

  int *cmap_r=new int[cmaplen];
  int *cmap_g=new int[cmaplen];
  int *cmap_b=new int[cmaplen];

	ProgMon progmon;
  for (i=0; i<cmaplen; i++) {
    cmap_b[i] = getc(fp);
    cmap_g[i] = getc(fp);
    cmap_r[i] = getc(fp);
    // printf("Colour %i %i %i\n",cmap_r[i],cmap_g[i],cmap_b[i]);

    getc(fp);         /* unused */

		// progmon.nowthrough((float)i/(float)cmaplen);
		if (i % (int)(cmaplen/100) == 0)
		  (progmon.*progmon.nowthrough)((float)i/(float)cmaplen);
  }
	// progmon.end();
	(progmon.*progmon.end)();


  /* check to see if cmap is monochrome */
  mono_cmap = 1;
  for (i = 0; i < cmaplen; i++) {
    if (cmap_b[i] != cmap_g[i] ||
      cmap_b[i] != cmap_r[i]) {
      mono_cmap = 0;
      break;
    }
  }



  fseek(fp, bfOffBits, 0);

  padb = (xdim*3)%4; /* # of pad bytes to read at EOscanline*/


  if (mono_cmap == 0) {
    for (i=(ydim)-1; i >= 0; i--) {
      for (j = 0; j < xdim; j++) {
        int c=(int)getc(fp);
        // if (c<0 || c>cmaplen)
        // error("File character %i out of cmap range 0-%i",c,cmaplen);
        c=intchop(c,0,cmaplen-1);
        // printf("%i ",(int)c);
        rgbmp->bmp[j][i] = myRGB(cmap_r[c],cmap_g[c],cmap_b[c]);
      }
      for (j=0; j<padb; j++) getc(fp);
      if (ferror(fp)) {
        fprintf(stderr, "[readBMP] file truncated\n");
        return NULL;
      }

    }
  }
  else {
    for (i=(ydim)-1; i >= 0; i--) {
      for (j = 0; j < xdim; j++) {
        int c=cmap_b[getc(fp)];
        rgbmp->bmp[j][i] = myRGB(c,c,c);
      }
      for (j=0; j<padb; j++) getc(fp);
      if (ferror(fp)) {
        fprintf(stderr, "[readBMP] file truncated\n");
        return NULL;
      }
    }
    // freeByteImageCMap(byte_img);
  }

  // closeImageStream(fp, fp_type);
  fclose(fp);

  // return(byte_img);
  return rgbmp;

}

template <class Object>
void anguswritebmp8(String fn_image,Map2d<Object> *map,float low,float scale) {

  FILE *fp;
  int fp_type;
  int bperlin;
  int   i,j,c,padb;
  int xdim, ydim;

  fp = fopen(fn_image,"wb");

  xdim = map->width;
  ydim = map->height;

  bperlin = ((xdim * 8 + 31) / 32) *4;   // # bytes written per line //
  putc('B', fp);  putc('M', fp);           // BMP file magic number //

  // compute filesize and write it //
  i = 14 +                     // size of bitmap file header //
  40 +                  // size of bitmap info header //
  (256 * 4) +   // cmaplength //
  bperlin * ydim;       // size of image data //
  putint(fp, i);

  putshort(fp, 0);         // reserved1 //
  putshort(fp, 0);         // reserved2 //
  putint(fp, 14 + 40 + (256 *4));// offset from BOfile to BObitmap //

  putint(fp, 40);          // biSize: size of bitmap info header //
  putint(fp, xdim);            // biWidth //
  putint(fp, ydim);            // biHeight //
  putshort(fp, 1);         // biPlanes:  must be '1' //
  putshort(fp, 8);       // biBitCount: 1,4,8, or 24 //
  putint(fp, 0);         // biCompression:  BI_RGB, BI_RLE8 or BI_RLE4 //
  putint(fp, bperlin*ydim);    // biSizeImage:  size of raw image data //
  putint(fp, 0);       // biXPelsPerMeter: (75dpi * 39" per meter) //
  putint(fp, 0);       // biYPelsPerMeter: (75dpi * 39" per meter) //
  putint(fp, 0);           // biClrUsed: # of colors used in cmap //
  putint(fp, 0);           // biClrImportant: same as above //

  for (i = 0; i < 256; i++) {
    int c=ucharchop(i);
    putc(c,fp); // blue
    putc(c,fp); // green
    putc(c,fp); // red
    putc(0,fp); // eh?!
  }

  // hmmm, seems wrong! //
  padb = (xdim*3)%4; // # of pad bytes to read at EOscanline//

  for (i = ydim-1; i >= 0; i--) {
    for (j = 0; j < xdim; j++) {
      putc(ucharchop((tofloat(map->pos[j][i])-low)*scale), fp);
    }

    for (j=0; j<padb; j++)
    putc(0, fp);
  }


  fclose(fp);

}


RGBmp *angusreadbmp24(char *fn_image) {

  FILE *fp;
  int fp_type;
  int          i, c, c1, rv, j, padb;
  unsigned int bfSize, bfOffBits, biSize, biWidth, biHeight, biPlanes;
  unsigned int biBitCount, biCompression, biSizeImage, biXPelsPerMeter;
  unsigned int biYPelsPerMeter, biClrUsed, biClrImportant;
  int xdim, ydim;


  RGBmp *rgbmp;

  // Open file //
  fp = fopen(fn_image, "rb");

  // read the file type (first two bytes) //
  c = getc(fp);  c1 = getc(fp);
  if (c!='B' || c1!='M') {
    // fprintf(stderr, "[readBMP24] file type != 'BM'\n");
    return NULL;
  }

  bfSize = getint(fp);
  getshort(fp);         // reserved and ignored //
  getshort(fp);
  bfOffBits = getint(fp);

  biSize          = getint(fp);
  biWidth         = getint(fp);
  biHeight        = getint(fp);
  biPlanes        = getshort(fp);
  biBitCount      = getshort(fp);
  biCompression   = getint(fp);
  biSizeImage     = getint(fp);
  biXPelsPerMeter = getint(fp);
  biYPelsPerMeter = getint(fp);
  biClrUsed       = getint(fp);
  biClrImportant  = getint(fp);

  xdim = biWidth; ydim = biHeight;

  if (ferror(fp)) {
    fprintf(stderr, "[readBMP] EOF reached in file header\n");
    return NULL;
  }

  // error checking //
  if (biBitCount!=24) {
    // fprintf(stderr, "[readBMP24] BMP type(%d) not supported\n", biBitCount);
    return NULL;
  }

  rgbmp = new RGBmp(xdim, ydim);

  // skip ahead to colormap, using biSize //
  c = biSize - 40;    // 40 bytes read from biSize to biClrImportant //
  for (i=0; i<c; i++) getc(fp);

  padb = (xdim*9)%4;  // # of pad bytes to read at EOscanline//
  for (i=(ydim)-1; i >= 0; i--) {
    for (j = 0; j < xdim; j++) {
      int b=getc(fp);
      int g=getc(fp);
      int r=getc(fp);
      rgbmp->bmp[j][i]=myRGB(r,g,b);
    }
    for (j=0; j<padb; j++) getc(fp);
    if (ferror(fp)) {
      fprintf(stderr, "[readBMP] file truncated\n");
      return NULL;
    }

  }
  fclose(fp);

  return rgbmp;

}


void anguswritebmp24(char *fn_image,RGBmp *byte_rgb_img) {

  FILE *fp;
  int fp_type;
  int bperlin;
  int   i,j,c,padb;
  int xdim, ydim;

  fp = fopen(fn_image,"wb");

  xdim = byte_rgb_img->width;
  ydim = byte_rgb_img->height;

  bperlin = ((xdim * 24 + 31) / 32) * 4;   // # bytes written per line //
  putc('B', fp);  putc('M', fp);           // BMP file magic number //

  // compute filesize and write it //
  i = 14 +                     // size of bitmap file header //
  40 +                  // size of bitmap info header //
  bperlin * ydim;         // size of image data //
  putint(fp, i);

  putshort(fp, 0);         // reserved1 //
  putshort(fp, 0);         // reserved2 //
  putint(fp, 14 + 40);   // offset from BOfile to BObitmap //

  putint(fp, 40);          // biSize: size of bitmap info header //
  putint(fp, xdim);            // biWidth //
  putint(fp, ydim);            // biHeight //
  putshort(fp, 1);         // biPlanes:  must be '1' //
  putshort(fp, 24);        // biBitCount: 1,4,8, or 24 //
  putint(fp, 0);         // biCompression:  BI_RGB, BI_RLE8 or BI_RLE4 //
  putint(fp, bperlin*ydim);    // biSizeImage:  size of raw image data //
  putint(fp, 0);       // biXPelsPerMeter: (75dpi * 39" per meter) //
  putint(fp, 0);       // biYPelsPerMeter: (75dpi * 39" per meter) //
  putint(fp, 0);           // biClrUsed: # of colors used in cmap //
  putint(fp, 0);           // biClrImportant: same as above //

  padb = (xdim*9)%4;  // # of pad bytes to read at EOscanline//


  for (i = ydim-1; i >= 0; i--) {
    for (j = 0; j < xdim; j++) {
      myRGB r=byte_rgb_img->bmp[j][i];
      putc(r.b, fp);
      putc(r.g, fp);
      putc(r.r, fp);
    }

    for (j=0; j<padb; j++) putc(0, fp);
  }
  fclose(fp);

}

/*
   void writeBMP24_ByteImage(ByteImage *byte_img, char *fn_image) {

     FILE *fp;
     int fp_type;
     int bperlin;
     int   i,j,c,padb;
     int xdim, ydim;

     fp = openImageOStream(fn_image, &fp_type);

     xdim = byte_img->xdim;
     ydim = byte_img->ydim;

     bperlin = ((xdim * 24 + 31) / 32) * 4;   // # bytes written per line //
     putc('B', fp);  putc('M', fp);           // BMP file magic number //

     // compute filesize and write it //
     i = 14 +                     // size of bitmap file header //
     40 +                  // size of bitmap info header //
     bperlin * ydim;         // size of image data //
     putint(fp, i);

     putshort(fp, 0);         // reserved1 //
     putshort(fp, 0);         // reserved2 //
     putint(fp, 14 + 40);   // offset from BOfile to BObitmap //

     putint(fp, 40);          // biSize: size of bitmap info header //
     putint(fp, xdim);            // biWidth //
     putint(fp, ydim);            // biHeight //
     putshort(fp, 1);         // biPlanes:  must be '1' //
     putshort(fp, 24);        // biBitCount: 1,4,8, or 24 //
     putint(fp, 0);         // biCompression:  BI_RGB, BI_RLE8 or BI_RLE4 //
     putint(fp, bperlin*ydim);    // biSizeImage:  size of raw image data //
     putint(fp, 0);       // biXPelsPerMeter: (75dpi * 39" per meter) //
     putint(fp, 0);       // biYPelsPerMeter: (75dpi * 39" per meter) //
     putint(fp, 0);           // biClrUsed: # of colors used in cmap //
     putint(fp, 0);           // biClrImportant: same as above //

     padb = (xdim*9)%4;  // # of pad bytes to write at EOscanline //


     if (byte_img->has_cmap == 1) {
       for (i = ydim-1; i >= 0; i--) {
         for (j = 0; j < xdim; j++) {
           putc(byte_img->cmap_b[byte_img->img[i][j]], fp);
           putc(byte_img->cmap_g[byte_img->img[i][j]], fp);
           putc(byte_img->cmap_r[byte_img->img[i][j]], fp);
         }

         for (j=0; j<padb; j++) putc(0, fp);
       }
     }
     else {
       for (i = ydim-1; i >= 0; i--) {
         for (j = 0; j < xdim; j++) {
           putc(byte_img->img[i][j], fp);
           putc(byte_img->img[i][j], fp);
           putc(byte_img->img[i][j], fp);
         }

         for (j=0; j<padb; j++) putc(0, fp);
       }
     }

     closeImageStream(fp, fp_type);

   }



   UShortImage *readBMP16_UShortImage(char *fn_image) {

     FILE *fp;
     int fp_type;

     int          i, c, c1, rv, j, padb;
     unsigned int bfSize, bfOffBits, biSize, biWidth, biHeight, biPlanes;
     unsigned int biBitCount, biCompression, biSizeImage, biXPelsPerMeter;
     unsigned int biYPelsPerMeter, biClrUsed, biClrImportant;
     int xdim, ydim;

     UShortImage *ushort_img;

     // Open file //
     fp = openImageIStream(fn_image, &fp_type);


     // read the file type (first two bytes) //
     c = getc(fp);  c1 = getc(fp);
     if (c!='B' || c1!='M') {
       fprintf(stderr, "[readBMP16] file type != 'BM'\n");
       exit(1);
     }

     bfSize = getint(fp);
     getshort(fp);         // reserved and ignored //
     getshort(fp);
     bfOffBits = getint(fp);

     biSize          = getint(fp);
     biWidth         = getint(fp);
     biHeight        = getint(fp);
     biPlanes        = getshort(fp);
     biBitCount      = getshort(fp);
     biCompression   = getint(fp);
     biSizeImage     = getint(fp);
     biXPelsPerMeter = getint(fp);
     biYPelsPerMeter = getint(fp);
     biClrUsed       = getint(fp);
     biClrImportant  = getint(fp);

     xdim = biWidth;
     ydim = biHeight;

     //
     fprintf(stderr,"\nLoadBMP:\tbfSize=%d, bfOffBits=%d\n",bfSize,bfOffBits);
     fprintf(stderr,"\t\tbiSize=%d, biWidth=%d, biHeight=%d, biPlanes=%d\n",
     biSize, biWidth, biHeight, biPlanes);
     fprintf(stderr,"\t\tbiBitCount=%d, biCompression=%d, biSizeImage=%d\n",
     biBitCount, biCompression, biSizeImage);
     fprintf(stderr,"\t\tbiX,YPelsPerMeter=%d,%d  biClrUsed=%d, biClrImp=%d\n",
     biXPelsPerMeter, biYPelsPerMeter, biClrUsed, biClrImportant);
     //

     if (ferror(fp)) {
       fprintf(stderr, "[readBMP] EOF reached in file header\n");
       exit(1);
     }


     // error checking //
     if (biBitCount!=16 || biClrUsed != 0 || biClrImportant != 0) {
       fprintf(stderr, "[readBMP16] BMP type(%d) not supported\n", biBitCount);
       exit(1);
     }

     // skip ahead to colormap, using biSize //
     c = biSize - 40;    // 40 bytes read from biSize to biClrImportant //
     for (i=0; i<c; i++) getc(fp);


     ushort_img = newUShortImage(xdim, ydim);

     padb = (xdim*6) % 4;  // # of pad bytes to read at EOscanline//
     for (i=(ydim)-1; i >= 0; i--) {
       for (j = 0; j < xdim; j++) {
         ushort_img->img[i][j] = (unsigned short) getshort(fp);
       }
       for (j=0; j<padb; j++) getc(fp);
       if (ferror(fp)) {
         fprintf(stderr, "[readBMP] file truncated\n");
         exit(1);
       }

     }
     closeImageStream(fp, fp_type);

     return(ushort_img);

   }




   void writeBMP16_UShortImage(UShortImage *ushort_img, char *fn_image) {

     FILE *fp;
     int fp_type;
     int bperlin;
     int   i,j,c,padb;
     int xdim, ydim;


     fp = openImageOStream(fn_image, &fp_type);

     xdim = ushort_img->xdim;
     ydim = ushort_img->ydim;


     bperlin = ((xdim * 16 + 31) / 32) * 4;   // # bytes written per line //
     putc('B', fp);  putc('M', fp);           // BMP file magic number //

     // compute filesize and write it //
     i = 14 +                     // size of bitmap file header //
     40 +                  // size of bitmap info header //
     bperlin * ydim;         // size of image data //
     putint(fp, i);

     putshort(fp, 0);         // reserved1 //
     putshort(fp, 0);         // reserved2 //
     putint(fp, 14 + 40);   // offset from BOfile to BObitmap //

     putint(fp, 40);          // biSize: size of bitmap info header //
     putint(fp, xdim);            // biWidth //
     putint(fp, ydim);            // biHeight //
     putshort(fp, 1);         // biPlanes:  must be '1' //
     putshort(fp, 16);        // biBitCount: 1,4,8, or 24 //
     putint(fp, 0);         // biCompression:  BI_RGB, BI_RLE8 or BI_RLE4 //
     putint(fp, bperlin*ydim);    // biSizeImage:  size of raw image data //
     putint(fp, 0);       // biXPelsPerMeter: (75dpi * 39" per meter) //
     putint(fp, 0);       // biYPelsPerMeter: (75dpi * 39" per meter) //
     putint(fp, 0);           // biClrUsed: # of colors used in cmap //
     putint(fp, 0);           // biClrImportant: same as above //

     padb = (xdim*6) % 4;  // # of pad bytes to write at EOscanline //

     for (i = ydim-1; i >= 0; i--) {
       for (j = 0; j < xdim; j++) {
         putshort(fp, ushort_img->img[i][j]);
       }

       for (j=0; j<padb; j++) putc(0, fp);
     }
     closeImageStream(fp, fp_type);

   }








*/

#endif
