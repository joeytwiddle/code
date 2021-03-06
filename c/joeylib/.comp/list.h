#ifndef list_H
  #define list_H

  #include <strings.h>

// num(i) ranges 1..len
// get(i) like Java and C ranges 0..(len-1)

#include <malloc.h>

class Integer {
	public:
		int i; // Exists
		 Integer(); // Method

		 Integer(int ii); // Method

		int getint(); // Method

};

template <class T>
bool equal(T a,T b); // Method


template <class T>
bool toString(T a); // Method


// If template stops working, try adding:
// template class List<int>; to end of file
// in order to get a List<int> class later

// Dynamic static list

// Template class appears in original format:
template<class ListObject>
class List {

public:
	int maxlen,len;
	ListObject* list;

	List() {
		len=0;
		maxlen=1;
		//    list=new ListObject[1];
		list=(ListObject *)malloc(1*sizeof(ListObject));
	}
	List(int n) {
		len=0;
		maxlen=n;
		if (maxlen<=0)
			maxlen=1;
		//    list=new ListObject[maxlen];
		list=(ListObject *)malloc(maxlen*sizeof(ListObject));
	}
	List(List<ListObject> *ls) {
		List(ls->len);
		add(*ls);
	}
	List(int n,ListObject o) {
		len=n;
		maxlen=n;
		if (maxlen<=0)
			maxlen=1;
		list=(ListObject *)malloc(maxlen*sizeof(ListObject));
		//    list=new ListObject[maxlen];
		for (int i=0;i<len;i++)
			list[i]=o;
	}
	List(int n,ListObject fn()) {
		len=n;
		maxlen=n;
		if (maxlen<=0)
			maxlen=1;
		list=(ListObject *)malloc(maxlen*sizeof(ListObject));
		//    list=new ListObject[maxlen];
		for (int i=0;i<len;i++)
			list[i]=fn();
	}
	/* Doesn't work
		List(List<ListObject> *ls) {
		maxlen=ls->len;
		if (maxlen<=0)
		maxlen=1;
		list=(ListObject *)malloc(maxlen*sizeof(ListObject));
		add(*ls);
		} */

	void clear() {
		/*    // Used to free list
				freedom();
				len=0;
				maxlen=1;
				list=(ListObject *)malloc(1*sizeof(ListObject)); */
		// Now it keeps maxlen
		len=0;
	}
	int length() {
		return len;
	}

	int expansionsuggestion() {
		if (maxlen<=0)
			return 1;
		else
			return maxlen*2;
	}
	void expandby(int n) {
		//    printf("ET\n");
		expandto(len+n);
	}
	void expandto(int n) {
		//    printf("ET0\n");
		if (n>maxlen) {
			if (n<expansionsuggestion())
				n=expansionsuggestion();
			//    printf("Expanding from %i to %i\n",maxlen,n);
			maxlen=n;
			//      printf("ET22 %i\n",maxlen*sizeof(ListObject));
			/*      struct mallinfo mi;
					  mi=mallinfo();
					  printf("\ntotal %i , noninuse %i , totalnoninuse %i\n\n",mi.arena,mi.ordblks,mi.fordblks);
					  printf("Trying to allocate %i bytes?\n",maxlen*sizeof(ListObject));*/
			ListObject *nl=(ListObject *)malloc(maxlen*sizeof(ListObject));
			//   printf("ET23\n");
			if (nl==NULL)
				error("Out of memory!\n");
			//    printf("ET24\n");
			//      ListObject *nl=(ListObject *)nlp; 
			//      ListObject *nl=(ListObject *)malloc(maxlen*sizeof(ListObject)); 
			//      ListObject* nl=new ListObject[maxlen];
			//    printf("ET3\n");
			for (int i=1;i<=len;i++) {
				//    printf("ET4\n");
				nl[i-1]=list[i-1];
				// nl[i-1].print();
				// list[i-1].print();
				// if (nl[i-1]==null) {
			}
			//    printf("ET5\n");
			//        for (int i=1;i<=len;i++)
			//          free(&list[i-1]);
			free(list);
			list=nl;
			// list[0].print();
		}
	}
	void empty(int n) {
		expandto(n);
	}
	/* int numempty() {
		int n=0;
		for (int i=1;i<=len;i++) {
		if (list[i-1]==null)
		n++;
		}
		return n;
		}*/

	void operator+(ListObject o) {
		add(o);
	}
	void add(ListObject o) {
		//    printf("LA0\n");
		expandby(1);
		//    printf("LA1\n");
		len++;
		//    printf("LA2\n");
		list[len-1]=o;
		//    printf("LA3\n");
		//    printf(">%i>",len); // (list[len-1]).print();
	}
	void add(List<ListObject> *l) {
		expandby(l->length());
		for (int i=1;i<=l->length();i++) {
			len++;
			list[len-1]=l->list[i-1];
		}
	}
	void add(List<ListObject> l) { // Java compatible
		add(&l);
	}
	void addandfree(List<ListObject> l) {
		add(l);
		l.freedom();
	}
	void concat(List<ListObject> l) {
		add(l);
	}
	void addifnot(ListObject o) {
		if (!contains(o))
			add(o);
	}
	void addifnot(List<ListObject> os) {
		for (int i=1;i<=os.len;i++)
			addifnot(os.num(i));
	}

	void replacenum(int n,ListObject o) {
		replace(n,o);
	}
	void setnum(int n,ListObject o) { // 1..len
		replace(n,o);
	}
	void put(int n,ListObject o) { // java-style 0..len-1
		setnum(n+1,o);
	}
	void replace(int n,ListObject o) { // not Java
		//    expandto(n);
		if (n>=1 && n<=len)
			list[n-1]=o;
		//    else {
		//    }
	}
	void insert(int n,ListObject o) {
		add(num(len));
		for (int i=len-1;i>n;i--)
			replace(i,num(i-1));
		replace(n,o);
		//        printf("Inserted %s\n",o);
	}
	void insert(int n,List<ListObject> l) {
		expandby(l.len);
		for (int i=l.len;i>=1;i--)
			insert(n,l.num(i));
	}

	ListObject num(int n) {
		// if (list[n-1]==null) {
		//      printf("L1\n");
		if (inlist(n)) {
			//        printf("L2\n");
			return list[n-1];
			//        printf("L22\n");
		} else{
			//        printf("L3\n");
			error(Sformat("Tried to get number %i in list length %i",n,len));
			//        printf("L33\n");
		}
		//      printf("L4\n");
	}
	ListObject get(int i) { // Java
		if (i>=0 && i<len)
			return list[i];
		else
			error(Sformat("Tried to get index %i in list length %i",i,len));
	}
	ListObject *getptr(int i) { // Java
		if (i>=0 && i<len)
			return &list[i];
		else
			error(Sformat("Tried to get index %i in list length %i",i,len));
	}
	ListObject numwrap(int n) {
		n=intmod2(n,1,len);
		return list[n-1];
	}
	ListObject wrapnum(int n) {
		return numwrap(n);
	}
	ListObject *p2num(int n) {
		if (inlist(n))
			return &list[n-1];
		else
			error(Sformat("Tried to get pointer to number %i in list length %i",n,len));
	}

	void removenum(int n) { // Warning: this destroys order
		if (n<1 || n>len)
			error("Trying to remove outside list.\n");
		list[n-1]=list[len-1];
		len--;
	}
	void remnum(int n) { // Warning: this destroys order
		removenum(n);
	}
	void removenumkeeporder(int n) {
		if (n<1 || n>len)
			error("Trying to remove outside list.\n");
		for (int i=n;i<len;i++)
			list[i-1]=list[i];
		len--;
	}
	void removenumkeeporder(int n,int j) {
		for (int i=1;i<=j;i++)
			removenumkeeporder(n);
	}
	void swapelements(int a,int b) {
		ListObject tmp=list[a-1];
		list[a-1]=list[b-1];
		list[b-1]=tmp;
	}
	void remove(ListObject o) {
		int f=find(o);
		if (f>0) {
			//  			  printf("Removing %i from %i\n",f,len);
			removenum(f);
		}
	}
	void remove(List<ListObject> ls) {
		for (int i=1;i<=ls.len;i++)
			remove(ls.num(i));
	}

	String toString() {
		String s="List:";
		for (int i=1;i<=len;i++)
			s=Sconc(Sconc(s,::toString(num(i))),","); // Nice guess! =)
		//          s=Sconc(Sconc(s,num(i).toString()),",");
		return Sleft(s,Slen(s)-1);
	}
	/* String pairstoString() {
		String s="";
		for (int i=1;i<=length();i++) {
		Pair p=(Pair)num(i);
		s=s+"("+(String)p.first+","+(String)p.second+"),";
		}
		return s.substring(0,s.length()-1);
		}*/

	/* boolean stringnotin(String s) {
		for (int i=1;i<=this->length();i++) {
		if (((String)this->num(i)).equals(s))
		return false;
		}
		return true;
		}*/
	int find(ListObject o,bool equal(ListObject,ListObject)) {
		for (int i=1;i<=len;i++) {
			if (equal(o,num(i)))
				return i;
		}
		return 0;
	}
	int findlike(bool equal(ListObject,ListObject),ListObject o) {
		return find(o,equal);
	}
	int find(ListObject o) {
		return find(o,&equal);
	}
	template <class Object>
	int findunlike(bool equal(ListObject*,Object),Object o) {
		for (int i=1;i<=len;i++) {
			if (equal(p2num(i),o))
				return i;
		}
		return 0;
	}
	int satisfy(bool isit(ListObject),int start) {
		for (int i=start;i<=len;i++) {
			if (isit(num(i)))
				return i;
		}
		return 0;
	}
	int satisfy(bool isit(ListObject)) {
		return satisfy(isit,1);
	}
	bool inlist(int n) { // Doesn't find the int Object within the list!
		//        printf("Comparing %i %i\n",n,len);
		return (n>=1 && n<=len);
	}
	int findbest(int (*ality)(ListObject),int s) {
		int best=s;
		int besti=-1;
		for (int i=1;i<=len;i++)
			if (ality(num(i))>best) {
				best=ality(num(i));
				besti=i;
			}
		return besti;
	}
	int findbest(int (*ality)(ListObject)) {
		int besti=1;
		int best=ality(num(i));
		for (int i=1;i<=len;i++)
			if (ality(num(i))>best) {
				best=ality(num(i));
				besti=i;
			}
		return besti;
	}
	bool contains(ListObject o) {
		int f=find(o);
		if (f>0)
			return true;
		return false;
	}

	void foralldo(void (*fn)(ListObject)) {
		for (int i=1;i<=len;i++) {
			fn(list[i-1]);
		}
	}
	void foralldo(void (*fn)(ListObject),String s) {
		for (int i=1;i<=len;i++) {
			fn(list[i-1]);
			if (i<len)
				printf(s);
		}
		// If inbetween string is newline, print one at end also
		if (s=="\n")
			printf(s);
	}
	void filterby(bool (*fn)(ListObject)) {
		for (int i=1;i<=len;i++) {
			if (!fn(num(i)))
				removenum(i);
		}
	}
	/*      void filterby(bool (Query<ListObject>::*fn)(ListObject)) {
			  for (int i=1;i<=len;i++) {
			  if (!fn(num(i)))
			  removenum(i);
			  }
			  }*/
	void applycall(void (*fn)(ListObject*)) {
		for (int i=0;i<len;i++)
			fn(&list[i]);
	}
	void applyop(ListObject (*fn)(ListObject)) {
		for (int i=0;i<len;i++)
			list[i]=fn(list[i]);
	}
	template <class Object>
		List<Object> mapby(Object (*fn)(ListObject)) {
			List<Object> l;
			for (int i=0;i<len;i++)
				l.add(fn(list[i]));
			return l;
		}

	/* save for pointer list
		int isnull(int n) {
		return (list[n]==NULL);
		}
		void settonull(int n) {
		list[n]=NULL;
		}
		void clearnulls() {
		for (int i=1;i<=len;i++) { // Don't optimise this!
		if (isnull(i))
		removenum(i);
		}
		}*/

	/*      bool operator==(List<ListObject> o) {
			  return false;
			  }*/

	void orderby(float (*fn)(ListObject)) {
		sort(fn);
	}
	void sortby(float (*fn)(ListObject)) {
		sort(fn);
	}
	List<ListObject> qsort(float (*fn)(ListObject));
	// This fails to compile for ListObject=non-class object:
	// List<ListObject> qsort(float (ListObject::*fn)(ListObject));
	void sort(float (*fn)(ListObject)) {
		// Bubblesort
		bool change=true;
		for (int k=len;k>1 && change;k--) {
			change=false;
			for (int i=1;i<k;i++) {
				if (fn(list[i])<fn(list[i-1])) {
					ListObject tmp=list[i-1];
					list[i-1]=list[i];
					list[i]=tmp;
					change=true;
				}
			}
		}
	}
	void sort(bool (*fn)(ListObject,ListObject)) { // fn: true -> A comes earlier in list than B
		// Bubblesort
		bool change=true;
		for (int k=len;k>1 && change;k--) {
			change=false;
			for (int i=1;i<k;i++) {
				if (fn(list[i],list[i-1])) {
					ListObject tmp=list[i-1];
					list[i-1]=list[i];
					list[i]=tmp;
					change=true;
				}
			}
		}
	}

	List<ListObject> sublist(int t,int b) {
		List<ListObject> n;
		for (int i=t;i<=b;i++)
			n.add(num(i));
		return n;
	}

	ListObject pop(int i) {
		ListObject o=num(i);
		//			  printf("Removing %i from %i\n",i,len);
		removenum(i);
		return o;
	}
	ListObject pop() {
		return pop(len);
	}

	void reverse() {
		for (int i=0;i<len/2;i++) {
			ListObject o=list[i];
			list[i]=list[len-1-i];
			list[len-1-i]=o;
		}
	}

	ListObject min() {
		ListObject min=num(1);
		for (int i=2;i<=len;i++)
			if (num(i)<min)
				min=num(i);
		return min;
	}
	ListObject max() {
		ListObject max=num(1);
		for (int i=2;i<=len;i++)
			if (num(i)>max)
				max=num(i);
		return max;
	}

	void freedom() {
		// Can causes crashes!
		//for (int i=1;i<=len;i++)
		//          free(p2num(i));
		free(list);
	}
	void destroyall() { // Assumes List of pointers to destructable objects
		for (int i=1;i<=len;i++)
			destroy(list[i]);
	}
	void destroynum(int i) { // Assumes List of pointers to destructable objects
		destroy(list[i-1]);
		remnum(i);
	}
	void freenum(int i) {
		free(num(i));
		removenumkeeporder(i);
	}
	void freeall() {
		for (int i=1;i<=len;i++)
			free(num(i));
	}
	void liberateall() {
		for (int i=1;i<=len;i++)
			p2num(i)->freedom();
	}

};// End template class


void writelinestoscreen(List<String> ls); // Method


#endif
