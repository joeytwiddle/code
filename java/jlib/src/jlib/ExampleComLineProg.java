package jlib;

import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;

public class ExampleComLineProg implements ComLineProg {

  String filename;
  int max=10;
  boolean debug=false;
  int intnum;
  Integer Intnum;
  // Integer Intnum2=new Integer(4);

  public void run(List args) {
    System.out.println("OK program is running!");
    System.out.println("I've got filename = "+filename+"");
    System.out.println("              max = "+max);
    System.out.println("            debug = "+debug);
    System.out.println("              num = "+intnum);
    System.out.println("             num2 = "+Intnum);
  }

}