#include <stdio.h>

void main(int argc,char** args) {

  for (int i=0;i<argc;i++)
    printf("Arg %i: %s\n",i,args[i]);

}