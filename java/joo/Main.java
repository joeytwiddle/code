import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;
import java.io.*;
import java.util.*;

import Class.*;
import JooFrame.*;

class JavaParser {
  String filename;
  String precode="";
  public Vector classes=new Vector();

  JavaParser(String fn) {
    filename=fn;
/*    filename="out.java";
    precode="import java.applet.*;\nimport java.awt.*;\nimport java.awt.event.*;\nimport java.util.Random;\nimport java.io.*;\nimport java.util.*;\n";
    Class tmp=new Class("Avatar");
    tmp.fields.add(new Field("String","name","\"bob\""));
    tmp.fields.add(new Field("GridPos","position","new GridPos(2,3)"));
    tmp.fields.add(new Field("int","direction","0"));
    classes.add(tmp);*/
  }
  public void jooify() {
    // To jooify a file, we must include Joo objects
    precode="import JooFrame.*;\n\n"+precode;
    precode="import Joo.*;\n"+precode;
    precode="import Field.*;\n"+precode;
    // We must add Joo functionality to all classes
    for (int i=0;i<classes.size();i++) {
//System.out.println("Jooifying "+(i+1)+" out of "+classes.size()+1);
      ((Class)classes.get(i)).jooify();
    }
/*    JavaParser tmp=new JavaParser("JooMainAdd.java");
    tmp.read();
    classes.add((Class)tmp.classes.get(0));*/
  }
  private Vector readlinesfromfile(String fname) { // trims lines and removes comments
    Vector v=new Vector();
    try {
      BufferedReader br=new BufferedReader(new FileReader(new File(fname)));
      while (br.ready()) {
        String tmp=br.readLine();
        // Strip comments
        while (tmp.indexOf("/*")>=0) {
          int i=tmp.indexOf("/*");
          v.add(tmp.substring(0,i));
          String rest=tmp.substring(i+2);
          while (tmp.indexOf("*/")==-1) {
            tmp=br.readLine();
          }
          tmp=tmp.substring(tmp.indexOf("*/")+2);
        }
        if (tmp.indexOf("//")>=0)
          tmp=tmp.substring(0,tmp.indexOf("//"));
        // Strip spaces
        tmp=tmp.trim();
        v.add(tmp);
//        System.out.println("Read line "+tmp);
      }
      br.close();
    }
    catch (Exception e) {
      System.out.println("Exception "+e+" occured");
    }
    return v;
  }
  public void read() {
System.out.println("Reading file "+filename);
    Vector ls=readlinesfromfile(filename);
    int i=0;
    while (i<ls.size() && !((String)ls.get(i)).startsWith("class") && !((String)ls.get(i)).startsWith("public class")) {
      precode+=(String)ls.get(i)+"\n";
      i++;
    }
    while (i<ls.size()) {
      JString l=new JString((String)ls.get(i));
//System.out.println("Looking for a class in "+l);
      boolean pubclass=false;
      if (l.starts("public ")) {
        pubclass=true;
        l=l.after(" ");
      }
      if (l.starts("class ")) {
        // Parse class
        // Parse first line
        l=l.after("class ");
        Class c=new Class(pubclass,l.before(" ").toString());
        l=l.after(" ");
        l=l.removeall(",");
        if (l.starts("extends ")) {
          l=l.after("extends ");
          while (!l.starts("{") && !l.starts("implements")) {
            c.extended.add(l.before(" "));
            l=l.after(" ");
          }
        }
        if (l.starts("implements ")) {
          l=l.after("implements ");
          while (!l.starts("{")) {
            c.implemented.add(l.before(" "));
            l=l.after(" ");
          }
        }
        l=l.expect("{");
        i++;
        l=new JString((String)ls.get(i));
        while (!l.r.equals("}")) {
//System.out.println("Parsing class "+c.name+" at "+l);
          if (l.r.length()==0)
            i++;
          else {
            // Parse a constructor, field or method
            String type="";
            boolean priv=true;
            boolean stat=false;
            boolean fin=false;
            boolean con=false;
            String a=l.getalphanum().toString();
//System.out.println("Comparing >"+a+"< to class name >"+c.name+"<");
//System.out.println("Line now "+l);
            if (a.equals(c.name)) { // Constructor
              con=true;
//System.out.println("hello\n");
            } else {
              type=l.before(" ").toString();
              l=l.after(" ");
              if (type.equals("public")) {
                priv=false;
                type=l.before(" ").toString();
                l=l.after(" ");
              }
              if (type.equals("private")) {
                priv=true;
                type=l.before(" ").toString();
                l=l.after(" ");
              }
              if (type.equals("static")) {
                stat=true;
                type=l.before(" ").toString();
                l=l.after(" ");
              }
//System.out.println("Checking "+type+" for \"final\"");
              if (type.equals("final")) { // Should this be before static?
                fin=true;
                type=l.before(" ").toString();
                l=l.after(" ");
              }
            }
            String name=l.getalphanum().toString();
//System.out.println("Got name "+name+" in line "+l);
            l=l.after(name);
            l.r=l.r.trim();
            // Is it a variable (field) ?
            // Are there more than one?
//System.out.println("Looking for = or ; in "+l);
            while (l.starts(",")) {
              // ***
              Field f=new Field(type,name,"");
              c.fields.add(f);
              l=l.after(",");
              name=l.getalphanum().toString();
              l=l.after(name);
              l.r=l.r.trim();
            }
            // Is this the last one?
            if (l.starts("=") || l.starts(";")) {
//System.out.println("Starts = or ;");
              String init="";
              if (l.starts("=")) {
                init=l.after("=").before(";").toString();
              }
              Field f=new Field(priv,stat,fin,type,name,init);
              c.fields.add(f);
              i++;
//System.out.println("Added field "+f);
            } else { // It must be a method
              l=l.expect("(");
              Method m=new Method(con,!priv,stat,type,name);
              while (!l.starts(")")) {
                String atype=l.before(" ").toString();
                l=l.from(" ");
                String aname; // Is it last?
                if (l.r.indexOf(",")!=-1) {
                  aname=l.before(",").toString();
                  l=l.after(",");
                } else {
                  aname=l.before(")").toString();
                  l=l.after(aname);
                }
                m.args.add(new Argument(atype,aname));
              }
              l=l.expect(")");
              l.r=l.r.trim();
              l.expect("{");
              // Read code
              i++;
              int curlycount=1;
              while (curlycount>0) {
                l=new JString((String)ls.get(i));
                for (int j=0;j<l.r.length();j++) {
                  if (l.r.charAt(j)=='{')
                    curlycount++;
                  if (l.r.charAt(j)=='}')
                    curlycount--;
                }
                if (curlycount>0) {
                  String ind="  ";
                  for (int ib=1;ib<=curlycount;ib++)
                    ind+="  ";
                  m.code+=ind+l+"\n";
//System.out.println("Adding code "+l);
                }
                i++;
//System.out.println("Ended on line "+(String)ls.get(i));
              }
              c.methods.add(m);
            }
          }
//System.out.println("Got so far with class "+c);
//        i++;
          l=new JString((String)ls.get(i));
//System.out.println("Checking line "+l);
        }
        classes.add(c);
      } else {
//        precode+=(String)ls.get(i)+"\n";
        i++;
      }
    }
System.out.println("Found "+classes.size()+" classes.");
  }
  public void write() {
    filename="open/"+filename;
    try {
      BufferedWriter br=new BufferedWriter(new FileWriter(new File(filename)));
      br.write(precode);
      for (int i=0;i<classes.size();i++) {
        System.out.println("Jooifying "+(i+1)+" out of "+classes.size()+1);
        br.write(((Class)classes.get(i)).toString());
      }
      br.close();
      System.out.println("Written file "+filename);
    }
    catch (Exception e) {
      System.out.println("Exception "+e+" occured");
    }
  }
}

public class Main {
  public static void main(String argv[]) {
/*  	Avatar u=new Avatar();
//    Unicorn u=new Unicorn();
  	JooFrame jf=new JooFrame(u);
  	jf.show();                  */
    for (int i=0;i<argv.length;i++) {
      String filename=argv[i];
      JavaParser jw=new JavaParser(filename);
      jw.read();
    //((Class)jw.classes.get(0)).jooify();
      jw.jooify();
      jw.write();
    }
  }
}
