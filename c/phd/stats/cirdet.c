#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const float pi=3.141593;
const rmin=4;

int width, height, edgethreshold, secondderivthresh, rmax, hotspotthreshold, radiusthreshold, fulloutput;
unsigned char **imagedata, **dfdxdata, **dfdydata, **d2fdx2data, **d2fdy2data, **d2magdata, **magdata, **angdata, **edgedata, **accdata, **hotdata;
unsigned char edgemethod;


void nl()
{
fprintf(stderr,"/n");
}


int square(int x)
{
return x*x;
}


/* Shifts x by multiples of y until it fits range [0,y) */
int mymod(int x, int y)
{
while ((x>=y) || (x<0)) {
if (x>=y)
x=x-y;
if (x<0)
x=x+y;
}
return x;
}


/* Returns value between 0 and 255
representing an angle from 0 to 180
from vertical (y positive) in clockwise direction */
int angle(float x, float y)
{
float tmp;

if (x==0)
return 0;
else {
tmp=256.0*(atan(y/x)+pi/2)/pi;
return tmp;
}
}


/* If above top, then = top
if below 0, then = 0 */
int chop(int x, int top)
{
if (x>top)
return top;
if (x<0)
return 0;
return x;
}


/* If (x,y) in image, then returns value,
otherwise returns value at nearest side of image */
int valatpos(unsigned char **data, int x, int y)
{
x=chop(x,width-1);
y=chop(y,height-1);
return data[y][x];
}


/* Returns approximation for df/dx given by Sobel filter */
int dfdxat(unsigned char **data, int x, int y)
{
int i,d,c;

d=0;
for (i=-1; i<=1; i++) {
c=valatpos(data,x-1,y+i)-valatpos(data,x+1,y+i);
if (i==0)
c=2*c;
d=d+c;
}
return d;
}


/* Returns approximation for df/dy given by Sobel filter */
int dfdyat(unsigned char **data, int x, int y)
{
int i,d,c;

d=0;
for (i=-1; i<=1; i++) {
c=valatpos(data,x+i,y-1)-valatpos(data,x+i,y+1);
if (i==0)
c=2*c;
d=d+c;
}
return d;
}


/* Returns 1 (true) if (x,y) is in image, otherwise 0 */
int inimage(int x, int y)
{
if ((x>=0) && (x<width) && (y>=0) && (y<height))
return 1;
else
return 0;
}


/* Increments all points in line (x1,y1)-(x2,y2) */
void line(unsigned char **acc, int x1, int y1, int x2, int y2)
{
int x,y,tmp;

/* If line is more horizontal than vertical */
if (abs(x2-x1)>abs(y2-y1)) {
if (x1>x2) {
tmp=x1; x1=x2; x2=tmp;
tmp=y1; y1=y2; y2=tmp;
}
for (x=x1; x<=x2; x++) {
y=y1+(y2-y1)*(x-x1)/(x2-x1);
if (inimage(x,y))
acc[y][x]++;
}
} else {
/* Otherwise, line is more vertical than horizontal */
if (y1>y2) {
tmp=x1; x1=x2; x2=tmp;
tmp=y1; y1=y2; y2=tmp;
}
for (y=y1; y<=y2; y++) {
x=x1+(x2-x1)*(y-y1)/(y2-y1);
if (inimage(x,y))
acc[y][x]++;
}
}
}


int toint(float x)
{
return x;
}


/* Returns magnitude of x0,y0 in direction at x1,y1 */
int magindir(int x0, int y0, int x1, int y1)
{
if (magdata[y1][x1]==0)
return magdata[y0][x0];
else
return ((dfdxdata[y0][x0]-128)*(dfdxdata[y1][x1]-128)+(dfdydata[y0][x0]-128)*(dfdydata[y1][x1]-128))/magdata[y1][x1];
}


/* Returns sign of x (+1/0/-1) */
int sgn(int x)
{
if (x==0)
return 0;
else {
if (x>0)
return 1;
else
return -1;
}
}


/* Image -> dfdx, dfdy, mag, ang, edge, 2d Circle Accumulator */
void accumulate()
{
int x,y,dfdx,dfdy,mag,ang,i,j,greatestlocal,mylm;
float realang;

/* Generate dfdx, dfdy, mag, ang */
for (y=0; y<height; y++) {
for (x=0; x<width; x++) {
dfdx=dfdxat(imagedata,x,y);
dfdy=dfdyat(imagedata,x,y);
dfdxdata[y][x]=128+dfdx/8; /* /8 ensures value is in [0,255] */
dfdydata[y][x]=128+dfdy/8; /* since can get as high as +/- 4*255 */
magdata[y][x]=sqrt(square(dfdx)+square(dfdy))/4/sqrt(2);
/* /4sqrt(2) ensures value is in [0,255]
since can get as high as sqrt(2)*4*255 */
angdata[y][x]=angle(dfdx,dfdy);
}
}

/* Find edges according to edge method */
switch (edgemethod) {
/* Normal mag threshold method */
case 'a' : 
for (y=0; y<height; y++) {
for (x=0; x<width; x++) {
if (magdata[y][x]>=edgethreshold) {
edgedata[y][x]=255;
}
}
}
break;
/* Local max (checks four neighbours */
case 'b' :
for (y=0; y<height; y++) {
for (x=0; x<width; x++) {
mag=magdata[y][x];
if (mag>=edgethreshold) {
greatestlocal=1;
if (inimage(x-1,y))
if (magdata[y][x-1]>mag)
greatestlocal=0;
if (inimage(x+1,y))
if (magdata[y][x+1]>mag)
greatestlocal=0;
if (inimage(x,y+1))
if (magdata[y+1][x]>mag)
greatestlocal=0;
if (inimage(x,y-1))
if (magdata[y-1][x]>mag)
greatestlocal=0;
if (greatestlocal==1)
edgedata[y][x]=255;
}
}
}
break;
/* Local max in its own direction */
case 'c' :
for (y=0; y<height; y++) {
for (x=0; x<width; x++) {
if (magdata[y][x]>=edgethreshold) {
greatestlocal=1;
mylm=magindir(x,y,x,y);
if (inimage(x-1,y-1))
if (magindir(x-1,y,x,y)>mylm)
greatestlocal=0;
if (inimage(x+1,y-1))
if (magindir(x+1,y,x,y)>mylm)
greatestlocal=0;
if (inimage(x-1,y+1))
if (magindir(x,y-1,x,y)>mylm)
greatestlocal=0;
if (inimage(x+1,y+1))
if (magindir(x,y+1,x,y)>mylm)
greatestlocal=0;
if (greatestlocal==1)
edgedata[y][x]=255;
}
}
}
break;
/* Second derivative is low */
case 'd' :
/* Form second derivative images */
for (y=0; y<height; y++) {
for (x=0; x<width; x++) {
d2fdx2data[y][x]=128+dfdxat(dfdxdata,x,y)/8;
d2fdy2data[y][x]=128+dfdyat(dfdydata,x,y)/8;
d2magdata[y][x]=sqrt(64*square(d2fdx2data[y][x]-128)+64*square(d2fdy2data[y][x]-128))/4/sqrt(2);
}
}
/* Check second derivative is low */
for (y=1; y<height-1; y++) {
for (x=1; x<width-1; x++) {
if (magdata[y][x]>=edgethreshold)
if (d2magdata[y][x]<secondderivthresh)
edgedata[y][x]=255;
/* Old method checked to see if second derivative crossed zero
but it didn't work very well!
if ((sgn(d2fdx2data[y][x+1]-128)!=sgn(d2fdx2data[y][x-1]-128)) || (sgn(d2fdx2data[y+1][x]-128)!=sgn(d2fdx2data[y-1][x]-128)))*/
}
}
break;
}

/* Accumulate 2d lines */
for (y=0; y<height; y++) {
for (x=0; x<width; x++) {
if (edgedata[y][x]==255) {
/* Accumulate line through edge point at appropriate angle */
realang=angdata[y][x]*pi/256.0-pi/2.0;
line(accdata,x-rmax*cos(realang),y-rmax*sin(realang),x+rmax*cos(realang),y+rmax*sin(realang));
}
}
}

}


/* Clear 1d accumulator */
clearacc(int *acc)
{
int i;

for (i=0; i<=rmax+2; i++) {
acc[i]=0;
}
}


/* Plot circle in image with colour col */
void circle(unsigned char **image, int x, int y, int rad, int col)
{
int i,j;
float a,step;

step=2*pi/(2*pi*rad);
if (step<0.1)
step=0.01;
if (step>1.0)
step=1.0;
for (a=0.0; a<2*pi; a=a+step) {
i=x+rad*cos(a); j=y+rad*sin(a);
if (inimage(i,j))
image[j][i]=col;
/* Checkered circle effect (very clear)
image[j][i]=255*(mymod(a*32/2/pi,2));*/
}
}


/* Image, Edges, 2d Accumulator -> Overlaid Image */
void detect()
{
int x,y,i,j,l,tmp,circles,topradv,topradindex;
int radacc[rmax+2],lookup[rmax+2];

/* Fill lookup table */
clearacc(lookup);
for (i=-rmax-1; i<=rmax+1; i++) {
tmp=sqrt(square(rmax+1)-square(i));
for (j=-tmp; j<=tmp; j++) {
lookup[toint(sqrt(square(i)+square(j)))]++;
}
}

/* Prints values for lookup table compared to those
calculated using 2*pi*r
for (i=0; i<=rmax; i++) {
if (lookup[i]==0)
lookup[i]=1;
fprintf(stderr,"radius $i : lookup $i math $i",i,lookup[i],toint(2*pi*i)); nl();
}*/

circles=0;

for (y=0; y<height; y++) {
for (x=0; x<width; x++) {
if (accdata[y][x]>=hotspotthreshold) {

/* For debugging, creates image of hotspots */
hotdata[y][x]=255;

/* Construct 1d radius accumulator */
clearacc(radacc);
/* For all points within radius rmax-1 */
for (i=-rmax-1; i<=rmax+1; i++) {
tmp=sqrt(square(rmax+1)-square(i));
for (j=-tmp; j<=tmp; j++) {
if (inimage(x+i,y+j))
/* If edge then increase accumulator at that radius */
if (edgedata[y+j][x+i]>0)
radacc[toint(sqrt(square(i)+square(j)))]++;
}
}

/* Check accumulator for signs of circles */

/* Any radii with count above radius threshold ($) */
for (l=rmin; l<=rmax; l++) {
if (radacc[l]>=lookup[l]*radiusthreshold/100) {
circles++;
/* Plots over image, with colour according to confidence */
circle(imagedata,x,y,l,255*radacc[l]/lookup[l]);
}
}

/* Radius with largest count
topradv=radacc[rmin]; topradindex=rmin;
for (l=rmin; l<=rmax; l++) {
if (radacc[l]>topradv) {
topradv=radacc[l];
topradindex=l;
}
}
circles++; fprintf(stderr,"O");
circle(imagedata,x,y,topradindex,255);*/

}
}
}

fprintf(stderr,"$i circles detected.",circles); nl();

}


int ord(char x)
{
return x-48;
}


/* Returns number from a string */
int num(char *str)
{
int i,n;

if (str[0]<'0' || str[0]>'9')
return 0;
else {
n=0;
for (i=0; (str[i]>='0' && str[i]<='9'); i++) {
n=10*n+ord(str[i])-ord('0');
}
return n;
}
}


void main(int argc, char **argv)
{
char *imagename;

/* If too few arguments given, display syntax */
if (argc<9) {
fprintf(stderr,"Command line:"); nl();
fprintf(stderr,"det <image> <edge threshold> <second derivative threshold> <maximum radius> <centre hotspot threshold> <radius hotspot threshold ($$)> <full output (0/1)> <edge method (a/b/c/d)>"); nl();
fprintf(stderr,"edge method a : >=mag"); nl();
fprintf(stderr,"            b : local max"); nl();
fprintf(stderr,"            c : local max in it's own direction"); nl();
fprintf(stderr,"            d : second derivative crosses zero"); nl();
} else {

/* Otherwise, go ahead and do the circle detection */
imagename=argv[1];
edgethreshold=num(argv[2]);
secondderivthresh=num(argv[3]);
rmax=num(argv[4]);
hotspotthreshold=num(argv[5]);
radiusthreshold=num(argv[6]);
fprintf(stderr,"Read $i $i $i $i $i",edgethreshold,secondderivthresh,rmax,hotspotthreshold,radiusthreshold); nl();
if (num(argv[7])==1) {
fulloutput=1;
fprintf(stderr,"Giving full output"); nl();
} else
fulloutput=0;
edgemethod=argv[8][0];
if (!(edgemethod=='a') && !(edgemethod=='b') && !(edgemethod=='c') && !(edgemethod=='d'))
edgemethod='a';
fprintf(stderr,"Edge method: $c",edgemethod); nl();

imagedata=readpgm(imagename,&width,&height);

dfdxdata=uchar_array(width,height);
dfdydata=uchar_array(width,height);
if (edgemethod=='d') {
d2fdx2data=uchar_array(width,height);
d2fdy2data=uchar_array(width,height);
d2magdata=uchar_array(width,height);
}
magdata=uchar_array(width,height);
angdata=uchar_array(width,height);
edgedata=uchar_array(width,height);
accdata=uchar_array(width,height);
hotdata=uchar_array(width,height);

accumulate();
detect();

/* Write extra images if requested */
if (fulloutput==1) {
writepgm(dfdxdata,width,height,"2dfdx.pgm");
writepgm(dfdydata,width,height,"2dfdy.pgm");
if (edgemethod=='d') {
writepgm(d2fdx2data,width,height,"3d2fdx2.pgm");
writepgm(d2fdy2data,width,height,"3d2fdy2.pgm");
writepgm(d2magdata,width,height,"3d2mag.pgm");
}
writepgm(magdata,width,height,"4mag.pgm");
writepgm(angdata,width,height,"5ang.pgm");
writepgm(edgedata,width,height,"6edge.pgm");
writepgm(hotdata,width,height,"8hot.pgm");
}

/* Write out compulsory images */
writepgm(accdata,width,height,"7acc.pgm");
writepgm(imagedata,width,height,"9out.pgm");

}
}

