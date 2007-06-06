package org.neuralyte.binarydiff;
import java.util.Date;

/*
 * Created on Oct 31, 2006
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */

/**
 * @author joey
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class ETA {

    public static java.util.Date startTime = null;
    
     public static long total = 0;
  
     public static void addTodo(long add) {
         total += add;
     }
     
    public static String getETA(double proportionNow) {
        Date now = new java.util.Date();
        if (startTime == null) {
            startTime = now;
            return "";
        } else {
            long passedTime = now.getTime() - startTime.getTime();
            long totalTime = (long)((double)passedTime * 1.0 / proportionNow);
            long remainingTime = totalTime - passedTime;
            // System.err.println("now="+now.getTime()+" startTime="+startTime.getTime()+" passed="+passed+" remaining="+remaining+" total="+total);
            // return ""+englishTimeDifference(remainingTime)+" remaining, out of "+englishTimeDifference(totalTime);
            return "(proportion="+proportionNow+",passed="+passedTime+",total="+totalTime+",remaining="+remainingTime+") "+englishTimeDifference(remainingTime)+" remaining, out of "+englishTimeDifference(totalTime);
        }
    }
    
    public static void printETA(double proportionNow) {
        System.err.println(getETA(proportionNow));
    }
    
    public static String englishTimeDifference(long millis) {
        String s = "";
        if (millis<0) {
            millis = - millis;
        }
        long minutes = -1;
        minutes = (long)(millis / 1000 / 60 + 0.5);
        s += (minutes) + " minutes";
        return s;
    }
    
}
