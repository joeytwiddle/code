package jlib;

import java.lang.*;
import java.io.*;

import jlib.Log;

public class CommandLine extends Echo {

  public final static void main(String[] args) {
    char c=getUserChar();
    System.out.println(c+" '"+c+"'");
  }

  /** Declare an error to Sys.out, log it, and exit -1. **/
  public static void error(String e) {
    System.out.println(e);
    Log.error(e);
    System.exit(1);
  }

  public static String getSecretString(String request) {
    return getUserInput(request);
  }

  public static File getFile(String name) {
    return new File(getUserInput("Please enter filename:"));
  }

  public static String getOption(String[] options) {
    while (true) {
      System.out.print("Choose one of:");
      for (int i=0;i<options.length;i++)
        System.out.print(" "+options[i]);
      System.out.println("");
      String s=readLine();
      if (JVector.contains(options,s))
        return s;
    }
  }

  /** Doesn't work how I'd like: you have to press &lt;return&gt; before the first char is processed.
   *  I guess System.in must be buffered.
  **/
  public static char getUserChar() {
    try {
      Reader br =new InputStreamReader(System.in);
      return (char)br.read();
      // return (char)System.in.read();
    } catch (Exception e) {
      return (char)-1;
    }
  }

  /** Read a line of text from the user. **/
  public static String getUserInput(String request) {
    System.out.println(request);
    return getUserInput();
  }
  public static String getUserInput() {

    BufferedReader r=new BufferedReader(new InputStreamReader(System.in));
    try {
      return r.readLine();
    } catch (Exception e) {
      JLib.report(e);
      return "";
    }

    /* String answer="";
    InputStreamReader isr=new InputStreamReader(System.in);
    StreamTokenizer st=new StreamTokenizer(isr);
    int c;
    try {
      while ( true ) {
        st.nextToken();
        String s=st.sval; // toString();
        System.out.println("Got token "+st.ttype+": "+s);
        answer+=s;
        if (st.lineno()>0) {
          System.out.println(">"+answer+"<");
          return answer;
        } */
/*        switch ( c = st.nextToken () ) {
          case StreamTokenizer.TT_EOL:
            return answer;
          default:
            answer+=st.toString();
            break;
        } */
/*         case StreamTokenizer.TT_NUMBER:
            System.out.println ( "Read a number: " + st.nval );
            break;
         case StreamTokenizer.TT_WORD:
            System.out.println ( "Read a word: " + st.sval );
            break; */
    /*  }
    } catch ( IOException ioe ) {
      System.out.println("IOException: "+ioe);
    }

    return answer; */

  }
  public static String getString(String request) {
    return getUserInput(request);
  }
  public static String readLine() {
    return getUserInput();
  }

}