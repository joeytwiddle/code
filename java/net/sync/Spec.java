import java.net.Socket;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import java.io.*;

import jlib.*;

public class Spec {

  // final static File pathToCollectors=new File(".");
  private static final String postfix="Collect";

  String collectorName; // classname or default

  static String classForName(String name) {
    return name+postfix;
  }
  static boolean isCollector(String className) {
    return className.endsWith(postfix);
  }
  Spec() {
    collectorName="Default";
  }
  Spec(String n) {
    collectorName=n;
  }
  public String toString() {
    return collectorName;
  }

  public Collect getCollectorOnServer() {
      try {
        Class c=Class.forName(classForName(collectorName));
        return (Collect)c.newInstance();
      } catch (Exception e) {
        JLib.error("getCollectorOnServer: Trying to get Collector class "+collectorName+" but got "+e);
        return null;
      }
  }

  public Collect getCollectorFromServer(SuperSocket sock) {
    CollectLoader cl=new CollectLoader();
    cl.setSocket(sock);
    Thread.currentThread().setContextClassLoader(cl);
    try {
      // byte[] bs=cl.loadClassData(collectorName+"Collect");
      // Class c=cl.defineClass(collectorName+"Collect",bs,0,bs.len);
      Class c=cl.loadClass(classForName(collectorName));
      // System.out.println("Got class "+c+" trying to instantiate.");
      return (Collect)c.newInstance();
    } catch (Exception e) {
      JLib.error(this,"getCollectorFromServer: "+e);
      return null;
    }
/*    sock.write("Can I use this collector?");
    sock.write(collectorName);
    Object o=sock.read();
    try {
      Collect c=(Collect)o;
      sock.write("thanx");
      System.out.println("Client got collector "+c);
      return c;
    } catch (Exception e) {
      sock.write("dying");
      JLib.error("Server would not provide Collector: "+o+"\n"+e);
      return null;
    }*/
  }

  public byte[] getClassBytes() {
    String resourceName=collectorName+".class";
    InputStream in=ClassLoader.getSystemClassLoader().getResourceAsStream(resourceName);
    // System.out.println("Resource "+resourceName+" got "+in);
    return JStream.streamBytes(in);
/*    File f=new File(pathToCollectors,collectorName+".class");
    try {
      return Files.readStringFromFile(f).getBytes();
    } catch (Exception e) {
      return null;
    }*/
  }

}

class CollectLoader extends ClassLoader {

  SuperSocket sock;

  public void setSocket(SuperSocket s) {
    sock=s;
  }

  public Class loadClass(String name) {
    return findClass(name);
  }

  public Class findClass(String name) {
    // System.out.println("Loader.find(): "+name);
    if (Spec.isCollector(name) && !name.equals("Collect")) { // must use local!
      System.out.println("Loading class "+name+" from server...");
      byte[] bs=null;
        bs=loadClassData(name);
      return defineClass(name,bs,0,bs.length);
    } else {
      // System.out.println("("+name+")");
      // System.out.println(this+" Failed to find class "+name+": NOT trying system ClassLoader");
      ClassLoader sysCL=ClassLoader.getSystemClassLoader();
      try {
        return sysCL.loadClass(name);
      } catch (Exception e) {
        return null;
      }
    }
  }

  private byte[] loadClassData(String name) {
    // System.out.println("Loader.data(): "+name);
    try {
      sock.write("Please give me byte[] collector class");
      sock.write(name);
      Object bs=sock.read();
      if (bs==null)
        return null;
    return (byte[])bs;
    } catch (Exception e) {
      JLib.report(e);
      return null;
    }
  }

}

