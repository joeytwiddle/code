import java.lang.String;
import java.io.File;
import java.util.Date;
import jlib.*;



interface DribbleMonitor {

  long getRecent();

}



public class LinuxPPP implements DribbleMonitor {

  File watching=new File(JLib.JPATH,"logs/packetdata.ppp");
  long lastsize=0;

  long getAbsolute() {
    return watching.length();
  }

  public long getRecent() {
    long newsize=getAbsolute();
    long diff=newsize-lastsize;
    lastsize=newsize;
    return diff;
  }



  public static void main(String[] args) {

    long timeframe=1000*2; // 2 seconds
    long max=0;
    long threshold=0;
    DribbleMonitor mon=new LinuxPPP();

    boolean firsttime=true;
    while (true) {

      long time=new Date().getTime();
      long recent=mon.getRecent();

      if (recent>max && !firsttime) {
        max=recent;
        threshold=(long)(0.8*max);
      }
      firsttime=false;

      System.err.print(recent+" / "+max+" ");
      if (recent<threshold) {
        System.err.print("Wahay, we're away!");
      } else {
        System.err.print("Whoa there!");
      }
      System.err.print("\r");

      JLib.stopUntil(time+timeframe);

    }

  }

}