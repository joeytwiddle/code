/*#ifdef UNIX

#include <mcheck.h>

void meminit() {
  mcheck();
}

void displaymeminfo() {
  struct mallinfo mi;
  mi=mallinfo();
  printf("\ntotal %i , noninuse %i , totalnoninuse %i\n\n",mi.arena,mi.ordblks,mi.fordblks);
}

#endif*/