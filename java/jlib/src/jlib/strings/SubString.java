package jlib.strings;

import java.lang.String;
import jlib.strings.*;
import jlib.*;

// If java.lang.String ever becomes un-final, this SubString class could
// extend String with the following modifications:
//   add " extends String "
//   remove "String s; // original": (this) is now the original!
//   modify constructor: change "s=ss;" to "super(ss);"
//   replace "s." with "String." to call superclass rather than original

public class SubString extends SomeString {

  String s; // original
  int a,z; // start,end
  int len;
  String reals=null;

  public static int sscnt=0;
  public static int ssrealedcnt=0;

  public SubString(String ss) {
    this(ss,0);
  }
  public SubString(String ss,int i) {
    this(ss,i,ss.length());
  }
  public SubString(SomeString ss,int i) {
    this(ss,i,ss.length());
  }
  public SubString(String ss,int aa,int zz) {
    setup(ss,aa,zz);
  }
  public void setup(String ss,int aa,int zz) {
    sscnt++;
    s=ss;
    a=aa;
    z=zz;
    len=zz-aa;
  }
  public SubString(SomeString ss,int aa,int zz) {
    if (ss instanceof RealString) {
      setup(ss.toString(),aa,zz);
    } else if (ss instanceof SubString) {
      setup(ss.topString(),ss.topIndex(aa),ss.topIndex(zz));
    }
  }

  public char charAt(int i) {
    return s.charAt(a+i);
  }

  public void thrower() throws Exception {
    throw new Exception("SubString.thrower");
  }

  public String real() {
    if (reals==null) {
      ssrealedcnt++;
//      try { thrower(); } catch (Exception e) { e.printStackTrace(); }
      reals=s.substring(a,z);
    }
    return reals;
  }
  public String toString() {
    return real();
  }

  public int indexOf(char c,int i) {
    int j=s.indexOf(c,a+i);
    return ( j>=a && j<z ? j-a : -1 );
  }
  public int indexOf(String x,int i) {
    int j=s.indexOf(x,a+i);
    return ( j>=a && j<z ? j-a : -1 );
  }

  public boolean startsWith(String x) {
    if (x.length()>len)
      return false;
    for (int i=0;i<x.length();i++)
      if (charAt(i)!=x.charAt(i))
        return false;
    return true;
  }

  /* public SubString subString(int i,int j) {
    return new SubString(this,i,j);
  }
  public SubString subString(int i) {
    return new SubString(this,i);
  } */

  public String substring(int i,int j) {
    return s.substring(a+i,a+j);
  }
  public String substring(int i) {
    return s.substring(a+i);
  }

  public int length() {
    return len;
  }

	// public int length() { // Now requied, and more efficient this way
		// return z-a;
	// }

  public boolean equals(String o) {
    if (length()!=o.length())
      return false;
    return startsWith(o);
  }

  public String topString() {
    return s;
  }
  public int topIndex(int i) {
    return a+i;
  }

	// // These functions are generic given charAt
	// public int indexOf(char c,int s) {
		// for (int i=s;i<length();i++)
			// if (charAt(i)==c)
				// return i;
		// return -1;
	// }
// 
	// public int indexOf(String x,int s) {
		// for (int i=s;i<length()-x.length();i++)
			// if (matchAt(i,x))
				// return i;
		// return -1;
	// }

}
