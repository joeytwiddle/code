package jlib.strings;

import java.lang.String;
import jlib.strings.*;
import jlib.*;

public abstract class SomeString {

  public abstract char charAt(int i);

  public int indexOf(char c) { // self-ref
    return indexOf(c,0);
  }
  public int indexOf(String x) { // self-ref
    return indexOf(x,0);
  }
  public abstract int indexOf(char c,int i); // eff-over forced
  public abstract int indexOf(String x,int i); // eff over forced
  // public int indexOf(char c,int i) { // eff-over
    // return toString().indexOf(c,i);
  // }
  // public int indexOf(String x,int i) { // eff over
    // return toString().indexOf(x,i);
  // }

  // public boolean startsWith(String x) {
		// try {
			// for (int i=0;i<x.length();i++)
				// if (x.charAt(i) != charAt(i))
					// return false;
		// } catch (Exception e) {
		  // return false;
		// }
		// return true;
    // // return toString().startsWith(x);
  // }
  public boolean startsWith(String x) {
		return matchAt(0,x);
	}

	private boolean matchAt(int x,String s) {
		try {
			for (int i=0;i<s.length();i++)
				if (charAt(x+i) != s.charAt(i))
					return false;
		} catch (Exception e) {
		  return false;
		}
		return true;
	}
			
  public SubString subString(int i,int j) {
    return new SubString(this,i,j);
  }
  public SubString subString(int i) {
    return new SubString(this,i);
  }

  public String substring(int i,int j) {
    return subString(i,j).toString(); // toString().substring(i,j);
  }
  public String substring(int i) {
    return subString(i).toString(); // toString().substring(i);
  }

  public abstract int length(); // eff-over forced
  // public int length() {
    // return toString().length();
  // }

  public int indexOf(SomeString s) {
    return indexOf(s,0);
  }
  public int indexOf(SomeString s,int j) {
    for (int i=j;i<=length()-s.length();i++) {
      boolean done=true;
      for (int k=0;k<s.length() && done;k++)
        if (charAt(i+k)!=s.charAt(k))
          done=false;
      if (done)
        return i;
      //SomeString couldbe=subString(i,i+s.length());
      //if (couldbe.equals(s))
      //  return i;
    }
    return -1;
  }

  public boolean equals(SomeString o) {
    if (length()!=o.length())
      return false;
    for (int i=0;i<o.length();i++)
      if (charAt(i)!=o.charAt(i))
        return false;
    return true;
  }

  public abstract String topString();
  public abstract int topIndex(int i);

}
