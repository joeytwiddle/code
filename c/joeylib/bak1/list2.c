/* is this deprecated by OrderedList ?

template <class Object>
class OrderedTree {
public:
  Object o;
  float hash;
  OrderedTree<Object> *left,*right;
  int len;
  OrderedTree() {
    len=0;
  }
  void add(Object op,float hashp) {
    if (len==0) {
      o=op;
      hash=hashp;
      left=new OrderedTree<Object>();
      right=new OrderedTree<Object>();
    } else {
      if (hashp<hash)
        left->add(op,hashp);
      else
      right->add(op,hashp);
    }
    len++;
  }
  void getascendinglist(List<Object> *l) {
    if (len>0) {
      left->getascendinglist(l);
      l->add(o);
      right->getascendinglist(l);
    }
  }
  List<Object> getascendinglist() {
    List<Object> l;
    getascendinglist(&l);
    return l;
  }
};

*/

int *p2int(int x) {
  int *n=new int[1];
  n[0]=x;
  return n;
}

      template <class ListObject>
      List<ListObject> List<ListObject>::qsort(float (*fn)(ListObject)) {
        OrderedTree<ListObject> *ot=new OrderedTree<ListObject>();
        for (int i=1;i<=len;i++)
          ot->add(num(i),fn(num(i)));
        return ot->getascendinglist();
      }


/*      template <class ListObject>
      void List<ListObject>::qsort(float (*fn)(ListObject)) {
        OrderedList<int> *ol=new OrderedList<int>();
        for (int i=1;i<=len;i++)
          ol->add(p2int(i),fn(num(i)));
        List<int> p=ol->getascendinglist();
        List<ListObject> c=*this;
        for (int i=1;i<=len;i++)
          list[i-1]=c.num(p.num(i));
      }
*/


