import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;
import java.io.*;
import java.util.*;

import Field.*;
import Argument.*;

class Method {
  public String name="";
  public Vector args=new Vector();
  String type="";
  String code="";
  boolean con=false;
  boolean pub=true;
  boolean stat=false;
  Method(String t,String n) {
    pub=true;
    type=t;
    name=n;
  }
  Method(boolean p,String t,String n) {
    pub=p;
    type=t;
    name=n;
  }
  Method(boolean c,boolean p,boolean s,String t,String n) {
    con=c;
    pub=p;
    stat=s;
    type=t;
    name=n;
  }
  Method(String t,String n,Vector a) {
    type=t;
    name=n;
    args=a;
    pub=true;
  }
  public String arglist() {
    String s="(";
    for (int i=0;i<args.size();i++) {
      Argument a=(Argument)args.get(i);
      s=s+a.type+" "+a.name;
      if (i<args.size()-1)
        s=s+",";
    }
    s=s+")";
    return s;
  }
  public boolean ismain() {
    return (pub && stat && type.equals("void") && name.equals("main"));
  }
}

