package jlib;

import java.lang.String;
import java.util.*;
import java.text.*;
import java.io.*;
import java.lang.*;
import java.util.zip.*;
import java.util.jar.*;
import jlib.strings.*;

/** JLib provides access to Log methods, which are identical except that errors cause a System.exit(1)!
**/

public class JLib {

  public static final int defaultSlavePort=3450;
  public static final int defaultSyncPort=3451;


	/** Related to system **/

  public static final File JPATH=getJPATH();
  public static final boolean Debugging=false;

  public static File getJPATH() {
    return new File(execute("sh -c showjpath").trim());
//    return new File(getEnv("JPATH"));
/*    return "/home/joey/j/";
    try {
      String e=System.getProperty("JPATH");
      return e;
    } catch (Exception e) {
    }
    System.out.println("$JPATH environment variable not found");
    return null; */
  }

  public static String getEnv(String varname) {
    System.out.println("I can't get environment variables yet!");
    return execute("sh -c \"echo '$"+varname+"'\"").trim();
    /* Properties p=System.getProperties();
    Enumeration e=p.keys();
    while (e.hasMoreElements()) {
      String s=(String)e.nextElement();
      String r=(String)p.get(s);
      System.out.println(s+" = "+r);
    }
    return "jftp!"; */
  }

  public static InputStream exec_stdout(String com) {
    try {
      // System.out.println("Executing \""+com+"\"");
      Process p=Runtime.getRuntime().exec(com);
      // p.waitFor();
      // JThread.sleep(1000);
      InputStream in=p.getInputStream();
      return in;
    } catch (Exception e) {
      System.out.println("JLib.execute(\""+com+"\"): "+e);
      return null;
    }
  }
  public static String exec(String com) {
    return execute(com);
  }
  public static String execute(String com) {
    try {
      // System.out.println("Executing \""+com+"\"");
      Process p=Runtime.getRuntime().exec(com);
      p.waitFor();
        // JThread.sleep(1000);
        /*InputStream in=p.getInputStream();
        int toget=in.available();
        byte[] bs=new byte[toget];
        in.read(bs); // ,0,toget);
        String s=new String(bs);*/
      String s=JNet.streamFrom(p.getInputStream());
      String err=JNet.streamFrom(p.getErrorStream()).trim();
      //if (err.length()>0)
      //  System.out.println("Executing \""+com+"\" got stderr:\n"+err);
      // System.out.println("Got \""+s+"\"");
      return s+err;
    } catch (Exception e) {
      System.out.println("JLib.execute(\""+com+"\"): "+e);
      return null;
    }
  }


	/** Error and reports to Log **/

  public static void error(String s) {
		Log.error(s);
    // CommandLine.error(s);
    System.exit(1);
  }
  public static void error(Exception e) {
		Log.error(e);
    System.exit(1);
  }
  public static void error(Object o,String s) {
    Log.error(o,s);
		// CommandLine.error(o.getClass()+" "+o+" : "+s);
    System.exit(1);
  }
  public static void error(Object o,Exception e) {
		Log.error(o,e);
    // System.err.println(o.getClass()+" "+o+" threw "+e);
    // e.printStackTrace();
    System.exit(1);
  }
  public static void error(Object obj,String method,Exception e) {
    // System.err.println(obj.getClass().getName()+"."+method+"(): "+e);
    // e.printStackTrace();
		// Log.report(obj+"."+method+"(): "+e);
		Log.error(obj,method,e);
    System.exit(1);
  }

  public static void report(String s) {
    // System.err.println("JLib reports: "+s);
    Log.report(s);
  }
  public static void report(Exception e) {
    // System.err.println(""+e);
		// StringWriter f=new StringWriter();
		// PrintWriter p=new PrintWriter(f);
		// p.println("test fakewriter");
    // e.printStackTrace(p);
		// Log.report(e+"\n at "+f);
		Log.report(e);
  }
  public static void report(Object o,String s) {
    // System.err.println(o.getClass()+" "+o+" reports: "+s);
    Log.report(o,s);
  }
  public static void report(Object o,Exception e) {
    // System.err.println(o.getClass()+" "+o+" reports: "+e);
		// StringWriter f=new StringWriter();
		// PrintWriter p=new PrintWriter(f);
		// e.printStackTrace(p);
    Log.report(o,e);
		// o.getClass()+" "+o+" reports: "+e+"\n at "+f);
	}


	/** User interaction **/

  public static String getUserName() {
    return execute("whoami").trim();
  }

  public static String getMachineName() {
    return execute("hostname").trim();
  }


	/** Time management **/

  public static void stopUntil(long futuretime) {
    stopFor(futuretime-new Date().getTime());
  }

  public static void stopFor(long milliseconds) {
    long start=new Date().getTime();
    long end=start+milliseconds;
    long now;
    while ((now=new Date().getTime())<end) {
      JThread.sleep((int)(end-now));
    }
  }


	/** Other niceities **/

  public static String niceDate(Date d) {
    return new SimpleDateFormat("dd/MM/yy HH:mm:ss").format(d); }
  public static String niceDate() {
    return niceDate(new Date()); }
  public static String niceDate(long ms) {
    return niceDate(new Date(ms)); }

  public static String niceTime(Date d) {
    return new SimpleDateFormat("h:mma").format(d).toLowerCase(); }
  public static String niceTime() {
    return niceTime(new Date()); }
  public static String niceTime(long ms) {
    return niceTime(new Date(ms)); }

  public static boolean makeJar(File jarFile,File topDir,List fileNames) {
    try {
      OutputStream out0=new FileOutputStream(jarFile);
      JarOutputStream out=new JarOutputStream(out0);
      for (int i=0;i<fileNames.size();i++) {
        String fileName=(String)fileNames.get(i);
        JarEntry e=new JarEntry(fileName);
        out.putNextEntry(e);
        try {
          File f=new File(topDir,fileName);
          InputStream in=new FileInputStream(f);
          JStream.pipe(in,out);
        } catch (Exception ex) {
          JLib.report(ex);
        }
        out.closeEntry();
//        System.err.print(".");
      }
      out.finish();
      out.close();
      out0.close();
      return true;
    } catch (Exception e) {
      JLib.error(e);
      return false;
    }
  }

  public static List namesOfEntries(ZipFile f) {
    List entries=JList.fromEnumeration(f.entries());
    Function fn=new Function() {
      public Object function(Object o) {
        return ((ZipEntry)o).getName();
      }
    };
    return JList.apply(fn,entries);
  }

  public static boolean openJar(File jarFile,File topDir) {
    try {
    JarFile jar=new JarFile(jarFile);
    List names=namesOfEntries(jar);
    for (int i=0;i<names.size();i++) {
      String name=(String)names.get(i);
      JarEntry e=jar.getJarEntry(name);
      InputStream in=jar.getInputStream(e);
      try {
        File f=new File(topDir,name);
        f.getParentFile().mkdirs();
        OutputStream out=new FileOutputStream(f);
        JStream.pipe(in,out);
        out.close();
      } catch (Exception ex) {
        JLib.report(ex);
      }
    }
    jar.close();
    return true;
    } catch (Exception e) {
      JLib.error(e);
      return false;
    }
  }

/*  public static void main(String[] args) {
    List l=new Vector();
    for (int i=1;i<args.length;i++)
      l.add(args[i]);
    makeJar(new File(args[0]),new File("."),l);
    File ext=new File("./tmp");
    ext.mkdir();
    openJar(new File(args[0]),ext);
  } */

}
