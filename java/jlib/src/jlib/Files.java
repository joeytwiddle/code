package jlib;

import java.applet.*;
import java.util.Random;
import java.io.*;
import java.util.Vector;
import java.util.*;

public class Files {

  public static final String legalchars=JString.alphanum+" _.,-/";

  public static String log="<b>Files Log:</b>\n<br>";

  public static String[] split(File f) {
    return split(f+"");
  }
  public static String[] split(String s) {
    try {
      final int maxextlen=10;
      int i=s.lastIndexOf(".");
      if (i<s.length()-maxextlen)
        i=-1;
      String[] lr=new String[2];
      lr[0]=( i==-1 ? s : s.substring(0,i) );
      lr[1]=( i==-1 ? "" : s.substring(i+1) ); // leaves '.' out of answers
      // lr[1]=( i==-1 ? "" : s.substring(i) ); // keeps '.' if exists
      // If extension contains illegal characters then it ain't a real extension!
      if (!JString.containedBy(lr[1],legalchars)) {
        lr[0]=s;
        lr[1]="";
      }
      return lr;
    } catch (Exception e) {
      System.out.println("*** C "+e);
      return null;
    }
  }

  public static PrintWriter printWriterToFile(File f) {
    try {
      PrintWriter p=new PrintWriter(
        new BufferedWriter(
          new FileWriter(f)
        )
      );
      return p;
    } catch (Exception e) {
      JLib.error(e);
      return null;
    }
  }

  public static Vector readLinesFromFile(File f) {
    return readlinesfromfile(f,false);
  }
  public static Vector readlinesfromfile(String fname) {
    return readlinesfromfile(fname,false);
  }
  public static Vector readlinesfromfileandtrim(String fname) {
    return readlinesfromfile(fname,true);
  }
  public static Vector readlinesfromfile(String fname,boolean dotrim) {
    return readlinesfromfile(new File(fname),dotrim);
  }
  public static Vector readlinesfromfile(File f,boolean dotrim) {

    try {

      Vector v=new Vector();
      BufferedReader br=new BufferedReader(new FileReader(f));
      while (br.ready()) {
        String tmp=br.readLine();
        // Strip spaces
        if (dotrim)
          tmp=tmp.trim();
        v.add(tmp);
        // System.out.println("Read line "+tmp);
      }
      br.close();

      return v;

    } catch (Exception e) {

      if (JLib.Debugging) {
        String report="Files.readlinesfromfile("+f+"): "+e+" - returning null;";
        System.out.println(report);
        log+=report;
      }

      return null;

    }
  }

  /** Designed to mess with your brain if you use delimeter "mydelimeter"
   *  and try to parse "favourite curry mydelimydelimeterfavourite soup bananamydelimeter"
   *  cos it won't work!
  **/
  public static Vector readlistfromfile(File f,String sep) {
    return readlistfromfile(f,sep,false);
  }
  public static Vector readlistfromfile(File f,String sep,boolean dotrim) {

    try {

      Vector v=new Vector();
      BufferedReader br=new BufferedReader(new FileReader(f));
      StringBuffer current=new StringBuffer();
      char first=sep.charAt(0);
      while (br.ready()) {
        char c=(char)br.read();
        if (c==first) {
          // Try to read delimeter.
          boolean broken=false;
          StringBuffer tried=new StringBuffer(""+c);
          for (int i=1;i<sep.length() && !broken;i++) {
            c=(char)br.read();
            tried.append(c);
            if (c!=sep.charAt(i))
              broken=true;
          }
          if (broken)
            current.append(tried);
          else {
            if (dotrim)
              v.add(current.toString().trim());
            else
              v.add(current.toString());
            JString.clearBuffer(current);
          }
        } else {
          current.append(c);
        }
      }
      br.close();
      if (dotrim)
        v.add(current.toString().trim());
      else
        v.add(current.toString());

      return v;

    } catch (Exception e) {

      if (JLib.Debugging) {
        String report="Files.readlinesfromfile("+f+"): "+e+" - returning null;";
        System.out.println(report);
        log+=report;
      }

      return null;

    }
  }

  public static String readStringfromfile(String fname) {
    return readStringFromFile(new File(fname));
  }
  public static String readStringFromFile(File f) {
    try {
      StringBuffer s=new StringBuffer();
      Reader r=new FileReader(f);
      while (true) {
        int i=r.read();
        if (i==-1)
          break;
        s.append(""+(char)i);
      }
      r.close();
      return ""+s;
    }
    catch (Exception e) {
      System.out.println("What should I do with exception "+e+" ?");
      log+="Files.readStringfromfile("+f+"):"+e;
    }
    return null;
  }

  public static boolean writelinestofile(Vector lines,String fname) {
    try {
      BufferedWriter br=new BufferedWriter(new FileWriter(new File(fname)));
      for (int i=0;i<lines.size();i++)
        br.write((String)lines.get(i)+"\n");
      br.close();
      return true;
      //System.out.println("Written file "+fname);
    }
    catch (Exception e) {
      System.out.println("Files.writelinestofile: "+e);
      log+="Files.writelinestofile("+fname+"):"+e;
      return false;
    }
  }

  public static boolean writeStringtofile(String s,String fname) {
    return writeStringtoFile(s,new File(fname));
  }
  public static boolean writeStringToFile(String s,File f) {
    return writeStringtoFile(s,f);
  }
  public static boolean writeStringtoFile(String s,File f) {
    log+="Writing file "+f+" size "+s.length()+"\n";
    try {
      BufferedWriter br=new BufferedWriter(new FileWriter(f));
      br.write(s);
      br.close();
      return true;
      //System.out.println("Written file "+fname);
    } catch (Exception e) {
      System.out.println("Files.writeStringtofile: "+e);
      log+="Files.writeStringtofile("+f+"):"+e;
      return false;
    }
  }

  public static boolean deltree(String f) {
    return deltree(new File(f));
  }
  public static boolean deltree(File f) {
    return deltree(f,true);
  }
  public static boolean deltree(File f,boolean show) {
    if (show) {
      System.out.println("Deleting \""+f+"\" and all subdirs");
    }
//    log+=f+"\n";
    boolean done=true;
    if (f.exists()) {
      log+="Deleting "+f+"\n";
      if (f.isDirectory()) {
        File[] fs=f.listFiles();
        for (int i=0;i<fs.length;i++) {
//          log+=""+fs[i]+"<br>";
          if (!deltree(fs[i],false)) {
            log+="deltree("+f+"): failed to delete tree "+fs[i]+"<br>\n";
            done=false;
//            return false;
          }
        }
      }
      if (!f.delete())
        done=false;
      if (!done) {
        log+="deltree("+f+"): failed to delete the file";
        if (f.isDirectory()) {
        File[] fs=f.listFiles();
        log+=" ("+fs.length+") { ";
        for (int i=0;i<fs.length;i++)
          log+=fs[i].getName()+", ";
        log+="}<br>\n";
        }
      }
//      else
//        log+=f+" deleted\n";
      return done;
    }
    return done;
  }

  public static List getrecursive(String s) {
    List l=new Vector();
    File f=new File(s);
    getfilesrecursive2(f,l);
    return l;
  }

  private static void getfilesrecursive2(File f,List l) {
    String[] indir=f.list();
    if (indir==null) { // File
      l.add(f);
    } else {
      for (int i=0;i<indir.length;i++)
        getfilesrecursive2(new File(f,indir[i]),l);
    }
  }

  public static boolean isPosixLink(File f) {
    // System.err.print("*");
    try {
      String s=JLib.execute("/bin/ls -ld "+f.getAbsolutePath());
      // System.out.println(s);
      return s.startsWith("l");
    } catch (Exception e) {
      // Would really like to check the filesystem ;)
      return false;
    }
  }

  public static List posixLinks(List fs) {
    // It seems a lot of arguments is too much!
    System.err.print("~%~");
    StringBuffer s=new StringBuffer();
    s.append("/bin/ls -ld");
    for (int i=0;i<fs.size();i++) {
      File f=(File)fs.get(i);
      // A problem, is we surround file by "s, Java parser
      // fucks up command line!
      s.append(" "+f.getAbsolutePath()+"");
    }
    String r=JLib.execute(s.toString());

    String[] rs=JString.split(r,"\n");
    if (rs.length!=fs.size()) {
      System.out.println("Files.posixLinks: # results "+rs.length+" != # files "+fs.size());
      System.out.println("Files.posixLinks: files: "+fs);
      System.out.println("Files.posixLinks: result: "+r);
      System.out.println("Files.posixLinks: results: "+rs);
      return null;
    }

    List lnks=new Vector();
    for (int i=0;i<rs.length;i++) {
      if (rs[i].startsWith("l"))
        lnks.add(fs.get(i));
    }
    return lnks;
  }

  public static void touch(File f) {
    if (!f.exists())
      Files.writeStringToFile("Touched by jlib.Files.touch() "+new Date(),f);
    else {
      try {
        f.setLastModified(new Date().getTime());
      } catch (Exception e) { JLib.report(e); }
    }
  }

  public static void main(String[] args) {
    if (args[0].equals("mod"))
      System.out.println(""+new File(args[1]).lastModified());
  }

}