package jlib;

import java.lang.String;
import java.io.*;
import java.util.*;
import jlib.*;
import jlib.strings.*;

// import changetheworld.CTW;

/** Keeps a static log (presumably within the runtime environment) of all reports and errors.
   Stored in plain-text and HTML format.  Intended for debugging and access logging.
   If a new error has occurred, logiferror() will get the log and reset the error flag.
   For security reasons, confidential data should not be written to the log, since it is currently reported to the user if an error occurs
   Therefore, be careful logging data in an error report - it might be someones password!

 *  The Log contains a report of all Exception which were declared with JLib.report or JLib.error,
 *  and and custom reports with Log.report and Log.error.
 *
 *  Logs may be retrieved using ...
 *
 *  It would be nice to be able to have a critical Log
 *  which would continually send information to a safe place
 *  in case the runtime exits or something crashes.
**/

class LogEntry {
	boolean err;
	String txt;
	Object obj;
	Exception e;
	public LogEntry(boolean error,String text,Object src,Exception ex,String html) {
		err=error;
		txt=text;
		obj=src;
		e=ex;
	}
}

public class Log {

	// public static final File logFile=new File(JLib.JPATH,"/logs/java.txt");
	// public static final String logFile="/usr/share/java/servlets/java.txt";
	public static final String logFile=JLib.JPATH+"/code/java/servlets/java.txt";
  public static boolean loggingOn = true;
  public static boolean toStdOut  = true;
	public static boolean toFile    = true;
	public static boolean toString  = false;
	public static boolean toHtml    = false;
	public static boolean toEntries = false;
  public static Vector watchers=new Vector();

  public static boolean error=false;
  public static int ind=0;
  public static String log="";
  public static String logHtml="";
	public static final PrintWriter out=getFileWriter();
	public static List entries=new Vector();

	private static PrintWriter getFileWriter() {
		PrintWriter pw=null;
		// Exception debug=null;
		try {
			if (loggingOn && toFile) {
				pw=new PrintWriter(new FileOutputStream(logFile,true));
			}
		} catch (Exception e) {
			// debug=e;
			System.out.println("Could not log to file "+logFile+" "+e);
		}
		if (pw==null) {
			toFile=false;
			toStdOut=toString=toHtml=toEntries=true;
			// Can't really go doing this, because this method is called during class instantiation
			// error("Could not log to file "+logFile,debug);
		}
		return pw;
	}

  public static void clear() {
		report(JLib.niceDate()+" ----------- CLEAR -----------");
    log=logHtml="";
		error=false;
  }

	public static void report(boolean err,String txt,Object src,Exception e,String html) {
		if (loggingOn) {
			String whole= 
					( err ? "Error! " : "" )
				+ ( src==null ? "" : "in "+src.getClass().getName()+" : " )
				+ ( txt==null ? "" : txt );
			if (e!=null) {
				FakeWriter f=new FakeWriter();
				e.printStackTrace(new PrintWriter(f));
				whole += e+"\n"+f.store;
			}
			if (toStdOut)
				System.out.println(whole);
			if (toString)
				log+=whole+"\n";
			if (toHtml)
				logHtml+=html; // JString.toHtml(whole+"\n"); // also see JString for tag removal
			if (toFile) {
				out.println(whole);
				out.flush(); // Vital
			}
			if (toEntries)
				entries.add(new LogEntry(error,txt,src,e,html));
		}
		if (err)
			error=true;
	}

  public static void warn(String s) {
		report(s); }

  public static void report(String s) {
		report(false,s,null,null,null); }
  public static void report(Exception e) {
		report(false,null,null,e,null); }
  public static void report(Object o,String s) {
		report(false,s,o,null,null); }
  public static void report(Object o,Exception e) {
		report(false,null,o,e,null); }
  public static void report(Object o,String method,Exception e) {
		report(false,method,o,e,null); }

  public static void error(String s) {
		report(true,s,null,null,null); }
  public static void error(Exception e) {
		report(true,null,null,e,null); }
  public static void error(Object o,String s) {
		report(true,s,o,null,null); }
  public static void error(Object o,Exception e) {
		report(true,null,o,e,null); }
  public static void error(Object o,String method,Exception e) {
		report(true,method,o,e,null); }

  public static void indent(int i) { ind+=i; }
  public static void undent(int i) { ind-=i; }

  public static String logiferror() {
    if (error) {
			String ans=log;
			clear();
      return ans;
    }
    return "";
  }

  public static String HTMLlogiferror() {
    if (error) {
			String ans=logHtml;
      clear();
      return ans;
    }
    return "";
  }

}
