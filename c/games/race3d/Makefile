CC	= g++ -I $(JPATH)/code/c/joeylib/.comp

CFLAGS	= -g -fconserve-space -O2 -c `sdl-config --cflags` -ffast-math -w
# -Wall
LDFLAGS	= `sdl-config --libs` $(JPATH)/code/c/joeylib/.comp/joeylib.o
	
OBJECT	= lin0.o
SRC	= lin0.c

.PHONY:	clean

all:	lin0

clean:
	rm -f *.o lin0

lin0:	$(OBJECT)
	$(CC) $(LDFLAGS) $< -o $@

%.o:	%.c
	$(CC) $(CFLAGS) $< -o $@
