#include <joeylib.h>

List<char> *c;

bool match(String s,int i) {
  if (Slen(s)==0)
    return true;
  else
    if (s[0]==c->num(i))
      return match(Sfrom(s,2),i+1);
    else
      return false;
}

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  a.opts.add("Binary / Ascii search and replace.  You must pad replace string for binary files");
  List<String> fs;
  while (a.argsleft()>2)
    fs.add(a.getarg());
  a.com=Sconc(a.com,"[Files] ");
  String search=a.getarg("Search string");
  String replace=a.getarg("Replace string");
  a.done();

  search=Sreplaceall(search,"\\(","(");
  search=Sreplaceall(search,"\\)",")");
  search=Sreplaceall(search,"\\\\","\\");
  search=Sreplaceall(search,"#~bsl~#","\\");

//  printf("Search string : %s\n",search);
//  printf("Replace string: %s\n",replace);

  List<char> replacelist;
  for (int i=0;i<Slen(replace);i++)
    replacelist.add(replace[i]);

  for (int j=1;j<=fs.len;j++) {
  
    String fname=fs.num(j);

    copyfile(fname,Sconc(fname,".b4sr"));

    c=readbinaryfile(fname);
    int cnt=0;
    for (int i=1;i<=c->len-Slen(search)+1;i++) {
      if (match(search,i)) {
        c->removenumkeeporder(i,Slen(search));
        c->insert(i,replacelist);
        i+=Slen(replace)-1;
        cnt++;
      }
    }

    writebinaryfile(fname,c);

    destroy(c);

    printf("%i replacements made in %s\n",cnt,fname);

  }
    
}
