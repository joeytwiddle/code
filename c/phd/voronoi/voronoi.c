#include <joeylib.c>

void main(int argc,String *argv) {

  ArgParser a=ArgParser(argc,argv);
  String biname=a.getarg("binary image");
  a.done();

  Map2d<bool> *bin=Map2d<bool>::readbinfile(biname);

  FILE *fp=fopen("idata.txt","w");
  int cnt=0;
  for (int i=0;i<bin->width;i++)
  for (int j=0;j<bin->height;j++)
    if (bin->getpos(i,j))
      cnt++;
  fprintf(fp,"2\n%i\n",cnt);
  for (int i=0;i<bin->width;i++)
  for (int j=0;j<bin->height;j++)
    if (bin->getpos(i,j))
      fprintf(fp,"%i %i\n",i,j);
  fclose(fp);

}
