package changetheworld; import jlib.JString;

import java.util.Date;

import jlib.Log;

public class Profiler {
  String what=null;
  Date start=null;
  public static String log="";
  long stored=0;
  boolean stopwatch=false;

  Profiler(String w) {
//    Profiler(w,false);
    if (CTW.Debugging) {
      what=w;
      start=new Date();
    }
  }

  Profiler(String w,boolean b) {
    if (CTW.Debugging) {
      what=w;
      start=new Date();
      stopwatch=b;
      if (stopwatch) {
        stop();
        stored=0;
      }
    }
  }

  public void clear() {
    stop();
    stored=0;
  }

  public void stop() {
    Date end=new Date();
    stored+=(end.getTime()-start.getTime());
//    if (stored==0)
//      stored=-1;
  }
  public void start() {
    start=new Date();
  }
  public void end() {
    if (CTW.Debugging) {
      Date end=new Date();
      long gone=end.getTime()-start.getTime();
      if (stopwatch)
        gone=stored;
      log+="<table width=\"100%\"><tr><td>"+XML.encode(what)+"</td><td align=\"right\">"+gone+"ms</td></tr></table><br>\n";
      Log.report(what+" took "+gone+"ms");
    }
  }
}
