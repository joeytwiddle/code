# gcc -g -O2 -c `sdl-config --cflags` -Wall -ffast-math -fconserve-space transtri.c -o transtri.o
# gcc `sdl-config --libs` -g transtri.o -o transtri
gcc -g -O2 `sdl-config --cflags` `sdl-config --libs` -Wall -ffast-math -fconserve-space transtri.c -o transtri -I/usr/local/include -I/usr/local/include/SDL -lSDLmain -lSDL
