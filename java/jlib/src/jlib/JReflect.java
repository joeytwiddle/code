package jlib;

import jlib.*;
import java.lang.String;
import java.lang.reflect.*;

public class JReflect {
  public static void main(String[] argv) {
    try {
      Class c=Class.forName(argv[0]);
      Field[] fs=c.getFields();
      Constructor[] cs=c.getConstructors();
      Method[] ms=c.getDeclaredMethods();
//      System.out.println(c+"\n"+fs+"\n"+ms);
      System.out.println(""+c+" extends "+c.getSuperclass()+" implements "+JVector.toString(c.getInterfaces())+":");
      System.out.println("  Fields:");
      for (int i=0;i<fs.length;i++) {
        System.out.println("    "+fs[i]);
				// System.out.println("        ( "+fs[i].getType().getName()+" )");
			}
      System.out.println("  Constructors:");
      for (int i=0;i<cs.length;i++)
        System.out.println("    "+cs[i]);
      System.out.println("  Methods:");
      for (int i=0;i<ms.length;i++)
        System.out.println("    "+ms[i]);
      System.out.println("Trying to create instance...");
      Object o=c.newInstance();
      System.out.println("Got "+o);
    } catch (Exception e) {
      System.out.println(""+e);
    }
  }
  public static Class classcalled(String type) {
    try {
      return Class.forName(type);
    } catch (Exception e) {
      return null;
    }
  }
  public static String classof(Object o) {
    return stripclasspath(o.getClass().getName());
  }
  public static String stripclasspath(String s) {
    int i=s.lastIndexOf(".");
    if (i>-1)
      s=s.substring(i+1);
    return s;
  }
  public static boolean isatypeof(Object o,String type) {
    return canbe(o,classcalled(type));
  }
  public static boolean canbe(Object o,String type) {
    return canbe(o,classcalled(type));
  }
  public static boolean canbe(Object o,Class c) {
    try {
      System.out.println("dc="+c+" oc="+o.getClass());
      if (c.getName().equals("int"))
        c=classcalled("java.lang.Integer");
      return c.isAssignableFrom(o.getClass());
    } catch (Exception e) {
      return false;
    }
  }
}
