#ifndef ordtree_H
  #define ordtree_H

  #include <list.h>

#define OrderedTree OrderedList

// Template class appears in original format:
template <class Object>
class OrderedList { // (in tree form)
public:
  Object *o;
  float hash;
  OrderedList<Object> *left;
  OrderedList<Object> *right;
  int len;
  int mynum; // equiavenlt equiaveltn euqlaievtn equiaveltnt equivalent to 1 + no added on left =)
  OrderedList() {
    len=0;
    o=NULL;
    mynum=0;
    left=right=NULL;
  }
  void add(Object *op,float hashp) {
    len++;
    if (o==NULL) {
      o=op;
      hash=hashp;
		if (left==NULL)
			left=new OrderedList<Object>();
		if (right==NULL)
			right=new OrderedList<Object>();
      mynum=1;
    } else {
      if (hashp<hash) {
        left->add(op,hashp);
        mynum++;
      } else
			right->add(op,hashp);
    }
//    printf("Added\n");
  }
  void clear() {
	  if (o!=NULL) {
		  o=NULL;
		  if (left!=NULL)
			  left->clear();
		  if (right!=NULL)
			  right->clear();
	  }
		len=0;
	}
  void freedom() {
    free(o);
    destroy(left);
    destroy(right);
  }
  void freestruct() {
    destroystruct(left);
    destroystruct(right);
  }
  Object *p2num(int n) {
    // printf("Asked for num %i of a list with %i.\n",n,mynum);
    if (n==mynum) {
      // printf("%f\n",hash);
      return o;
    } else {
      // Only fails if len=0
      if (n<mynum)
        return left->p2num(n);
      else
			return right->p2num(n-mynum);
    }
  }
  Object num(int n) {
    // printf("Asked for num %i of a list with %i.\n",n,mynum);
    if (n==mynum) {
      // printf("%f\n",hash);
      return *o;
    } else {
      // Only fails if len=0
      if (n<mynum)
        return left->num(n);
      else
      return right->num(n-mynum);
    }
  }
   void getascendinglist(List<Object> *l) {
       if (o!=NULL) {
         left->getascendinglist(l);
         l->add(*o);
         right->getascendinglist(l);
       }
     }
     List<Object> getascendinglist() {
       List<Object> l=List<Object>();
       getascendinglist(&l);
       return l;
     }
  
  void add(OrderedList<Object> *o) {
    for (int i=1;i<=o->len;i++) {
      add(o->num(i,hash));
    }
  }
  
};// End template class


#endif
