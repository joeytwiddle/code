/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.changetheworld.org.uk */

#include <joeylib.c>

class Type;
class Atom;
class Var;
class Text;
class Match;
class RuleSet;
class Grammar;
#define Rule List<Type>

Grammar g;

struct Grammar {
  List<RuleSet> grammar;
};

struct Type {
  virtual Match* match(String s);
};

struct RuleSet {
  Atom atom;
  List<Rule> types;
};

struct Match {
  Type type;
  String string;
  List<Match> matches;
  Match(Type t,String s) {
    type=t;
    string=s;
    matches=List<Match>();
  }
};

struct Atom : public Type {
  String type;
  Atom(String t) {
    type=t;
  }
  Match* match(String s) {

    return NULL;
  }
};

struct Var : public Type {
  String name;
  String deny; // characters which are not allowed
  Var(String n) {
    name=n;
    deny="";
  }
  Var(String n,String d) {
    name=n;
    deny=d;
  }
  Match* match(String s) {
    for (int i=0;i<Slen(deny);i++)
      if (Sinstr(name,&deny[i])>0)
        return NULL;
    return new Match(Var(name,deny),s);
  }
};

struct Text : public Type {
  String text;
  Text(String t) {
    text=t;
  }
  Match* match(String s) {
    if (Seq(text,s))
      return new Match(Text(text),s);
    return NULL;
  }
};


void main(int argc,String *argv) {
  
  ArgParser a=ArgParser(argc,argv);
  a.done();

}
