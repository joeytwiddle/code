/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.geocities.com/SouthBeach/5144 */

// Now uses a tree search structure and tries to follow

import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;
import java.io.*;
import java.util.*;

//import NujuFrame;
//import NujuComWin;

import jlib.Files;
import jlib.Satisfier;
import jlib.JString;
import jlib.JCurses;

public class Columnise {

  int cols=70;

  public static void main(String args[]) {
    Columnise u=new Columnise();
    if (args.length==0) {
      //System.out.println("Unicorn: Unique or not?  By Joey (pclark@cs.bris.ac.uk) =)");
      System.out.println("Usage: unicorn <files> [-keep-unique-only] [-write-unique] [-quiet] [-ignore-comments] [-show-lines]");
    }
    u.columnise(args);
  }

  Columnise() {
  }

  public int firstAlpha(String s) {
    for (int i=0;i<s.length();i++) {
      char c=s.charAt(i);
      if ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz".indexOf(c)>0)
        return i;
    }
    return -1;
  }

  public String trimleft(String s) {
    for (int i=0;i<s.length();i++) {
      if (s.charAt(i)!=' ')
        return s.substring(i);
    }
    return s; // A load of spaces!
  }

  public Vector split(String s) {
    Vector v=new Vector();
    String left=s.substring(0,cols);
    int i=left.lastIndexOf(" ");
    if (i==-1) {
      s=s.substring(0,cols-1)+"-"+s.substring(cols-1);
      i=cols;
    }
    left=s.substring(0,i);
    String right=s.substring(i);
    int j=firstAlpha(left);
    if (j>-1) {
      right=trimleft(right);
      right=JString.repeat(" ",j)+"  "+right;
    }
    v.add(left);
    v.add(right);
    return v;
  }
  public void columnise(String args[]) {
    Vector lines=Files.readlinesfromfile(args[0]);
    for (int i=0;i<lines.size();i++) {
      String s=(String)lines.remove(i);
      while (s.length()>cols) {
        Vector res=split(s);
        lines.insertElementAt(res.get(0),i);
        i++;
        s=(String)res.get(1);
      }
      lines.insertElementAt(s,i);
    }
    for (int i=0;i<lines.size();i++) {
      System.out.println((String)lines.get(i));
    }
  }

}