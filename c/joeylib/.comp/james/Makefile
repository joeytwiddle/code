CC = gcc

LIBRARY = -lm
INCDIRS = -I /usr/include -I /usr/local/include
LIBDIRS = -L /usr/lib -L /usr/local/lib
RUNTIME = -R /usr/lib -R /usr/local/lib
SRCFILES = main.c ccsParser.c fileReader.c dependency.c
OBJFILES = main.o ccsParser.o fileReader.o dependency.o
CFLAGS = $(INCDIRS) $(LIBDIRS) $(RUNTIME) -O2

parser: $(OBJFILES)
	$(CC) $(CFLAGS) -o parser $(OBJFILES) $(LIBRARY)

