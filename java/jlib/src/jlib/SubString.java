package jlib;

import java.lang.String;

// If java.lang.String ever becomes un-final, this SubString class could
// extend String with the following modifications:
//   add " extends String "
//   remove "String s; // original": (this) is now the original!
//   modify constructor: change "s=ss;" to "super(ss);"
//   replace "s." with "String." to call superclass rather than original

public class SubString {
  String s; // original
  int a,z; // start,end
  String reals=null;
  SubString(String ss,int aa,int zz) {
    s=ss;
    a=aa;
    z=zz;
  }
  public String real() {
    if (reals==null)
      reals=s.substring(a,z);
    return reals;
  }
  public char charAt(int i) {
    return s.charAt(a+i);
  }
  public String toString() {
    return real();
  }
  public int indexOf(String x) {
    return indexOf(x,0);
  }
  public int indexOf(String x,int i) {
    int j=s.indexOf(x,a+i);
    return ( j<=z-x.length() ? i : -1 );
  }
}