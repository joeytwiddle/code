#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define showTypeDetails(typestr,type) printf("%20s: %i\n",typestr,sizeof(type));

int main() {
    showTypeDetails("short int",short int);
    showTypeDetails("int",int);
    showTypeDetails("unsigned int",unsigned int);
    showTypeDetails("unsigned short int",unsigned short int);
    showTypeDetails("char",char);
    showTypeDetails("unsigned char",unsigned char);
}
