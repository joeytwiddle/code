#include <stdio.h>
#include <sys/stat.h>

#include <joeylib.h>

#define hash long
#define hashtop 256*256*256*256

struct match {
	int astart,bstart,length;
	int aend,bend; // derived
};

int alen,blen;
hash *ahashes,*bhashes;
match* matches;

void hashFile(char *fname,int *len,hash **hashes) {
	// printf("%s\n",fname);
	// struct stat *fileattr;
	// stat(fname,fileattr);
	// *len=fileattr->st_size;
	*len=getfilesize(fname);
	// printf("%i\n",*len);
	*hashes=new hash[*len+2];
	int c;
	int i=0;
	hash h=0;
	FILE *file=fopen(fname,"r");
	while ((c=getc(file))!=EOF) {
		// printf("%c",(char)c);
		h=h+c;
		if (h>=hashtop)
			h=h-hashtop;
		(*hashes)[i]=h;
		i++;
		// if (i>*len)
			// printf("gone over limit\n");
	}
	fclose(file);
	// if (i!=*len)
		// printf("*len = %i but i = %i\n",*len,i);
}
		

void main(int argc,char **argv) {

	// printf("%i\n",sizeof(long));
	
	hashFile(argv[1],&alen,&ahashes);
	hashFile(argv[2],&blen,&bhashes);

	
	
	exit(0);
	
}
