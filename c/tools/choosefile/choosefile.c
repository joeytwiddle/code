#include <joeylib.c>

int scrwid=320;
int scrhei=200;

void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  a.com=Sconc(a.com,"[source directories] ");
  String dest=a.argafter("-dest","destination file location",Sconc(safegetenv("JPATH"),"/background.jpg"));

  // Get possible files
  List<String> fs;
  if (a.argsleft()==0) {
    fs.add(getfilesrecursive(Sconc(safegetenv("JPATH"),"/wallpapers/"))); // Default if no args
  }
  while (a.argsleft()) {
    String d=a.getarg();
    fs.add(getfilesrecursive(d));
  }
//  printf("%s\n",fs.toString());
  a.done();

  // Exclude those of wrong type
  List<String> types;
  types.add("jpg");
  types.add("jpeg");
  types.add("bmp");
  types.add("gif");
  types.add("tgz");
  types.add("gz");
  for (int i=1;i<=fs.len;i++) {
    String fname=fs.num(i);
    String ext=Safterlastorall(fname,".");
//    printf("%s has ext %s\n",fname,ext);
    if (    types.find(ext,&Sequiv)==0
         || Sinstr(fname,"wallpapers/tiles/")>0
         || Sinstr(fname,"wallpapers/small/")>0
         || Sinstr(fname,"wallpapers/naff/")>0
         || Sinstr(fname,"wallpapers/for/")>0
         || Sinstr(fname,"/babes/")>0
         || Sinstr(fname,"/fairies/")>0
         || Sinstr(fname,"/mermaids/")>0
         || Sinstr(fname,"/.xvpics/")>0 ) { // Case-insensitive search
//      printf("Didn't find extension %s\n",ext);
      fs.removenum(i);
      i--;
    }
  }

  // Choose a random file and symbolically link it
  randomise();
  int n=intrnd(1,fs.len);
  #ifdef DOS
  String call=Sformat("copy \"%s\" \"%s\"",fs.num(n),dest);
  #else
  String call=Sformat("ln -s -f \"%s\" \"%s\"",fs.num(n),dest);
  #endif
  system(call);
//  setenv("RANDOMFILENAME",fs.num(n),true);

}
