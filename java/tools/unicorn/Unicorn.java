/* All my source code is freely distributable under the GNU public licence.
   I would be delighted to hear if have made use of this code.
   If you make money with this code, please give me some!
   If you find this code useful, or have any queries, please feel free to
   contact me: pclark@cs.bris.ac.uk / joeyclark@usa.net
   Paul "Joey" Clark, hacking for humanity, Feb 99
   www.cs.bris.ac.uk/~pclark / www.geocities.com/SouthBeach/5144 */

// Fix: It appears writeunique and keepunique are redundant.
// Fix: and ignoreComments because we use File.readlines() to parse
// 13/Apr/01: Unicorn now supports matching of non-unique lines
//            Comparisons made by character histogram or transition histogram of string.
// Now uses a tree search structure.

import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;
import java.io.*;
import java.util.*;

//import NujuFrame;
//import NujuComWin;

import jlib.*;
import jlib.Files;
import jlib.Satisfier;
import jlib.JString;
import jlib.JCurses;

class StringHash {

  final static int sz=128-32;

  String str;

  int[] cnt=new int[sz];

  int[][] trans=new int[sz][sz];

  StringHash(String s) {
    str=s;
    accumulate();
  }

  void accumulate() {
    // cnt[i] = number of characters 'c' in str, where 'c'=(char)i+32
    for (int i=0;i<str.length();i++) {
      int c=(int)str.charAt(i);
      int j=c-32;
      if (j>=0 && j<sz)
        cnt[j]++;
    }
    // trans[i][j] = number of times a transition "cd" occurrs where "d"=(char)j+32
    for (int i=0;i<str.length()-1;i++) {
      int a=(int)str.charAt(i)-32;
      int b=(int)str.charAt(i+1)-32;
      if (a>=0 && a<sz
       && b>=0 && b<sz)
        trans[a][b]++;
    }
  }

  double similarity(StringHash o) {
    // return similarityHistogram(o);
    return similarityTransition(o);
  }

  double similarityHistogram(StringHash o) {
    int s=0;
    for (int i=0;i<sz;i++)
      s=s+JMaths.square(cnt[i]-o.cnt[i]);
    // s=0 means strings appear identical
    // otherwise can reach str.length()^2 + o.str.length()^2
    return 1.0-(double)s/(double)(JMaths.square(str.length())+JMaths.square(o.str.length()));
  }

  double similarityTransition(StringHash o) {
    int s=0;
    for (int i=0;i<sz;i++)
    for (int j=0;j<sz;j++)
      s=s+JMaths.square(trans[i][j]-o.trans[i][j]);
    // s=0 means strings appear identical
    return 1.0-(double)s/(double)(JMaths.square(str.length()-1)+JMaths.square(o.str.length()-1));
  }

}



class LineRecord implements Comparable {
  String line;
  int lineno;
  LineRecord(String ss,int l) {
    line=ss;
    lineno=l;
  }
  public int compareTo(Object o) {
    return line.compareTo(((LineRecord)o).line);
  }
}

class LRSat implements Satisfier {
  String search;
  int start;
  LRSat(String s,int i) {
    search=s;
    start=i;
  }
  public boolean satisfies(Object o) {
    LineRecord lr=(LineRecord)o;
    if (lr.lineno>=start)
      if (search.trim().equals(lr.line.trim()))
        return true;
    return false;
  }
}

class BinTree {
  Object o;
  BinTree left=null;
  BinTree right=null;
  BinTree() {
    o=null;
  }
  BinTree(Object oo) {
    o=oo;
  }
  public void add(Comparable n) {
    if (o==null)
      o=n;
    else
      if (n.compareTo(o)>0)
        if (right==null)
          right=new BinTree(n);
        else
          right.add(n);
      else // left takes ==
        if (left==null)
          left=new BinTree(n);
        else
          left.add(n);
  }
  /* Vector findfirst(Satisfier s) {
    return null;
  } */ 
  Object findfirst(String s,int i) {
    LineRecord lr=(LineRecord)o;
    if (lr.lineno>=i)
      if (lr.line.equals(s)) //  && lr.line.length()>1)
        return o;
    if (s.compareTo(lr.line)<=0 && left!=null)
      return left.findfirst(s,i);
    if (s.compareTo(lr.line)>0 && right!=null)
      return right.findfirst(s,i);
    return null;
  }
}

class Unicorn {

  boolean keepuniqueonly=false;
  boolean writeunique=false;
  boolean quiet=false;
  boolean ignorecomments=false;
  boolean showlines=false;
  boolean nocurses=false;
  boolean label=false;

  public static void main(String args[]) {
    Unicorn u=new Unicorn();
    if (args.length==0) {
      //System.out.println("Unicorn: Unique or not?  By Joey (pclark@cs.bris.ac.uk) =)");
      System.out.println("Usage: unicorn <files> [-label] [-quiet] [-show-lines] [-no-curses] [-match-similar] [-separator <delimiter>]");
    }
    u.comparefiles(args);
  }

  Unicorn() {
  }

  String simplify(String s) {
    s=s.trim();
    int i=s.indexOf("//");
    if (i>0)
      s=s.substring(i+2);
    return s;
  }

  public static BinTree treefromlines(Vector lines) {
    BinTree m=new BinTree();
    for (int i=0;i<lines.size();i++) {
      m.add(new LineRecord(((String)lines.get(i)),i));
    }
    return m;
  }

  int MODEfindUnique=0;
  int MODEmatchSimilar=1;
  public void comparefiles(String args[]) {
    int mode=MODEfindUnique;
    String sep="\n";
    Vector files=new Vector();
    Vector readfiles=new Vector();
    Vector trees=new Vector();
    // Parse arguments
    for (int i=0;i<args.length;i++) {
//      System.out.println(args[i]);
      if (args[i].equals("-keep-unique-only"))
        keepuniqueonly=true;
      if (args[i].equals("-label"))
        label=true;
      else if (args[i].equals("-write-unique"))
        writeunique=true;
      else if (args[i].equals("-quiet"))
        quiet=true;
      else if (args[i].equals("-ignore-comments"))
        ignorecomments=true;
      else if (args[i].equals("-show-lines"))
        showlines=true;
      else if (args[i].equals("-match-similar"))
        mode=MODEmatchSimilar;
      else if (args[i].equals("-no-curses")) {
        JCurses.noCurses();
        nocurses=true;
      } else if (args[i].equals("-sep")) {
        i++;
        sep=args[i];
      } else {
        files.add(args[i]);
        //  Vector lines=Files.readlinesfromfile(args[i]);
          // Own function removes whitespace at ends also
        Vector lines=Files.readlistfromfile(new File(args[i]),sep,true);
        if (mode==MODEfindUnique) {
          readfiles.add(lines);
          trees.add(treefromlines(lines));
        } else {
          Vector hashes=new Vector();
          for (int j=0;j<lines.size();j++)
            hashes.add(new StringHash((String)lines.get(j)));
          readfiles.add(hashes);
        }
      }
    }

    // Investigate files

    for (int f=0;f<files.size()-1;f++) {

      Vector thislines=(Vector)readfiles.get(f);
      Vector complines=(Vector)readfiles.get(f+1);

      if (mode==MODEfindUnique) {

      BinTree thisfile=(BinTree)trees.get(f);
      BinTree comparefile=(BinTree)trees.get(f+1);
      System.out.println("Comparing "+files.get(f)+" to "+files.get(f+1));
      int i=0;
      int j=0;
      boolean stop=false;
      while (!stop) {
        String s=(String)thislines.get(i);
        // Find occurrance of s in the other file (starting from j) 
        Object res=comparefile.findfirst(s,j);
        if (res==null) {
          // Is it in there at all?
          Object res2=comparefile.findfirst(s,0); // inefficient because seaches j onwards a second time
          if (res2==null) // LINE MISSING
            System.out.println(JCurses.darkred+"xx "+s+JCurses.off);
          else // LINE MOVED (is above where we are currently looking)
            System.out.println(JCurses.lightblue+"^^ "+JCurses.grey+s+JCurses.off);
            // System.out.println("  X did not find in "+files.get(f));
        } else {
          LineRecord lr=(LineRecord)res;
          if (lr.lineno!=j) { // LINES IN BETWEEN
            // System.out.println("> found "+(lr.lineno-j)+" lines later");
            for (j=j;j<lr.lineno;j++) {
              String insertedline=(String)complines.get(j);
              String elsewhere=( thisfile.findfirst(insertedline,0)==null ?
                                 "" : JCurses.grey );
              System.out.println(JCurses.green+">> "+elsewhere+insertedline+JCurses.off);
            }
            j=lr.lineno;
            // System.out.println(">");
          }
          // DUPLICATE LINE
          System.out.println(JCurses.grey+".. "+s+JCurses.off);
          j++;
        }
        i++;
        if (i>=thislines.size())
          stop=true;
      }
      System.out.println("");

      } else {

        Vector notmatchedinb=(Vector)complines.clone();

        for (int i=0;i<thislines.size();i++) {
          StringHash sh=(StringHash)thislines.get(i);
          StringHash best=null;
          double bestsimilarity=0.0;
          for (int j=0;j<complines.size();j++) {
            StringHash osh=(StringHash)complines.get(j);
            double sim=sh.similarity(osh);
            if (sim>bestsimilarity) {
              bestsimilarity=sim;
              best=osh;
            }
          }
          if (!sh.str.trim().equals(best.str.trim()) || keepuniqueonly) {
            if (label)
              System.out.println(">>>>>>>>>>");
            System.out.println(sh.str);
            if (label)
              System.out.println("========== "+sh.similarity(best));
            System.out.println(best.str);
            if (label)
              System.out.println("<<<<<<<<<<");
            System.out.println();
          }
          notmatchedinb.remove(best);
        }

        System.out.println();
        System.out.println("Lines in "+files.get(f+1)+" which were not matched:");
        for (int i=0;i<notmatchedinb.size();i++)
          System.out.println(""+((StringHash)notmatchedinb.get(i)).str);

      }

    }


    /*

    boolean allunique=true;
    for (int i=0;i<files.size();i++) {
      Vector lines=(Vector)readfiles.get(i);
      if (!quiet)
        System.out.println((String)files.get(i)+" :");
      // Is this file unique, or is it totally contained within other files?
      boolean funique=false;
      Vector uniquelines=new Vector();
      for (int j=0;j<lines.size();j++) {
        String line=(String)lines.get(j);
        // Is this line unique, or does it appear in another file?
        boolean lunique=true;
        for (int k=0;k<files.size() && lunique;k++) {
          if (k!=i) {
            Vector clines=(Vector)readfiles.get(k);
            for (int l=0;l<clines.size() && lunique;l++)
              if (simplify(line).equals((simplify((String)clines.get(l)))))
                lunique=false;
          }
        }
        if (lunique) {
          funique=true;
          uniquelines.add(line);
          if (!quiet)
            System.out.println((showlines ? (j+1)+": " : "  ")+line);
        }
      }
      if (!funique)
        allunique=false;
      if (!quiet) {
        String not="";
        if (!funique)
          not="not ";
        System.out.println((String)files.get(i)+" is "+not+"unique.");
      }
      if (writeunique)
        writelinestofile((String)files.get(i)+".uniquelines",uniquelines);
      if (keepuniqueonly && !funique) {
        if (i==(files.size()-1) && allunique)
          System.out.println((String)files.get(i)+" has been left, but is not unique.");
        else {
          File f=new File((String)files.get(i));
          f.delete();
        }
      }

    }
    NujuFrame.present(files);
    NujuFrame.present(readfiles);
    */

  }

  private Vector readlinesfromfile(String fname) {
    Vector v=new Vector();
    try {
      BufferedReader br=new BufferedReader(new FileReader(new File(fname)));
      while (br.ready()) {
        String tmp=br.readLine().trim();
        // Strip comments
        if (ignorecomments) {
          while (tmp.indexOf("/*")>=0) {
            int i=tmp.indexOf("/*");
            v.add(tmp.substring(0,i));
            String rest=tmp.substring(i+2);
            while (tmp.indexOf("*/")==-1) {
              tmp=br.readLine().trim();
            }
            tmp=tmp.substring(tmp.indexOf("*/")+2);
          }
        }
        v.add(tmp);
      }
      br.close();
    }
    catch (Exception e) {
      System.out.println("What should I do with exception "+e+" ?");
    }
    return v;
  }

  private void writelinestofile(String fname,Vector lines) {
    try {
      BufferedWriter br=new BufferedWriter(new FileWriter(new File(fname)));
      for (int i=0;i<lines.size();i++)
        br.write((String)lines.get(i)+"\n");
      br.close();
      //System.out.println("Written file "+fname);
    }
    catch (Exception e) {
      System.out.println("What should I do with exception "+e+" ?");
    }
  }

}