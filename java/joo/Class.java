import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;
import java.io.*;
import java.util.*;

import Field.*;
import Argument.*;
import Method.*;

class Class {
  public String name="";
  Vector extended=new Vector();
  Vector implemented=new Vector();
  Vector fields=new Vector();
  Vector methods=new Vector();
  boolean pub=false;

  static Vector builtintypes=new Vector();
  static boolean setup=false;

  static private void setup() {
    if (!setup) {
    builtintypes.add("int");
    builtintypes.add("String");
    builtintypes.add("char");
    builtintypes.add("boolean");
    builtintypes.add("float");
    builtintypes.add("double");
    builtintypes.add("long");
    setup=true;
    }
  }

  static public boolean isabuiltintype(String s) {
    setup();
    for (int i=0;i<builtintypes.size();i++)
      if (((String)builtintypes.get(i)).equals(s))
        return true;
    return false;
  }

  Class(String n) {
    name=n;
  }
  Class(boolean p,String n) {
    pub=p;
    name=n;
  }
  public String toString() {
System.out.println("Hello!");
    String s="\n";
    if (pub)
      s=s+"public ";
    s=s+"class "+name+" ";
    // Implements?  Extends?
    if (extended.size()>0) {
      s+="extends ";
      for (int i=0;i<extended.size();i++) {
        s+=(String)extended.get(i).toString();
System.out.println("On ext "+i+" of "+extended.size()+": "+s);
        if (i<extended.size()-1)
          s+=",";
        s+=" ";
      }
    }
    if (implemented.size()>0) {
      s+="implements ";
      for (int i=0;i<implemented.size();i++) {
System.out.println("On implement "+i+" of "+implemented.size());
        s+=(String)implemented.get(i).toString();
        if (i<implemented.size()-1)
          s+=",";
        s+=" ";
      }
    }
    s+="{\n";
    for (int i=0;i<fields.size();i++) {
System.out.println("On field "+i+" of "+fields.size());
      Field f=(Field)fields.get(i);
      s=s+"  ";
      if (f.pub)
        s=s+"public ";
      else
        s=s+"private ";
      s=s+f.type+" "+f.name;
      if (f.initial!="")
        s=s+"="+f.initial;
      s=s+";\n";
    }
    for (int i=0;i<methods.size();i++) {
System.out.println("On method "+i+" of "+methods.size());
      Method m=(Method)methods.get(i);
      s=s+"  ";
      if (!m.con) {
        if (m.pub)
          s=s+"public ";
        else
          s=s+"private ";
        if (m.stat)
          s=s+"static ";
        s=s+m.type+" ";
      }
      s=s+m.name+m.arglist()+" {\n";
      s=s+m.code;
      s=s+"  }\n";
    }
    s=s+"}\n";
    return s;
  }

  void jooify() {

    Method m;

    // Implement Joo
    implemented.add("Joo");

    // Rename the main function
    // Add suggestion in main function
    for (int i=0;i<methods.size();i++) {
      m=(Method)methods.get(i);
      if (m.ismain()) {
        m.code+="    // JooFrame.present(..);\n";
//        methods.remove(i);
//        i--;
      }
    }

    // Add init() to all constructors
    for (int i=0;i<methods.size();i++) {
      m=(Method)methods.get(i);
      if (m.con)
        m.code+="init();\n";
    }

    // Add methods: init(), type(), fields(), toString(), getfield()
    m=new Method("void","init");
    for (int i=0;i<fields.size();i++) {
      Field f=(Field)fields.get(i);
      m.code+="    fs.add(new Field(\""+f.type+"\",\""+f.name+"\"));\n";
    }
    methods.add(m);

    m=new Method("String","type");
    m.code+="    return \""+name+"\";\n";
    methods.add(m);

    m=new Method("Vector","fields");
    m.code+="    return fs;\n";
    methods.add(m);

    m=new Method("String","toString");
    m.code+="    return \"\"";
    for (int i=0;i<fields.size();i++) {
      Field f=(Field)fields.get(i);
      m.code+="+"+f.name;
      if (i<fields.size()-1)
        m.code+="+\",\"";
    }
    m.code+=";\n";
    methods.add(m);
    Vector v=new Vector();
    v.add(new Argument("String","name"));

    m=new Method("Object","getfield",v);
    for (int i=0;i<fields.size();i++) {
      Field f=(Field)fields.get(i);
      m.code+="    if (name.equals(\""+f.name+"\"))\n      return ";
      if (isabuiltintype(f.type)) // Should be "is it a Joo class, ie. are we going to joo it or have we already?"
        m.code+="\"\"+";
      m.code+=f.name+";\n";
    }
    m.code+="    return null;\n";
    methods.add(m);

    // Add field list field
    fields.add(new Field("Vector","fs","new Vector()"));

  }
}
