// Scale

#include <joeylib.c>

float neg(float f) {
  return -f;
}

int main(int argc,String *argv) {
  ArgParser a=ArgParser(argc,argv);
  float scale=a.floatafter("-s","Scale",0.5);
  bool towidth=a.argexists("-w","scale to width",false);
  int nwidth;
  if (towidth)
    nwidth=a.intafter("-w","width",1024);
  bool toheight=a.argexists("-h","scale to height",false);
  int nheight;
  if (toheight)
    nwidth=a.intafter("-h","height",768);
  bool wgs=a.argexists("-gs","Greyscale output instead of colour");
  String infile=a.getarg("image file");
  String outfile=a.getarg("scaled file");
  a.done();
  RGBmp *in=RGBmp::readfile(infile);
  if (towidth)
    scale=nwidth/in->width;
  if (toheight)
    scale=nheight/in->height;
  RGBmp *out=RGBmp::scale(in,scale);
  /*
  RGBmp *out=new RGBmp(in->width*scale,in->height*scale);
  for (int i=0;i<out->width;i++)
  for (int j=0;j<out->height;j++) {
    if (scale
    int l=i*in->width/out->width;
    int t=j*in->height/out->height;
    int r=(i+1)*in->width/out->width;
    int b=(j+1)*in->height/out->height;
    V3d c=V3d::o;
    int cnt=0;
    for (int x=l;x<r;x++)
    for (int y=t;y<b;y++) {
      if (x<in->width && y<in->height) {
        c=c+in->bmp[x][y].v3d();
        cnt++;
      }
    }
    out->bmp[i][j]=(c/(float)cnt).myrgb();
  }*/
  if (wgs)
    out->getgreyscale()->writefile(outfile);
  else
    out->writefile(outfile);
}
