package jlib;

import java.lang.*;
import java.io.*;

public class SimpleUserInt {

  public static void main(String[] args) {
    SimpleUserInt.getuserinput();
  }

  public static String getuserinput() {

    InputStreamReader isr = new InputStreamReader ( System.in );
    StreamTokenizer st = new StreamTokenizer ( isr );
    /* Once you have a StreamTokenizer, you can customize it's behavior.
       For this example we just show basic usage.
    */
    int c;
    try {
      while ( true ) {
        switch ( c = st.nextToken () ) {
         case StreamTokenizer.TT_NUMBER:
            System.out.println ( "Read a number: " + st.nval );
            break;
         case StreamTokenizer.TT_WORD:
            System.out.println ( "Read a word: " + st.sval );
            break;
         default:
            // do something in other cases (EOL, EOF,whitespace, quotes...)
            break;
        }
      }
    } catch ( IOException ioe ) {
     // Won't happen very often from the keyboard
    }

    return "";

  }

}