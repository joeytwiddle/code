import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;
import java.io.*;
import java.util.*;
//import java.lang.reflect;

// To do to parser:
// - Remove comments

// To jooify
// - Add init() to all constructors

// import Class.*;

class WinMan {
  private Vector wins;
}

interface Joo {
  public String type();
//  public Component fullview();
//  public Component shortview();
  public Vector fields();
//  public Vector methods();
//  public Object domethod(String name,Vector args);
  public Object getfield(String name);
//  public void makewindow(WinMan);
  public String toString();
}

class Jint implements Joo {
  public int i;
  private Vector fs=new Vector();
  Jint(int x) {
    i=x;
    init();
  }
  private void init() {
    fs.add(new Field("int","i"));
  }
  public String type() {
    return "Jint";
  }
  public Vector fields() {
    return fs;
  }
  public String toString() {
    return ""+i;
  }
  public Object getfield(String name) {
    return new Integer(i);
  }
}

class GridPos implements Joo {
  public Jint x,y;
  private Vector fs=new Vector();
  GridPos(int xx,int yy) {
    x=new Jint(xx);
    y=new Jint(yy);
    init();
  }
  private void init() {
    fs.add(new Field("Jint","x"));
    fs.add(new Field("Jint","y"));
  }
  public String type() {
    return "GridPos";
  }
  public Vector fields() {
    return fs;
  }
  public String toString() {
    return "("+x+","+y+")";
  }
  public Object getfield(String name) {
    if (name.equals("x"))
      return x;
    if (name.equals("y"))
      return y;
    return null;
  }
}

class Avatar implements Joo {
  public GridPos position;
  public int direction;
  private Vector fs=new Vector();
  Avatar() {
    position=new GridPos(2,3);
    direction=2;
    init();
  }
  private void init() {
    fs.add(new Field("GridPos","position"));
    fs.add(new Field("int","direction"));
  }
  public String type() {
    return "Avatar";
  }
  public Vector fields() {
    return fs;
  }
  public String toString() {
    return "("+position+","+direction+")";
  }
  public Object getfield(String name) {
    if (name.equals("position"))
      return position;
    if (name.equals("direction"))
      return ""+direction;
    return null;
  }
}

