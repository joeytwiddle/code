if test "$1" = ""; then
	PROG="lin0"
else
	PROG="$1"
	shift
fi
#' gcc -g -O2 -c `sdl-config --cflags` -Wall -ffast-math -fconserve-space $PROG.c -o $PROG.o
# gcc `sdl-config --libs` -g $PROG.o -o $PROG
# g++ -g -O2 `sdl-config --cflags` `sdl-config --libs` -w -ffast-math -fconserve-space $PROG.c -o $PROG -I/usr/local/include -I/usr/local/include/SDL -lSDLmain -lSDL -I $JPATH/code/c/joeylib/.comp $JPATH/code/c/joeylib/.comp/joeylib.o -DDOS
g++ -g -O2 -w -ffast-math -fconserve-space -w -g -I/usr/local/include -I/usr/local/include/SDL -lSDLmain -lSDL -I /home/joey/j/code/c/joeylib/.comp $PROG.c -o $PROG /home/joey/j/code/c/joeylib/.comp/joeylib.o -DDOS "$@" \
-I/usr/local/include -I/usr/local/include/SDL -I/usr/include/mingw -DWIN32 -Uunix \
-L/usr/local/lib -lmingw32 -lSDLmain -lSDL -mwindows -mno-cygwin
# -Dmain=SDL_main 
# `sdl-config --cflags` `sdl-config --libs` 
# worked for jfc:
# g++ -w -g -I /home/joey/j/code/c/joeylib/.comp $PROG.c -o $PROG /home/joey/j/code/c/joeylib/.comp/joeylib.o -DDOS
