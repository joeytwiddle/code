#include "oldstrings.c"

#undef char *

class String {
private:
  char *str;
public:
  String() {
    str="";
  }
  String(char *s) {
    str=s;
  }
  char *toString() {
    return str;
  }
};

String operator+(String a,String b) {
  return "Conced\n";
//  return Sconc(a.toString(),b.toString());
}

void main() {
  String s="hello";
  printf("Here\n");
  printf(s); printf("\n");
  String b=s+"fart";
  printf("And here\n");
  printf(b+"\n"); printf("\n");
}