// Binary contract

#include <joeylib.c>

bool nonpixelwithinrad(Map2d<bool> *m,int i,int j,int r) {
  for (int x=-r;x<=r;x++) {
    int h=sqrt(r*r-x*x);
    for (int y=-h;y<=h;y++) {
      int dx=i+x;
      int dy=j+y;
      if (m->inmap(dx,dy))
        if (!m->pos[dx][dy])
          return true;
    }
  }
//  printf("*");
  return false;
}

int main(int argc,String *argv) {
  ArgParser a=ArgParser(argc,argv);
  String ifname=a.strafter("-i","test.bmp");
  String ofname=a.strafter("-o","contract.bmp");
  int r=a.intafter("-r",1);

  Map2d<bool> *in=Map2d<float>::readfile(ifname)->threshold(0.5);
//  in->display();
  Map2d<bool> *n=new Map2d<bool>(in->width,in->height);

  for (int i=0;i<n->width;i++)
    for (int j=0;j<=n->height;j++) {
      if (nonpixelwithinrad(in,i,j,r))
        n->pos[i][j]=false;
      else
        n->pos[i][j]=true;
    }

  n->writefile(ofname);
//  n->display();
  printf("contract: written file %s radius %i\n",ofname,r);

}
