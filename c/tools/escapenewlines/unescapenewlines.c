#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {

	FILE *fp;
	int i;

	if (argc>1) {
		fp=fopen(argv[1],"rb");
	} else {
		fp=stdin;
	}

	// printf("%s","[decoding]");

	while (1) {
		i=getc(fp);
		if (i==-1)
			break;
		// printf("%i\n",i);
		if (i=='\\') {
			int j=getc(fp);
			if (j=='\\') printf("%c",'\\');
			else if (j=='n') printf("%c",'\n');
			else fprintf(stderr,"Got %i followed by %i ('%c','%c').\n",i,j,i,j);
		}
		else printf("%c",i);
	}

	fclose(fp);

	return 0;

}
