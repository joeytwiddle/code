package changetheworld; import jlib.JString;

import java.lang.String;
import java.lang.reflect.*;

public class JReflect {
  public static void main(String[] argv) {
    try {
      Class c=Class.forName(argv[0]);
      Field[] fs=c.getFields();
      Method[] ms=c.getMethods();
      System.out.println(c+"\n"+fs+"\n"+ms);
      for (int i=0;i<fs.length;i++)
        System.out.println(""+fs[i]);
      for (int i=0;i<ms.length;i++)
        System.out.println(""+ms[i]);
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
    try {
      Class c=classcalled(type);
      if (c==null)
        return false;
      return c.isAssignableFrom(o.getClass());
    } catch (Exception e) {
      return false;
    }
  }
  public static boolean canbe(Object o,String type) {
    return isatypeof(o,type);
  }
  public static boolean canbe(Class c,String type) {
    try {
      return classcalled(type).isAssignableFrom(c);
    } catch (Exception e) {
      return false;
    }
  }
}
