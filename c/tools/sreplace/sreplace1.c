#include <joeylib.c>

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
  String fname=a.getarg(1);
  String search=a.getarg(1);
  String replace=a.getarg(1);
  a.done();

  List<char> replacelist;
  for (int i=1;i<=Slen(replace);i++)
    replacelist.add(replace[i]);

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

  writebinaryfile(Sconc(fname,".sr"),c);
  
  printf("sreplace %s : %i replacements made.\n",fname,cnt);
    
}
