import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import jlib.*;
import nuju.*;

public class WatchXmms {

  public static void main(String[] args) {

    try {

    // Process p=Runtime.getRuntime().exec("tail -f "+JLib.JPATH+"/tmp/xmms-out.txt");
    // System.out.println("Got here");
    // InputStream in=p.getInputStream();
    File f=new File(JLib.JPATH+"/tmp/xmms-output.txt");
    InputStream in=new FileInputStream(f);
    // Skip to end of file
    long skipped=in.skip(f.length());
    System.out.println(skipped+"/"+f.length());
    while (true) {
      while (in.available()>0) {
        System.out.print((char)in.read());
      }
      JThread.sleep(1);
    }

    } catch (Exception e) {
      System.out.println(""+e);
    }

  }

}