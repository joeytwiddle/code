#include <joeylib.c>

List<QuadsLine2d> *getquad(List<QuadsLine2d> *l,List<QuadsLine2d> *sf,float cd) {
//  printf("%i %i\n",l->len,sf->len);
  if (sf->len==4)
    if (V2d::dist(sf->num(1).a,sf->num(sf->len).b)<cd)
      return sf;
    else
      return NULL;
  QuadsLine2d last=sf->num(sf->len);
  for (int i=1;i<=l->len;i++) {
    QuadsLine2d c=l->num(i);
    if (V2d::dist(last.b,c.b)<cd)
      c.swapends();
    if (V2d::dist(last.b,c.a)<cd) {
      sf->add(c);
      l->removenum(i);
      List<QuadsLine2d> *q=getquad(l,sf,cd);
//      if (q.len==4)
      if (q!=NULL)
        return q;
      l->add(c);
      l->swapelements(i,l->len);
      sf->removenum(sf->len);
    }
  }
  return NULL;
}      

void display(List<QuadsLine2d> *l) {
  allegrosetup(640,480);
  JBmp b=JBmp(640,480);
  for (int i=1;i<=l->len;i++)
    b.line(4.8*l->num(i).a,4.8*l->num(i).b,15);
  b.writetoscreen();
}

void main() {

  float minlen=10.0;
  float cornerdist=2.0;

  // Generate random list
  List<QuadsLine2d> l=List<QuadsLine2d>();
  for (int i=1;i<=1000;i++) {
    V2d a=V2d(myrnd()*100,myrnd()*100);
    V2d b=V2d(myrnd()*100,myrnd()*100);
    l.add(QuadsLine2d(a,b));
  }

  printf("%i lines\n",l.len);

  // Strip out small lines
  for (int i=1;i<=l.len;i++) {
    if (l.num(i).length<minlen) {
      l.removenum(i);
      i--;
    }
  }

  printf("Stripped small lines, now %i\n",l.len);

//  display(&l);

  // Find quadrilaterals
  List<List<QuadsLine2d>> qs=List<List<QuadsLine2d>>();
  while (l.len>0) {
    List<QuadsLine2d> *q=new List<QuadsLine2d>();
    q->add(l.num(1));
    l.removenum(1);
    q=getquad(&l,q,cornerdist);
    if (q!=NULL) {
      // Join corners
      for (int i=1;i<=q->len;i++) {
        int j=1+(i%q->len);
        q->num(i).b.print();
        printf(",");
        q->num(j).a.print();
        printf(" = ");
        q->p2num(i)->b=(q->num(i).b+q->num(j).a)/2.0;
        q->num(i).b.print();
//        printf(" && ");
        q->p2num(j)->a=q->p2num(i)->b;
//        q->num(j).a.print();
        printf("\n");
      }
      List<V2d> vs;
      vs.add(q->num(1).a);
      vs.add(q->num(2).a);
      vs.add(q->num(3).a);
      vs.add(q->num(4).a);
      Polygon2d p=Polygon2d(vs);
      printf("Quad found! %f\n",p.area());
      qs.add(*q);
      free(q); // don't destroy because qs's copy uses same list=new QuadsLine2d[4];
    }
  }

  for (int i=1;i<=qs.len;i++) {
    List<QuadsLine2d> *q=qs.p2num(i);
      for (int j=1;j<=q->len;j++) {
        q->num(j).a.print();
        printf(" - ");
        q->num(j).b.print();
        printf("\n");
      }
  }

  for (int i=1;i<=qs.len;i++) {
    display(qs.p2num(i));
    do { } while (!key[KEY_ESC]);
  }

  printf("%i quadrilaterals found\n",qs.len);

}
