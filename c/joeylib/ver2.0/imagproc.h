/* ----------------------------------------------------
   Headerfile for the mini image processing library.
   
   Adapted from adc's version by stef 9/2 96
   ----------------------------------------------------
   ----------------------------------------------------
   Log:
   
   o Ansi-fied all source files. Major advantages !
   o Typedef-ed the complex type
   o Named parameter function prototypes. 
   o Wrote `readppm' and `writeppm'
   o Implemented `colour support' in terms of a 
   [x][y][3] array type (uchar, float and complex)
   o Implemented `writegif_bw' and `writegif_col' by
   means of callouts.
   o func gasdev() (from num rec lib) added to lib - 21.10.97 ADC
   
   stef
   ----------------------------------------------------*/

#define PPMTOGIF "/usr/local.share/msc/im+au/bin/ppmtogif"

/* Typedefs */
typedef struct _complex {
  float re,im;
} complex;

/* ---------- Function Prototypes ---------------------*/

/* uchararray.c */
unsigned char  **uchar_array(int width,int height);
unsigned char ***uchar_array3D(int width, int height);

/* farray.c */
float  **float_array(int width, int height);
float ***float_array3D(int width, int height);

/* carray.c */
complex  **complex_array(int width, int height);
complex ***complex_array3D(int width, int height);

/* free.c */
void free_uchar_array(unsigned char **p, int height);
void free_uchar_array3D(unsigned char ***p, int height, int width);
void free_float_array(float **p, int height);
void free_float_array3D(float ***p, int height, int width);
void free_complex_array(complex **p, int height);
void free_complex_array3D(complex ***p, int height, int width);

/* readpgm.c */
unsigned char **readpgm(char *filename,int *width,int *height);

/* writepgm.c */
void writepgm(unsigned char **p, int width, int height, char *filename);

/* readppm.c */
unsigned char ***readppm(char *filename,int *width,int *height);

/* writeppm.c */
void writeppm(unsigned char ***p, int width, int height, char *filename);

/* rowfft.c */
void row_fft(complex **a, int n, int clsz, int inv);

/* colfft.c */
void col_fft(complex **a, int n, int rwsz, int inv);

/* fft2d.c */
complex **fft2d(complex **p, int width, int height, int inv);

/* pseudo_gif.c */
void writegif_bw(unsigned char **p, int xsize, int ysize, char *filename);
void writegif_col(unsigned char ***p, int xsize, int ysize, char *filename);

/* gasdev.c */
float gasdev();
