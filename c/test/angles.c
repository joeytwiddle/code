#include <maths.c>
#include <stdio.h>

int main() {
  for (float ang=-2*pi;ang<2*pi;ang=ang+pi/10) {
    printf("%f %f\n",ang,myangle(sin(ang),cos(ang)));
  }
}
