package jlib;

import java.net.URL;
import java.net.URLConnection;
import java.lang.String;
import java.io.InputStream;
import java.io.File;

/**
 *  Warning about URL.equals!
 *  It considers abc.name.com to be equal to xyz.name.com .
**/

public class JNet {

  public static URL url(File f) {
    return url(""+f.getAbsoluteFile());
  }
  public static URL url(String s) {
    int i=s.indexOf(":");
    if (i==-1 || i>6) { // No protocol
      if (s.startsWith("/"))
        s="file://"+s;
      else
        s="http://"+s;
    }
    System.out.println(""+s);
    try {
      return new URL(s);
    } catch (Exception e) {
      System.out.println("JNet.url( "+s+" ): "+e);
      return null;
    }
  }

  public static URL url(URL u,String s) {
    try {
      return new URL(u,s);
    } catch (Exception e) {
      System.out.println("JNet.url( "+u+" , "+s+" ): "+e);
      return null;
    }
  }

  public class URLResponse {
    URL url;
    public String type="broken";
    public String content="broken";
    URLResponse(URL u) {
      url=u;
      get();
    }
    void get() {
      try {
        URLConnection con=url.openConnection();
        type=con.getContentType();
        content=streamFrom((InputStream)con.getContent());
      } catch (Exception e) {
        System.out.println(""+e);
      }
    }
  }


  /** Streams the InputStream provided until it ends, returning the read data as String.
   *  Politely does a sleep(10) when the stream has nothing to give.
  **/
  public static String streamFrom(InputStream in) {
    try {
      // InputStream in=(InputStream)o;
      StringBuffer s=new StringBuffer();
      // while (b.available()>0)
      //   s.append((char)b.read());
      boolean done=false;
      while (!done) {
        try {
          JThread.sleep(1);
          int c=in.read();
          if (c<0)
            done=true;
          else {
            s.append((char)c);
            // System.out.println("Got char "+(int)c+" '"+(char)c+"'");
            int av=in.available();
            if (av>0) {
              byte[] bs=new byte[av];
              int read=in.read(bs);
              if (read>0)
                s.append(new String(bs));
            }
          }
        } catch (Exception e) {
          System.out.println("JNet.streamFrom(): "+e);
          done=true;
        }
      }
      return s.toString();
    } catch (Exception e) {
      System.out.println(""+e);
      return null;
    }
  }

  public static String getUrlContent(URL url) {
    try {
      Object o=url.getContent();
      return streamFrom((InputStream)o);
    } catch (Exception e) {
      System.out.println(""+e);
      return null;
    }
  }

  public static String relative(URL src,URL dest) {
    String[] ss=JString.splitSharedAt(src.toString(),dest.toString(),'/');
    String base=ss[0];
    String endsrc=ss[1];
    String enddest=ss[2];
    int updirs=JString.countOf('/',endsrc);
    String ref=src.getRef();
    System.out.println("*** JNet.relative: Got Ref \""+ref+"\" from "+src+" to "+dest);
    String refout=( ref!=null
      ? (ref.length()>0 ? "#"+ref : "" )
      : "" );
    return JString.repeat("../",updirs)+enddest+refout;
  }

}