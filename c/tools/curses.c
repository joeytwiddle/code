#include <joeylib.c>

void main() {
  bool b = getyesno();
  printf( ( b ? "true\n" : "false\n" ) );
}