#include <joeylib.c>

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  bool recursive=a.argexists("-r","recursive");
  String dir=a.argor(".");
  a.done();

  List<String> fs;
  if (recursive)
    fs=getfilesrecursive(dir);
  else
    fs=getfilesindir(dir);
  fs.insert(1,"..");
//  for (int i=1;i<=fs.len;i++)
//    printf("%s\n",fs.num(i));
  
  HTMLer h;
  h.start(Sformat("Index of %s",dir));
  h.addtitle();
  for (int i=1;i<=fs.len;i++)
    h.add(Sformat("<tt><a
href=\"%s\">%s</a></tt><br>",fs.num(i),fs.num(i)));
  h.end();
  h.write("index.html");

}
