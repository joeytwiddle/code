// Make a noisy image

#include <joeylib.c>

int main(int argc,String *argv) {
  Jargsetup(argc,argv);
  String fname=Jgetargafter("-o","noise.bmp");
  int width,height;
  sscanf(Jgetargafter("-w","320"),"%i",&width);
  sscanf(Jgetargafter("-h","200"),"%i",&height);
  Map2d<int> *n=new Map2d<int>(width,height);
  for (int i=0;i<n->width;i++)
    for (int j=0;j<=n->height;j++)
      n->pos[i][j]=myrnd()*255;
  n->writefile(fname);
  printf(Sformat("noise: written file %s w %i h %i\n",fname,width,height));
}
