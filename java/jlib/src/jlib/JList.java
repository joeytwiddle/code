package jlib;

import java.util.List;
import java.util.Vector;
import java.util.Enumeration;

public class JList {

	public static List toList(Object[] os) {
		List l=new Vector();
		for (int i=0;i<os.length;i++)
			l.add(os[i]);
		return l;
	}

	public static void swap(List l,int i,int j) {
		Object tmp=l.get(i);
		l.set(i,l.get(j));
		l.set(j,tmp);
	}

	public static void shuffle(List l) {
		for (int i=0;i<l.size()*2;i++) {
			int a=JMaths.rnd(0,l.size()-1);
			int b=JMaths.rnd(0,l.size()-1);
			swap(l,a,b);
		}
	}

	/** Returns intersection of two lists.
	 *  ie. [x] where ( x in A AND x in B )
	 **/
	public static List intersection(List a,List b) {
		List l=new Vector();
		for (int i=0;i<a.size();i++) {
			Object o=a.get(i);
			if (b.contains(o))
				l.add(o);
		}
		return l;
	}

	/** Removes all elements in b from a.
	 *  This leaves A = [x] where ( x not in B )
	 **/
	public static void remove(List a,List b) {
		a.removeAll(b);
	}

	public static List fromEnumeration(Enumeration e) {
		List l=new Vector();
		while (e.hasMoreElements())
			l.add(e.nextElement());
		return l;
	}

	public static List apply(Function f,List a) {
		List l=new Vector();
		for (int i=0;i<a.size();i++)
			l.add(f.function(a.get(i)));
		return l;
	}

	public static Object getFirst(Satisfier s,List v) {
		return getfirst(s,v);
	}

	public static Object getfirst(Satisfier s,List v) {
		int i=getFirstIndex(s,v);
		return ( i==-1 ? null : v.get(i) );
	}

	public static int getFirstIndex(Satisfier s,List v) {
		for (int i=0;i<v.size();i++) {
			if (s.satisfies(v.get(i)))
				return i;
		}
		return -1;
	}

	public static int find(List l,Object o) {
		/*		return getFirstIndex( new Satisfier() {
				public boolean satisfies(Object c) {
				return o.equals(c);
				}
				} ); */
		for (int i=0;i<l.size();i++)
			if (o.equals(l.get(i)))
				return i;
		return -1;
	}
	public static int find(Object[] l,Object o) {
		return find(toList(l),o);
	}

	public static boolean contains(List os,Object o) {
		for (int i=0;i<os.size();i++)
			if (os.get(i).equals(o))
				return true;
		return false;
	}

	public static boolean contains(Object[] os,Object o) {
		for (int i=0;i<os.length;i++)
			if (os[i].equals(o))
				return true;
		return false;
	}

	public static String toString(List v) {
		String s="";
		for (int i=0;i<v.size();i++)
			s+=v.get(i).toString()+",";
		return s.substring(0,s.length()-1);
	}

	public static String toString(Object[] v) {
		String s="";
		for (int i=0;i<v.length;i++)
			s+=v[i].toString()+(i<v.length-1?",":"");
		return s;
	}

	public static void addArray(List v,Object[] a) {
		for (int i=0;i<a.length;i++)
			v.add(a[i]);
	}

	public static Object[] toArray(List l) {
		return l.toArray();
	}

}
