import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;
import java.io.*;
import java.util.*;
//import java.lang.reflect;

// To do to parser:
// - Remove comments

class JString {
  public String r;
  JString (String ss) {
//System.out.println(">"+ss+"<");
    r=ss;
  }
  public boolean starts(String s) {
    return r.startsWith(s);
  }
  public JString from(String s) {
    int i=r.indexOf(s)+s.length();
    return new JString(r.substring(i));
  }
  public JString after(String s) {
    return from(s);
  }
  public JString before(String s) {
    int i=r.indexOf(s);
    return new JString(r.substring(0,i));
  }
  public JString replaceall(String s,String w) {
    JString n=new JString(new String(r));
    while (n.r.indexOf(s)!=-1)
      n=new JString(n.before(s)+w+n.after(s));
    return n;
  }
  public JString removeall(String s) {
    return replaceall(s,"");
  }
  public JString expect(String s) {
    if (!starts(s))
      System.out.println("I expected a "+s+" in "+r);
    return from(s);
  }
  public JString getalphanum() {
    String n="";
    String a="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    String t=r;
    while (a.indexOf(t.charAt(0))!=-1) {
      n=n+t.charAt(0);
      t=t.substring(1);
    }
    return new JString(n);
  }
  public String toString() {
    return r;
  }
}
