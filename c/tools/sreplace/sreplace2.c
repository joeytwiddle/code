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
  List<String> fs;
  while (a.argsleft()>2)
    fs.add(a.getarg(1));
  String search=a.getarg(1);
  String replace=a.getarg(1);
  a.done();

  for (int i=1;i<=Slen(replace);i++)
    replacelist.add(replace[i]);

  for (int j=1;j<=fs.len;j++) {
  
  String fname=fs.num(j);
  
  copyfile(fname,Sconc(fname,".bfsr"));
  
  List<char> replacelist;
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
  
  c.freedom();
  
  printf("sreplace %s : %i replacements made.\n",fname,cnt);
  
  }
    
}
