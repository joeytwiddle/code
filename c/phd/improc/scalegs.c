// Scale

#include <joeylib.c>

float neg(float f) {
  return -f;
}

int main(int argc,String *argv) {
  ArgParser a=ArgParser(argc,argv);
  String infile=a.argafter("-i","Input file","histvar.bmp");
  String outfile=a.argafter("-o","Output file","s1hv.bmp");
  float scale=a.floatafter("-s","Scale",0.5);
  Map2d<float> *in=Map2d<float>::readfile(infile);
  Map2d<float> *out=new Map2d<float>(in->width*scale,in->height*scale);
  for (int i=0;i<out->width;i++)
  for (int j=0;j<out->height;j++) {
    int l=i*in->width/out->width;
    int t=j*in->height/out->height;
    int r=(i+1)*in->width/out->width;
    int b=(j+1)*in->height/out->height;
    float c=0;
    int cnt=0;
    for (int x=l;x<r;x++)
    for (int y=t;y<b;y++) {
      if (x<in->width && y<in->height) {
        c=c+in->pos[x][y];
        cnt++;
      }
    }
    out->pos[i][j]=c/(float)cnt;
  }
  out->writefile(outfile);
}
