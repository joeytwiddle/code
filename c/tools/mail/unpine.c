#include <joeylib.h>

List<String> ls;

int nextstart(int n) {
  if (n==ls.len)
    return 0;
  for (n=n;n<=ls.len;n++) {
  	
  	// Find size of content
  	printf(">> %s\n",ls.num(n));
  	if (Sstarts(ls.num(n),"Content-Length: ")) {
  	
  	  String p=Sfrom(ls.num(n),17);
    	int len;
	    sscanf(p,"%i",&len);
	    printf("*** %s\n*** %s\n*** %i\n",ls.num(n),p,len);
	
	    // Find beginning of content
			n=n+1;
			while (Slen(ls.num(n))>0) {
			  n++;
			}
			
			// Read through content
			while (len>10 && n<=ls.len) {
      	printf("%i %s\n",len,ls.num(n));
			  len=len-Slen(ls.num(n))-1;
			  n++;
			}
			
			// Find start of next
			while (n<=ls.len && !Sstarts(ls.num(n),"From - ")) {
      	// printf("%i %s\n",len,ls.num(n));
			  // len=len-Slen(ls.num(n))-1;
			  n++;
			  printf(".");
			}
			printf("\n");
			
			return n;
		
		}
  }
	return ls.len+1;
}

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  String fname=a.getarg(1);
  a.done();
  
  ls=readlinesfromfile(fname);
	
  String destdir=Sformat("%s.ems",fname);
  system(Sformat("mkdir %s",destdir));
  system(Sformat("rm %s/*",destdir));
  
  int i=1;
  while (i>0 && i<=ls.len) {
    int j=nextstart(i);
    List<String> nls;
    for (int k=i;k<j;k++)
      nls.add(ls.num(k));
    writelinestofile(nls,Sformat("%s/%s",destdir,getnextfilename("txt")));
    i=j;
  }
    
}
