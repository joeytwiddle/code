// Boolean and

#include <joeylib.c>

float neg(float f) {
  return -f;
}

int main(int argc,String *argv) {
  if (argc<=1) {
    printf("band : boolean and operation, provide filenames\n");
    exit(0);
  }
  Map2d<bool> **ps=new (Map2d<bool>*)[argc];
  for (int i=1;i<argc;i++) {
    ps[i]=Map2d<float>::readfile(argv[i])->threshold(0.5);
  }
  Map2d<bool> *n=ps[1];
  for (int i=2;i<argc;i++) {
    //ps[i]->display();
    n=Map2d<bool>::applyop(n,&booland,ps[i]);
    //n->display();
  }
  n->writefile("band.bmp");
}
