#ifndef __fileReader_h
#define __fileReader_h

typedef struct fileReader fileReader;

fileReader *frOpen(char *, char *);
char frGetchar(fileReader *);
void frUngetchar(fileReader *, char);
char lookahead(fileReader *);

#endif
