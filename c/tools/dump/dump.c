#include <joeylib.c>

void main(int argc,String *argv) {
  ArgParser a=ArgParser(argc,argv);
  String fname=a.getarg("filename");
  a.done();
  FILE *fp=fopen(fname,"r");
  if (fp==NULL)
    error("Problem reading %s\n",fname);
  char c;
  while ((c=(char)getc(fp))!=EOF) {
    if ((c>=32 && c<=126) || c==10)
      printf("%c",c);
    else
      printf("<%i>",(int)c);
  }
  fclose(fp);
  printf("\n");
}
