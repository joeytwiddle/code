// Admit it: the extension you get from the URL is irrelevent
// since it might be a shell or CGI query

// Two stage process needed to check if file should be retrieved:
//   1) with knowledge before opening connection
//   2) with knowledge after opening connection

// Clean up the following:
//   The Trawlet calls the Strategy to find out information
//   about itself, which it then sets for itself.
// A bit messy, especially when we call fileName() twice.

// Put actual InputStream parsing in another class from Trawlet,
// and let it have an idle timeout and a size limit
// and outputs: to file, to String for parsing ...

import java.io.*;
import java.lang.*;
import java.util.*;
import java.net.*;
// import javax.swing.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import javax.swing.text.html.parser.ParserDelegator;

import jlib.Files;
import jlib.Profile;
import jlib.JString;
import jlib.JTypes;
import jlib.JList;
import jlib.JNet;
import nuju.*;

import MyParserCallback;

public class TrawlerStrategy {

  URL startURL;
  int maxdepth;
  int maxhosts; // maximum connections per host
  int maxtotal; // maximum connections in total (ie. max the local machine can handle)
  File destdir;
  int timeout=60; // seconds of no input before Trawlet is abandoned
  int largestparsed;
  List extensions; /** Only in use if non-null and non-empty. **/
  boolean showStream,showProgress;
  boolean localLink;
  URL similar;
  int maxattempts=4; // should depend on type of error!
  boolean subdirsOnly;
  public Vector forbiddenStarts; // Strings which URLs may not start with
    // Added to by TrawlerGUI at present

  public TrawletManager manager=null;

  public TrawlerStrategy(String u,int d,int h,int t,int lp,List es,File dd,boolean ss,boolean sp,boolean ll,URL f,boolean sdo,Vector fbs) {
    startURL=JNet.url(u);
    maxdepth=d;
    maxhosts=h;
    maxtotal=t;
    largestparsed=lp;
    extensions=es;
    if (extensions.contains("html") && !extensions.contains("htm"))
      extensions.add("htm");
    destdir=dd;
    showStream=ss;
    showProgress=sp;
    localLink=ll;
    similar=f;
    subdirsOnly=sdo;
    forbiddenStarts=fbs;
    System.out.println("Got extensions "+extensions+" and forbiddenS "+forbiddenStarts);
  }

  public boolean startsWith(URL u,URL s) {
    return startsWith(u,s.toString());
  }
  public boolean startsWith(URL u,String s) {
    return u.toString().startsWith(s);
  }
  public boolean startsWith(String s,URL u) {
    return u.toString().startsWith(s);
  }

  /** Is this URL forbidden?
  **/
  public boolean urlOk(URL u) {
    // Already trawled
    for (int i=0;i<manager.urls.size();i++) {
      URL au=(URL)manager.urls.get(i);
      if (urlsEquivalent(au,u)) {
        System.out.println("Already got "+u);
        return false;
      }
    }
    // Outside the subdir domain
    if (subdirsOnly) {
      String dir=u.toString();
      if (!dir.endsWith("/"))
        dir=dir.substring(0,dir.lastIndexOf("/")+1);
      if (!startsWith(dir,startURL)) {
        System.out.println("Outside subdir");
        return false;
       }
    }
    // Inside a forbidden domain
    for (int i=0;i<forbiddenStarts.size();i++)
      if (startsWith(u,(String)forbiddenStarts.get(i))) {
        System.out.println("Forbidden domain");
        return false;
      }
    return true;
  }

  /** Is this extension in the list, or is the list not in use?
  **/
  public boolean satisfiesExtensions(String ext) {
    if (extensions==null)
      return true;
    if (extensions.size()==0) // disgusting
      return true;
    // String ext=JString.afterlast(f.getName(),".");
    // System.out.println("EXT "+ext+": "+extensions.contains(ext.toLowerCase()));
    return extensions.contains(ext.toLowerCase());
  }

  /** Is is worthwhile opening a connection to the Trawlet?
   *  <p>It is currently deemed worthwhile if extension is empty or satisfied by list.
  **/
  public boolean open(Trawlet t) {
    // Always open first URL
    if (t.depth==1)
      return true;
    if (t.extension==null)
      return true;
    if (t.extension.equals(""))
      return true;
    /* if (t.extension.equals("html"))
      return true;
    if (t.extension.equals("htm"))
      return true; */
    if (satisfiesExtensions(t.extension))
      return true;
    if (notleaf(t))
      return true;
    return false;
  }

  /** Do we want to write the contents to a file? **/
  public boolean write(Trawlet t) {
    // Must be correct extension
    // System.out.println("*** "+satisfiesExtensions(t)+" "+t.extension+" "+t.destFile.getName());
    if (urlsEquivalent(t.url,similar))
      return false;
    // if (t.contentType!=null)
      if (t.contentType.startsWith("text/html"))
        if (satisfiesExtensions("html"))
          return true;
    if (satisfiesExtensions(t.extension))
      return true;
    System.out.println("Not writing ********************* "+t.contentType);
    return false;
    // If not in list, if we're reading it, we may as well write it!
    // return keep(t);
  }

  boolean notleaf(Trawlet t) {
    return (t.depth<maxdepth);
  }

  /** Should we keep a copy of the URL to parse for more links?
   *  <!--<p>At the moment, this-->
  **/
  public boolean keep(Trawlet t) {
    if (urlsEquivalent(t.url,similar))
      return true;
    // Always parse first URL
    if (t.depth==1)
      return true;
    if (notleaf(t) && t.contentType.startsWith("text/html"))
      return true;
    // If it isn't a specfied extension, don't parse it
    if (!satisfiesExtensions(t.extension))
      return false;
    // Always take HTML documents
    /* if (t.contentType.startsWith("text/html"))
      return true; */
    // If small, take it anyway!
    // Note we allow size=-1 unknown!
    // but it will be caught later.
    /* if (t.contentLength<largestparsed) {
      t.parsingUnsure=true;
      return true;
    } */
    return false;
  }

  File fileName(Trawlet t) {

    if (t.fileName==null) {

    String add =
      ( t.url.toString().endsWith("/") ?
        "/index.html" :
        "" );
    String from=t.url.getFile()+add;
    try {

    String fname="";

    // Remove illegal characters
    // final String legalchars=" _.,-/";
    // We do not allow space from the default context
    final String legalchars=JString.before(Files.legalchars," ")+JString.after(Files.legalchars," ");
    for (int i=0;i<from.length();i++) {
      char c=from.charAt(i);
      /* if ( (c>='A' && c<='Z')
        || (c>='a' && c<='z')
        || (c>='0' && c<='9')
        || legalchars.indexOf(c)>-1 ) */
      if (JString.containedBy(""+c,legalchars))
        fname+=c;
      else
        fname+="_";
    }

    // What extension should the file have?
    // System.out.println("Type: "+t.contentType);
    String ext=null;
    /*if (t.contentType!=null) { // Get extension from MIME type
      if (t.contentType.startsWith("text/html"))
        ext="html";
    }
    // Ensure correct extension
    if (ext!=null) {
      if (!fname.endsWith("."+ext))
        fname+="."+ext;
    }*/

    // Evaluate path and file where fname may contain '/'
    File basedir=new File(destdir,t.url.getHost());
    File f=new File(basedir,fname);
    // Correct them to get real path and filename
    basedir=f.getParentFile();
    fname=f.getName();

    try {
    // Ensure name not too long
    final int maxlen=30;
    if (fname.length()>30) {
      String[] lr=Files.split(fname); String left=lr[0]; String right=lr[1];
      if (right.length()>0) { right="."+right; }
      // System.out.println("*** >"+left+"< >"+right+"<");
      left=left.substring(0,maxlen-right.length());
      // System.out.println("*** reduced to "+left+" and "+right);
      fname=left+right;
      f=new File(basedir,fname);
    }
    } catch (Exception e) {
      System.out.println("*** A "+e);
    }

    try {
    // Ensure unique filename
    if (f.exists()) {
      fname=f.getName();
      String[] lr=Files.split(fname); String left=lr[0]; String right=lr[1];
      if (right.length()>0) { right="."+right; }
      int num=2;
      while ((f=new File(basedir,left+num+right)).exists()) {
        // System.out.println("Already exists: "+left+num+right);
        num++;
        // System.out.println("  Trying:       "+left+num+right);
      }
      System.out.println("Second version! "+num+": "+f);
    }
    } catch (Exception e) {
      System.out.println("*** B");
    }

    t.fileName=f;

    } catch (Exception e) {
      System.out.println("TrawlerStrategy.fileName("+from+"): ERROR ****** "+e);
      return null;
    }

    }

    return t.fileName;

  }

  public void retrieved(Trawlet t,TrawletManager manager) {

    // Save the content
    // Files.writeStringtofile(t.content,f.toString());
    // Now done by Trawlet if write() is true

    // Trawl all links
    if (t.depth<maxdepth && t.parsing) {

      MyParserCallback myparser=new MyParserCallback(manager,t,t.url,t.content,t.depth,true,true);

      if (t.saving) {
        t.destFile.getParentFile().mkdirs();
        String data=( localLink ? myparser.copy.toString() : t.content );
        Files.writeStringtofile(data,""+t.destFile);
      }

      System.out.println("Parser found "+myparser.links.size()+" links.");
      System.out.println();

      Vector/*List*/ urls=myparser.links;
      JList.shuffle(urls);
      for (int i=0;i<urls.size();i++) {
        URL u=(URL)urls.get(i);
        manager.goget(u,t.depth+1);
      }

    } else {
      System.out.println();
    }

  }

  public static boolean urlsEquivalent(URL u,URL v) {
    String a=JString.before(u.toString(),"#");
    String b=JString.before(v.toString(),"#");
    return (a.equals(b));
  }

}

