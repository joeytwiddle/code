import java.io.*;
import java.lang.*;
import java.util.List;
import java.util.Vector;
import java.util.Date;
import java.net.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import javax.swing.text.html.parser.ParserDelegator;

import jlib.JThread;
import jlib.Files;
import jlib.Profile;
import jlib.JString;
import jlib.JMaths;
import jlib.JVector;
import jlib.JNet;
import nuju.*;

import Trawlet;

public class TrawletManager {

  // Start state and properties of search
  TrawlerStrategy strat;

  // State
  List hosts,urls; // All the hosts and URLs visited in this trawl
  List waiting; // Trawlets waiting to be started
  List open; // The open connections (Trawlets)
  List parsing; // Completed connections
  List dodgy; // Timed-out but haven't exited
  List parsed=new Vector(); // After being parsed
  List saved=new Vector(); // After being parsed
  List all=new Vector();
  Vector rejected=new Vector();

  TrawlerGUI gui=null;

  TrawletManager(TrawlerStrategy s, boolean dogui) {
    strat=s;
    hosts=new Vector();
    urls=new Vector();
    waiting=new Vector();
    open=new Vector();
    parsing=new Vector();
    dodgy=new Vector();

    strat.manager=this;

    if (dogui)
      gui=new TrawlerGUI(this);
  }

  void start() {

    setup();

    goget(strat.startURL,1);

    while ( waiting.size()>0 ||
            open.size()>0 ||
            parsing.size()>0 ) {
      checkfortimeout();
      trynew(); // Didn't used to need this =(
      if (gui!=null)
        gui.redraw();
      // JThread.yield();
      JThread.sleep(2000);
    }

    int therewere=dodgy.size();
    for (int i=0;i<dodgy.size();i++)
      if (((Trawlet)dodgy.get(i)).exited) {
        dodgy.remove(i);
        i--;
      }
    if (therewere>0) {
      System.out.println("Of "+therewere+" timed out trawlets, "+dodgy.size()+" of them failed to exit properly:");
      for (int i=0;i<dodgy.size();i++) {
        Trawlet t=(Trawlet)dodgy.get(i);
        System.out.println(t+" "+t.exited+" "+t.url);
      }
    }

    // Create summary file
    File sumfile=new File(strat.destdir,"index.html");
    StringBuffer page=new StringBuffer();
    page.append("<html><body>\n");
    page.append("<center><font size=\"+5\">Trawl results for </font>&quot;<a href=\""+strat.startURL+"\">"+strat.startURL+"</a>&quot;</center><br>\n");
    for (int i=0;i<all.size();i++) {
      Trawlet t=(Trawlet)all.get(i);
      page.append("<br>"+t.contentType+"&nbsp;&nbsp;&nbsp;<a href=\""+JNet.relative(JNet.url(sumfile),JNet.url(strat.fileName(t)))+"\">"+t.url+"</a>&nbsp;&nbsp;&nbsp;("+(t.parsing?"parsed":"")+(t.parsing&&t.saving?",":"")+(t.saving?"saved":"")+")<br>\n");
      page.append(t.finished+"&nbsp;"+t.stage+"&nbsp;"+t.cnt);
      if (t.attempt>1)
        page.append("&nbsp;ATTEMPT "+t.attempt);
      page.append("<br>\n");
      if (t.finished==-1)
        page.append("Failed: "+t.reasonForFailure+"<br>\n");
      if (t.finished==0)
        page.append("Reckons it's still going at &quot;"+t.stage+"&quot;!<br>\n");
    }
    for (int i=0;i<rejected.size();i++) {
      URL u=(URL)rejected.get(i);
      page.append("<br>URL rejected: "+u+"\n");
    }
    /* page.append("<br><br><h2>Dodgy trawlets:</h2>\n");
    for (int i=0;i<dodgy.size();i++) {
      Trawlet t=(Trawlet)dodgy.get(i);
      page.append("<br><a href=\""+t.url+"\">"+t.url+"</a>"+"&nbsp;&nbsp;&nbsp;"+t+"<br>\n");
    } */
    page.append("</body></html>\n");
    Files.writeStringtoFile(page.toString(),sumfile);

  }

  void setup() {
    if (strat.similar!=null && !strat.similar.equals(Trawler.notSimilar)) {
      System.out.println("Looking for similar urls...");
      // urls.add(strat.similar);
      Trawlet t=new Trawlet(strat.similar,this,100000);
      t.start();
      //while (t.finished==0)
      //  JThread.sleep(1000);
      JThread.join(t);
      // System.out.println(t.content);
      MyParserCallback myparser=new MyParserCallback(this,t,t.url,t.content,t.depth,false,false);
      Vector us=myparser.links;
      for (int i=0;i<us.size();i++) {
        URL u=(URL)us.get(i);
        urls.add(u);
        // System.out.println("Forbidding "+u);
      }
      System.out.println("Got "+urls.size()+" urls which will not be parsed.");
    }
  }

  void goget(URL u,int depth) {
      // System.out.println("Skipping previously visited "+u);
    if (strat.urlOk(u)) {
      urls.add(u);
      Trawlet t=new Trawlet(u,this,depth);
      all.add(t);
      int i=JMaths.rnd(0,waiting.size());
      waiting.add(i,t);
      trynew();
    } else {
      System.out.println("Reject by Strategy: "+u);
      rejected.add(u);
    }
  }

  void trynew() {
    // Check to see if any waiting Trawlets can get started
    // Don't bother if max connections open
    if (open.size()<strat.maxtotal) {
      for (int i=0;i<waiting.size();i++) { // For each waiting Trawlet
        Trawlet t=(Trawlet)waiting.get(i);
        if (okayToGo(t)) {
          if (!waiting.remove(t))
            System.out.println("TrawletManager.trynew(): "+waiting+" does not contain "+t);
          open.add(t);
          t.stage="started";
          t.start();
          i--;
        }
      }
    }
  }

  // Check if trawler t conflicts with any open trawlets
  boolean okayToGo(Trawlet t) {
    int conflicts=0;
    for (int i=0;i<open.size();i++) {
      Trawlet tt=(Trawlet)open.get(i);
      // System.out.println(""+tt);
      if (t.url.getHost().equals(tt.url.getHost())) {
        conflicts++;
        if (conflicts>=strat.maxhosts) {
          return false;
        }
      }
    }
    return true;
  }

  /** Called by the Trawlet when it has successfully retrieved the content of a URL.
  **/
  void trawletdone(Trawlet t) {
    // Move from open to parsing so that waiting Trawlets
    // start immediately
    parsing.add(t);
    if (!open.remove(t)) {
      System.out.println("TrawletManager.trawletdone(): Could not find "+t+" in open "+open);
      parsing.remove(t);
      dodgy.add(t);
      return;
    }
    // Now process the retrieved file
    // ie. extract new URLs from it
    // and/or save it
    strat.retrieved(t,this);
    // Remove from parsing so that manager knows we are done
    // (parsing means parsing!)
    if (!parsing.remove(t))
      System.out.println("TrawletManager.trawletdone(): Could not find "+t+" in parsing "+open);
    if (t.saving)
      saved.add(t);
    else
      if (t.parsing)
        parsed.add(t);
    trynew();
  }

  /** Trawlets which have had an open connection without any input for
   *  timeout seconds are thrown away.
  **/
  // I can't get the Trawlets to stop when they are stuck requesting
  // a connection ( urlcon.getContent() )
  // So instead, I am exiting the whole program if _all_ open Trawlets
  // are
  // That sentence wasn't finished - I imagine it's been fixed!
  void checkfortimeout() {
    for (int i=0;i<open.size();i++) {
      Trawlet t=(Trawlet)open.get(i);
      if (!t.dontTimeMeOut && t.lastContact.getTime()<new Date().getTime()-strat.timeout*1000) {
        System.out.print("Manager:  ");
        timeout(t);
        t.reasonForFailure="timeout - took too long";
        i--;
      }
    }
  }

  void timeout(Trawlet t) {
        System.out.println("Timing out "+t+" at stage "+t.stage+" of URL "+t.url);
        t.externaltimeout=true;
        dodgy.add(t);
        abortTrawlet(t);
        if (t.attempt<strat.maxattempts) {
          t.attempt++;
          waiting.add(t);
          System.out.println("~~~~~~~~~~~ Will try a "+t.attempt+"th attempt on "+t.url);
        }
//        t.interrupt();
        /* while (!t.exited) {
          t.interrupt();
          try { Thread.sleep(1000); } catch (Exception e) { }
        } */
        // t.stop();
  }

  /** Trawlet is assumed to be closed correctly elsewhere.
   *  This just removes Trawlet to allow Manager to continue with waiting Trawlets.
  **/
  void abortTrawlet(Trawlet t) {
    if (!open.remove(t))
      System.out.println("TrawletManager.abortTrawlet(): Could not find "+t+" in open "+open);
    trynew();
  }

}
