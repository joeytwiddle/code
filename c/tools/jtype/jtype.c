#include <joeylib.c>

bool crs;

bool isacr(int i) {
  return (i==10 || i==13);
}

bool constraints(int i) {
	if (crs && isacr(i))
	  return false;
  return (i>=32 && i<=126);
}

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  crs=a.argexists("-cr","describe carriage returns");
  String fname=a.getarg();
  a.done();

  FILE *fp=fopen(fname,"rb");
  if (fp==NULL)
    error("Problem reading %s\n",fname);

  char c;
  while ((c=(char)getc(fp))!=EOF) {
    int i=(int)c;
    if (constraints(i))
      printf("%c",c);
    else
      printf("<%i>",i);
		if (isacr(i))
		  printf("\n");
  }

  fclose(fp);

}
