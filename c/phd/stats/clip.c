// Clip picture by boolean image

#include <joeylib.c>

int main(int argc,String *argv) {
ArgParser a=ArgParser(argc,argv);
String infile=a.argafter("-p","Picture","image.bmp");
String clipfile=a.argafter("-b","Boolean image","final.bmp");
String ofile=a.argafter("-o","Output clipped image","clipped.bmp");
float ambient=a.floatafter("-a","Ambient shade intesity",0.2);
myRGB shade=myRGB::hue(a.floatafter("-h","Shade hue",2.0/3.0));
//  myRGB shade=myRGB((float)0.0,(float)0.0,(float)1.0);
RGBmp *p=RGBmp::readfile(infile);
float oscale=a.floatafter("-os","Scale of output image",1.0);
a.done();

Map2d<float> *b=Map2d<float>::readfile(clipfile);
RGBmp *o=new RGBmp(p->width,p->height);
for (int i=0;i<p->width;i++) {
for (int j=0;j<p->height;j++) {
if (b->pos[i*b->width/p->width][j*b->height/p->height])
o->bmp[i][j]=p->bmp[i][j];
else
o->bmp[i][j]=shade*p->bmp[i][j];

}
}
o->scaledby(oscale)->writefile(ofile);
}
