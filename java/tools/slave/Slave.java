// Problems:
// If a program does a System.exit(), the Slave exits, and must be restarted (slow loading new VM!)
// The slave program does not run in the directory from which the request was made!

import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import java.io.*;
import jlib.*;

// This guy will reload your desired class without a restart =)
// He doesn't appear to load the standard java libs though which is also handy =)

// For efficiency we should probably have him check file dates
// to avoid reloading when not neccessary.

class SmartClassLoader extends ClassLoader {

  List classpath=new Vector();

  public void addClassPath(File f) {
    classpath.add(f);
  }

  File findClassFile(String name) {
    // Get filename from class name ( convert package "."s to file "/"s )
    name=JString.replace(name,".",""+File.separatorChar)+".class";
    for (int i=0;i<classpath.size();i++) {
      File n=new File((File)classpath.get(i),name);
      if (n.exists())
        return n;
    }
    JLib.report("SmartClassLoader.findClassFile(\""+name+"\"): Not found in any of "+classpath);
    return null;
  }

  public Class findClass(String name) {
    byte[] bs=loadClassData(name);
    if (bs==null) {
      JLib.report("Trying system ClassLoader");
      ClassLoader sysCL=ClassLoader.getSystemClassLoader();
      try {
        return sysCL.loadClass(name);
      } catch (Exception e) {
        return null;
      }
    }
    return defineClass(name,bs,0,bs.length);
  }

  private byte[] loadClassData(String name) {
		System.out.println("Loading class "+name);
    File f=findClassFile(name);
    if (f==null) // Will this call the default or me again?!
      return null;
    return Files.readStringFromFile(f).getBytes();
  }

}


public class Slave {

  public static void main(String[] args) {
    Slave s=new Slave();
    s.start();
  }

  Slave() { }

  void start() {

    File watchFile=new File(JLib.JPATH+"/data/wakeup.slave");
    File todoFile=new File(JLib.JPATH+"/data/dothis.slave");
    File finishedFile=new File(JLib.JPATH+"/data/finished.slave");

    echo("");
    echo("Slave waiting for things to do...");
    echo("");

    while (true) {

      // Wait for instructions on what to do
      while (!watchFile.exists()) {
        JLib.stopFor(10); // 0.01 seconds
      }
      echo("Slave is awake!");
      watchFile.delete();

      // Read instructions
      List todo=Files.readLinesFromFile(todoFile);
      File classpath=new File(""+todo.get(0));
      String name=""+todo.get(1);
      String[] argv=JString.split(""+todo.get(2)," ");
      todoFile.delete();

      try {
        // Set up special classloader
        SmartClassLoader cl=new SmartClassLoader();
        cl.addClassPath(classpath);
        Thread.currentThread().setContextClassLoader(cl);

        echo("Loading class...");
        // Try to load the class
        Class c=cl.loadClass(name);
        echo("Got class: "+c);

        // Find its main method
        Class[] paramTypes=new Class[1];
        paramTypes[0]=argv.getClass();
        Method m=c.getMethod("main",paramTypes);
        echo("Got method: "+m);

        // Run the main method with arguments provided
        echo("");
        Object[] arguments=new Object[1];
        arguments[0]=argv;
        m.invoke(null,arguments);
				echo("Main method incation over");

      } catch (Exception e) {
        JLib.report(e);
      }

      Files.touch(finishedFile);

      echo("");

    }

  }

  static void echo(String s) {
    System.out.println(s);
  }

}



