#ifndef list_C
  #define list_C

  #include <list.h>

// num(i) ranges 1..len
// get(i) like Java and C ranges 0..(len-1)

#include <malloc.h>

// Starts of class Integer


  // Variable declared in .h file
   Integer::Integer() { }
   Integer::Integer(int ii) {
    i=ii;
  }
  int Integer::getint() {
    return i;
  }
// End class 


template <class T>
bool equal(T a,T b) {
  return (a==b);
}

template <class T>
bool toString(T a) {
  return a.toString();
}

// If template stops working, try adding:
// template class List<int>; to end of file
// in order to get a List<int> class later

// Dynamic static list
// Template class must appear in .h file


void writelinestoscreen(List<String> ls) {
	for (int i=1;i<=ls.len;i++)
		printf("%s\n",ls.num(i));
}

#endif
