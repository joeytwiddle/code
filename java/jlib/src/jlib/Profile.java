package jlib;

import java.util.*;
import java.lang.*;
import jlib.*;

import changetheworld.CTW;

  /**
   * Profiling class.  Use this class to find out
   * which bits of your code are using the most time.
   * <p>Instructions for use:
   * <blockquote>
   * <p>At the beginning of your run, call Profile.clear().  The Profiler will start running.
   * <p>If you want to know how much time is spent between a few lines of code, insert
   * Profile.start and Profile.end calls above and below the lines
   * you are interested in.
   * <p>For example:
   * <blockquote><tt>
   * ...
   * <br>Profile.start("first suspect: mybruteforcesearch"); // The keyname for this profile
   * <br>Answer ans=Question.mybruteforcesearch(); // The region of code I wish to profile
   * <br>Profile.stop();
   * <br>...
   * <br>Profile.start("second suspect: visualisation for user");
   * <br>UserVis.displaynewdata(ans); // Another bit of code
   * <br>Profile.stop();
   * <br>...
   * </tt></blockquote>
   * <p>The profiler will count the number of milliseconds spent on the two different
   * bits of code above.
   * <p>At the end of your, run, call Profile.report() to get a String reporting the
   * time spent on each of your selections of code.  For example:
   * <table border="1">
   * <tr><td align="center">first suspect: mybruteforcesearch</td>
   *     <td align="right">574534 ms</td></tr>
   * <tr><td align="center">second suspect: visualisation for user</td>
   *     <td align="right">218675 ms</td></tr>
   * <tr><td align="center">The rest</td>
   *     <td align="right">1322 ms</td></tr>
   * </table>
   * Total of above = 12770 ms with actual total = 13917 ms<br>
   *   This means 1147 ms spent in Profile.<br>
   * <br>
   * </blockquote>
   * <p>The key "The rest" tells you how long during the run was spent on code that was not profiled.
   * If this has taken more time than the code you have profiled,
   * then you are looking in the wrong place for your inefficiencies!
   * <p>Since a java.util.Map is used to store and recall the names you provide,
   * you may reuse these keys in different parts of the code.  This will lump the profiles for
   * the two different bits of code together under the same name.
   * <p>If a profile for a different name is started whilst a profile is already occurring for
   * the current code, the profiler for the earlier region of code is paused until the profile
   * for the new region has been finished.
   * <p>You <i>must</i> ensure that all profiles that are started are also stopped, otherwise the
   * profile tree will get confused as to what it is profiling!
   * This is a danger in the presence of Exceptions, unless you put the start and stop above and below the try catch statement.
   * <p>You can use stop(String key) to make the profiler check at runtime that you are indeed trying to stop the correct (current/last) profile.
   * Errors regarding problems with the profile tree will be reported in the report() and in jlib.Log.
   **/

public class Profile {

  static String mode="";
  static long time=-1;
  static Map modes=new HashMap();
  static List pastmodes=new ArrayList();

  static long cleartime=-1;

//  public final static boolean profilingOn=CTW.Debugging;
  public final static boolean profilingOn=true;

  public static void clear() {
   if (profilingOn) {
    modes=new HashMap();
    pastmodes=new ArrayList();
    mode="";
    time=cleartime=new Date().getTime();
/*    Set ks=modes.keySet();
    Iterator i=ks.iterator();
    while (i.hasNext()) {
      String key=(String)i.next();
      modes.put(key,new Long(0));
    }
    time=new Date().getTime(); */
    start("The rest");
   }
  }

  /**
   * @param s Your own choice of keystring indicating the piece of code being profiled.  "The Rest" and "" are reserved.
   **/
  public static void start(String m) {
   if (profilingOn) {
    juststop();
    mode=m;
    pastmodes.add(m);
    time=new Date().getTime();
//    Log.report("<font color=\"#0080ff\">Starting \""+m+"\"</font>");
   }
  }

  private static long juststop() { // stops the current, doesn't know whether recursing forward or back
   if (profilingOn) {
    long now=new Date().getTime();
    long sofarnow=-1;
    try {
      Long sofar=(Long)modes.get(mode);
      sofarnow=sofar.longValue()+(now-time);
    } catch (Exception e) {
      sofarnow=now-time;
    }
    modes.put(mode,new Long(sofarnow));
    time=new Date().getTime();
    return sofarnow;
   }
   return -1;
  }

  public static void stop() {
    stop(null);
  }

  public static long stop(String expect) {
   if (profilingOn) {
    long returnTime=juststop();
//    Log.report("<font color=\"#0080ff\">Stopping \""+mode+"\" ("+pastmodes+")</font>");
    mode="";
    if (pastmodes.size()>=1) {
      String lastmode=(String)pastmodes.remove(pastmodes.size()-1);
      if (expect!=null)
        if (!lastmode.equals(expect))
          Log.error("Profile.stop(): Expected "+expect+" but got "+lastmode+" with rest = "+pastmodes);
      if (pastmodes.size()>=1) {
        mode=(String)pastmodes.get(pastmodes.size()-1);
//        Log.report("<font color=\"#0080ff\">Returning to \""+mode+"\" ("+pastmodes+")</font>");
      }
    }
    if (mode.equals(""))
      Log.error("Profile.stop(): Stack is empty!");
    time=new Date().getTime();
    return returnTime;
   }
   return -1;
  }

  public static long timeSpentOn(String key) {
    return ((Long)modes.get(key)).longValue();
  }

  public static String report() {
   if (profilingOn) {
//    stop();
    time=new Date().getTime();

    String r="<table>\n";
    long total=0;
    Set ks=modes.keySet();
    Iterator i=ks.iterator();
    while (i.hasNext()) {
      String key=(String)i.next();
      long time=timeSpentOn(key);
      r+="<tr><td align=\"left\">"+key+"</td>\n    <td align=\"right\">"+time+" ms</td></tr>\n";
      total+=time;
    }
    r+="</table>\n";
    r+="Total of above = "+total+" ms with actual total = "+(time-cleartime)+" ms<br>\n";
    if (total!=time-cleartime)
      r+="  This means "+((time-cleartime)-total)+" ms spent in Profile.<br>\n";
    if (pastmodes.size()!=1)
      r+="ERROR: We have "+pastmodes+" left in stack.<br>\n";
    return r;
   } else
    return "Profiling.profilingOn==false!";
  }

}
