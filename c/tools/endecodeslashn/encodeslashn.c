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

	// printf("%s","[encoding]");

	while (1) {
		i=getc(fp);
		if (i==-1)
			break;
		// printf("%i\n",i);
		if (i=='\\')    printf("%s","\\\\");
		else if (i==10) printf("%s","\\n");
		else            printf("%c",i);
	}

	fclose(fp);

	return 0;

}
