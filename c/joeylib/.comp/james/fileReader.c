#include <stdio.h>
#include <stdlib.h>

#include "fileReader.h"
#include "globals.h"

#define BUFFERSIZE 128

struct fileReader
{
    FILE *fp;
    char *buf;
    int p;
} ;

/* Simulates fopen
 */
fileReader *frOpen(char *name, char *mode)
{
    fileReader *ret=malloc(sizeof(fileReader));
    int i=0;
    ret->fp=fopen(name,mode);
    if (ret->fp==NULL)
    {
        free(ret);
        return NULL;
    }
    ret->buf=malloc(sizeof(char)*BUFFERSIZE);
    for (i=0;i<BUFFERSIZE;i++)
        ret->buf[i]=0;
    ret->p=0;
    return ret;
}

/* Simulates getchar
 */
char frGetchar(fileReader *f)
{
    char tmp;
    if (f->p==0)
        return fgetc(f->fp);
    else
    {
        f->p--;
        tmp=f->buf[f->p];
        f->buf[f->p]=0;
        return tmp;
    }
}

/* Simulates ungetchar
 */
void frUngetchar(fileReader *f, char c)
{
    if (f->p>=BUFFERSIZE)
    {
        fprintf(stderr,"Can't frUngetchar -- buffer full.\n");
        exit(3);
    }
    else
    {
        f->buf[f->p]=c;
        f->p++;
    }
}

/* Simulates fclose
 */
void frClose(fileReader *f)
{
    fclose(f->fp);
    free (f);
}

/* For special use in parsing
 */
char lookahead(fileReader *f)
{
    char tmp=frGetchar(f);
    frUngetchar(f,tmp);
    return tmp;
}

