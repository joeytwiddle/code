# LIBS=$(JPATH)/code/c/joeylib/.comp/joeylib.o
LIBS=`./getdeps.sh lin0.c`

CC	= g++ -I $(JPATH)/code/c/joeylib/.comp
# For profiling:
# -pg

CFLAGS	= -g -fconserve-space -O2 -c `sdl-config --cflags` -ffast-math -w
# -Wall
LDFLAGS	= `sdl-config --libs` ${LIBS}
	
OBJECT	= lin0.o
SRC	= lin0.c

.PHONY:	clean

all:	lin0

clean:
	rm -f *.o lin0

lin0:	$(OBJECT)
	$(CC) $(LDFLAGS) $(JPATH)/code/c/joeylib/.comp/joeylib.o $< -o $@

%.o:	%.c
	$(CC) $(CFLAGS) $< -o $@
