// Try using a URLConnection!  It will tell you the length
// and give you a String nice and easily!

import java.io.*;
import java.lang.*;
import java.util.*;
import java.net.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import javax.swing.text.html.parser.ParserDelegator;

import jlib.Files;
import jlib.Profile;
import jlib.JString;
import jlib.JFtp;
import jlib.JLib;
import nuju.*;

/** Collects the contents of a URL and reports back to the TrawletManager.
**/

public class Trawlet extends Thread {

  // Initial information
  TrawletManager manager; // Who to report to when finished or deciding to abort
  URL url; // URL to be retrieved
  int depth; // depth of Trawlet in search
  int attempt=1;

  Object conobj=null;

  // Inferred file data, and whether the Strategy wants to keep/save the file.
  URLConnection urlcon=null;
  int contentLength=-1;
  String contentType="nun"; // To resolve filename before and after opening connection
  File destFile;
  String extension;
  boolean saving,parsing;
  boolean parsingUnsure=false; // Keep provided stays below size
  String content; // Only if parsing
  int cnt; // How much read (whether parsing or not)

  // Set by TrawlerStrategy
  File fileName=null;

  public int finished; // 0 means not finished, 1 means successful, -1 means failure
  public String reasonForFailure="";

  // Timeout handling
  boolean dontTimeMeOut=true;
  Date lastContact; // Time of last transmission from the Input stream
  volatile boolean externaltimeout=false; // Set to true when someone decides a timeout is needed
  String stage="none"; // Timeout debugging
  boolean exited=false;

  public String toString() {
    StringBuffer s=new StringBuffer();
    s.append(finished+" "+contentType+" ");
    if (finished==-1)
      s.append("'"+reasonForFailure+"' ");
    s.append(url.getHost()+":/"+new File(url.getFile()).getName()+" "+stage+" ("+depth+") "
           +( content==null ? "..." : ""+cnt ) );
    return s.toString();
  }

  Trawlet(URL u,TrawletManager m,int d) {
    url=u;
    manager=m;
    depth=d;
    lastContact=new Date();
    finished=0;
  }

  public void run() {

    finished=0;

    content=null;
    try {

      stage="1";
      destFile=manager.strat.fileName(this);
      stage="2";
      extension=Files.split(destFile.getName())[1].toLowerCase();
      // extension=JString.afterlast(destFile.getName(),".").toLowerCase();
      stage="3";
      boolean worthopening=manager.strat.open(this);
      if (!worthopening) {
        stage="not worth it";
        echo("Skipping - not worth opening: "+url);
        manager.abortTrawlet(this);
        return;
      }

      String prot=url.getProtocol();

      dontTimeMeOut=false;
      lastContact=new Date();

      // if (!prot.equals("ftp")) {

        // Get connection to URL
        stage="requesting url connection";
        echo("Opening \""+url+"\"...");
        urlcon=url.openConnection();

        lastContact=new Date();
        echo("Getting information and setting up file");
        stage="getting information and setting up file";
        // urlcon.connect(); // no need in spec or in practice
        contentType=urlcon.getContentType();
        contentLength=urlcon.getContentLength(); // will be -1 if unknown

      // }

      destFile=manager.strat.fileName(this); // re-read filename now that we have type
      extension=Files.split(destFile.getName())[1].toLowerCase();

      saving=manager.strat.write(this);
      parsing=manager.strat.keep(this);
      if (!saving && !parsing) {
        echo("Skipping - no save or parse desired: "+url);
        manager.abortTrawlet(this);
        return;
      }
      echo( "Intending to " + ( saving ? "save" : "" )
                            + ( saving && parsing ? " and " : "" )
                            + ( parsing ? "parse" : "" ) + "." );

      stage="starting download";

      // System.out.println("Protocol: "+prot);

        // Normal URL (http or file)

        BufferedOutputStream out=null;
        if (saving) {
          // Ensure directory exists ready to write
          destFile.getParentFile().mkdirs();
          out=new BufferedOutputStream(new FileOutputStream(destFile));
        }

        InputStream in=null;

      lastContact=new Date();

      // if (prot.equals("ftp")) {

        // Do ftp connection

        try {
          stage="trying lynx";
          File f=new File(url.getFile());
          String parent=f.getParent();
          if (parent==null)
            parent="/";
          String com="lynx -dump "+url;
          in=JLib.exec_stdout(com);
          stage="got lynx stream";
          /*String data=JFtp.getLynxDump(url.getHost(),parent,f.getName());
          if (keeping)
            content=data;
          stage="lynx over";
          if (saving) {
            destFile.getParentFile().mkdirs();
            Files.writeStringtoFile(content,destFile);
          }
          stage="lynx succeeded";*/
        } catch (Exception e) {
          echo("Trawlet: "+e);
          manager.timeout(this);
          reasonForFailure="Trying FTP: "+e;
          finished=-1;
          return;
        }

      // } else {

        // Object o=url.getContent();
        stage="requesting stream";
        echo("Requesting connection...");
        // Object o=urlcon.getContent();
        Object o=urlcon.getInputStream();
        conobj=o;
        echo("Connection is a "+o);
        try {
          in=(InputStream)o;
        } catch (Exception e) {
          System.out.println("Could not cast "+o+" to InputStream!");
          reasonForFailure=o+" is not InputStream!";
          manager.timeout(this);
          finished=-1;
          return;
        }

      // }

        stage="reading data";

        // Read stream
        echo("Expecting "+contentLength);
        cnt=0;
        content="";
        boolean done=false;
        Date startTime=new Date();
        while (!done) {

          if (externaltimeout) {
            echo("Trawlet recognising timeout on "+url);
            in.close();
            if (saving)
              out.close();
            exited=true;
            finished=-1;
            return;
          }

          // If Stream has nothing to give yet, wait 0.5 seconds
          if (in.available()<=0) {
            if (manager.strat.showStream)
              System.out.print(".");
              //  yield();
            sleep(500);
            if (manager.strat.showStream)
              System.out.print("?");
          }

          // Check if we are at the end of the data
          int r=in.read();
          if (manager.strat.showStream)
            System.out.print(">");
          lastContact=new Date();
          if (r<0) {
            done=true;
            if (manager.strat.showStream)
              System.out.print("#EOF#");
          } else {
            if (parsing)
              content+=(char)r;
            if (saving)
              out.write((char)r);
            cnt++;
          }

          if (contentLength>0 && manager.strat.showProgress) {
            long timesofar=lastContact.getTime()-startTime.getTime();
            float bpms=(float)cnt/(float)timesofar;
            long bytesleft=contentLength-cnt;
            long timeleft=(long)(((float)bytesleft)/bpms);
            int percent=100*cnt/contentLength;
            String txt=percent+"% of "+destFile.getName()+" in "+JString.time(timesofar)+"\n";
            txt+="  leaving "+JString.time(timeleft)+" for "+(float)bytesleft/1024.0f/1024.0f+"mb";
            txt+=" at "+bpms*1000/1024+"k/s";
            // "of "+contentType;
            // String txt2=JString.time(timesofar)+" to do "+cnt+"/"+contentLength+" so ("+timeleft/1000+") "+txt+" left.";
            System.out.println(txt);
            }
          if (contentLength>0 && (""+cnt).charAt((""+cnt).length()-1)=='0')
            System.out.println(cnt+"/"+contentLength);

          // Read a block of data
          if (!done) {
            int a=in.available();
            //System.out.print("("+a+"=");
            byte[] bs=new byte[a];
            int numread=in.read(bs);
            // if (numread!=a)
              // JLib.error("Trawlet sending: numread "+numread+" != a "+a);
            //System.out.print(numread+")");
            if (parsing)
              content+=new String(bs).substring(0,numread);
            if (saving)
              out.write(bs,0,numread);
            cnt+=a;
            // cnt+=numread;
            if (manager.strat.showStream)
              System.out.print(""+numread);
          }

          // If the size was originally unknown, but has now grown larger than
          // the parsed size, give up parsing it!
          if (parsingUnsure && content.length()>manager.strat.largestparsed) {
            echo("Aborted parsing because size "+content.length());
            content="";
            parsing=false;
            // If not saving either, stop!
            if (!saving) {
              externaltimeout=true;
              reasonForFailure="not parsing too large no HTML evident, and not saving";
            }
            // NOTE: Manager is not refreshed to trynew().
            // At the moment this is catered for by regular trynew() call thanks to checktimeout()
          }

        }
        in.close();
        if (saving)
          out.close();
        if (manager.strat.showStream)
          System.out.print("\n");

        // while (in.available()>0)
        //  content+=(char)in.read();
        stage="Retrieved";
        echo("Retrieved \""+url+"\" ("+cnt+")");

      finished=1;
      manager.trawletdone(this);

    } catch (Exception e) {
      echo("Trawlet: "+e);
      manager.timeout(this);
      reasonForFailure=""+e;
      finished=-1;
    }

    if (externaltimeout) {
      echo("Trawlet recognising timeout (naturally or through exception) on "+url);
      exited=true;
      finished=-1;
    }

    // stop();

  }

  public static void echo(String s) {
    System.out.println(s);
  }

}
