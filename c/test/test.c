#include <joeylib.c>

void main() {
  String s="aA019!#";
  for (int i=1;i<=Slen(s);i++) {
    int j=s[i-1];
    printf("%c %i\n",(char)s[i-1],j);
  }
}
