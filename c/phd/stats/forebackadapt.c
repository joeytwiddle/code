#include <joeylib.c>

void main(int argc,String *argv) {
RGBmp *r=new RGBmp("../images/c_10.bmp");
Map2d<float> *g=r->getgreyscale();
Map2d<bool> *b=g->adaptivethreshold(0.3);
b->display();
}
