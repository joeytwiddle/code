package changetheworld; import jlib.JString;

import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;
import java.io.*;
import java.util.Vector;
import java.util.*;

public class Files {

  public static String log="<b>Files Log:</b>\n<br>";

  public static Vector readlinesfromfile(String fname) {
    try {
      Vector v=new Vector();
      BufferedReader br=new BufferedReader(new FileReader(new File(fname)));
      while (br.ready()) {
        String tmp=br.readLine();
        // Strip spaces
//        tmp=tmp.trim();
        v.add(tmp);
        // System.out.println("Read line "+tmp);
      }
      br.close();
      return v;
    }
    catch (Exception e) {
      System.out.println("What should I do with exception "+e+" ?");
      log+="Files.readlinesfromfile("+fname+"):"+e;
    }
    return null;
  }

  public static String readStringfromfile(String fname) {
    try {
      String s="";
      BufferedReader br=new BufferedReader(new FileReader(new File(fname)));
      while (br.ready()) {
        String tmp=br.readLine();
        // Strip spaces
//        tmp=tmp.trim();
        s+=tmp+"\n";
        // System.out.println("Read line "+tmp);
      }
      br.close();
      return s;
    }
    catch (Exception e) {
      System.out.println("What should I do with exception "+e+" ?");
      log+="Files.readStringfromfile("+fname+"):"+e;
    }
    return null;
  }

  static boolean writelinestofile(Vector lines,String fname) {
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
    log+="Writing file "+fname+" size "+s.length()+"\n";
    try {
      BufferedWriter br=new BufferedWriter(new FileWriter(new File(fname)));
      br.write(s);
      br.close();
      return true;
      //System.out.println("Written file "+fname);
    }
    catch (Exception e) {
      System.out.println("Files.writeStringtofile: "+e);
      log+="Files.writeStringtofile("+fname+"):"+e;
      return false;
    }
  }

  public static boolean deltree(File f) {
    System.out.println(""+f);
//    log+=f+"\n";
    boolean done=true;
    if (f.exists()) {
      log+="Deleting "+f+"\n";
      if (f.isDirectory()) {
        File[] fs=f.listFiles();
        for (int i=0;i<fs.length;i++) {
//          log+=""+fs[i]+"<br>";
          if (!deltree(fs[i])) {
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

  public static boolean deltree(String f) {
    return deltree(new File(f));
  }

}