// Task:
// Recognise 4+m*q+3*q*q+2*q^2=0
// as Ax^2=Bx+c=0
// and solve to get
// ?!

#include "strings.c"

class Statement {
public:
  String type;
  String between;
  List l;
  virtual String tostr();
};

#include "stlist.c"

class Operation : public Statement {
public:
  int ordered,sizeset;
};

class Equals : public Operation {
public:
  Equals(Statement a,Statement b) {
    type="Equals";
    between="=";
    l.add(a);
    l.add(b);
  }
  virtual String tostr() {
    return "=";
  }
};

String Statement::tostr() {
//  if (!strcmp(type,"Equals")) {
  printf("%i",l.length());
    return between;
//  }
//    Equals x=(Equals)(*this);
//    return Equals::tostr();
}

class Sum : public Operation {
public:
  Sum(Statement a,Statement b) {
    l.add(a);
    l.add(b);
  }
  Sum(Statement a,Statement b,Statement c) {
    l.add(a);
    l.add(b);
    l.add(c);
  }
  Sum(Statement a,Statement b,Statement c,Statement d) {
    l.add(a);
    l.add(b);
    l.add(c);
    l.add(d);
  }
};

class Mult : public Operation {
public:
  Mult(Statement a,Statement b) {
    l.add(a);
    l.add(b);
  }
  Mult(Statement a,Statement b,Statement c) {
    l.add(a);
    l.add(b);
    l.add(c);
  }
};

class Power : public Operation {
public:
  Power(Statement a,Statement b) {
    ordered=1;
    sizeset=2;
    l.add(a);
    l.add(b);
  }
};

class Num : public Statement {
public:
  float f;
  String s;
  Num(float a) {
    f=a;
  }
};

class Var : public Statement {
  String s;
public:
  Var(String a) {
    s=a;
  }
};

// Recognise 4+m*q+3*q*q+2*q^2=0

void main() {
  Statement input=Equals(Sum(Num(4),Mult(Var("m"),Var("q")),Mult(Num(3),Var("q"),Var("q")),Mult(Num(2),Power(Var("q"),Num(2)))),Num(0));
  printf(input.tostr());
}
