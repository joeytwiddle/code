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
  public int indexOf(char c,int i) { // eff-over
    return toString().indexOf(c,i);
  }
  public int indexOf(String x,int i) { // eff over
    return toString().indexOf(x,i);
  }

  public boolean startsWith(String x) {
    return toString().startsWith(x);
  }

  public SubString subString(int i,int j) {
    return new SubString(this,i,j);
  }
  public SubString subString(int i) {
    return new SubString(this,i);
  }

  public String substring(int i,int j) {
    return toString().substring(i,j);
  }
  public String substring(int i) {
    return toString().substring(i);
  }

  public int length() {
    return toString().length();
  }

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

}