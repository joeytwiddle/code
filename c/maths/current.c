#include <joeylib.c>

struct Expression {
  virtual String toString(){
    return "Nothing defined for Expression::toString() (should you be using a new pointer?)";
  }
};

struct Var : public Expression {
  float f;
  Var(float ff) {
    f=ff;
  }
  String toString() {
    return Sformat("%f",f);
  }
};

struct Sine : Expression {
  Expression *within;
  Sine(Expression *ww) {
    within=ww;
  }
  String toString() {
    return Sconc("sin(",Sconc(within->toString(),")"));
  }
};

struct Sum : Expression {
  List<Expression*> *es; // =new List<Expression*>();
  Sum(List<Expression*> *l) {
    es=l;
  }
  String toString() {
    String s="";
    for (int i=1;i<=es->len;i++) {
      s=Sconc(s,es->num(i)->toString());
      if (i<es->len)
        s=Sconc(s,"+");
    }
    return s;
  }
};

void main() {
  List<Expression*> *l=new List<Expression*>();
  l->add(new Sine(new Var(0.7)));
  l->add(new Sine(new Var(0.2)));
  Sum *s=new Sum(l);
  Expression *e=s;
  printf(e->toString());
}
