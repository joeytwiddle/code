//  us.ca.la.anm.util.io.JTextAreaLog
//  Copyright 2000, Andrew n. marshall
//
//  Personal and Educational use is hereby granted.
//  Permission required for commercial use and redistribution.


package us.ca.la.anm.util.io;

import java.lang.*;
import java.io.PrintWriter;
import java.io.StringWriter;

import javax.swing.JTextArea;
import javax.swing.SwingUtilities;

import us.ca.la.anm.util.io.*;


/**
 *  @author <a href="mailto:amarshal#JTextAreaLog@isi.edu">Andrew n marshall</a>
 */
public class JTextAreaLog extends DebugLog.Base {
  //  Private Data
  ///////////////////////////////////////////////////////////////////////////
  protected StringBuffer buffer = new StringBuffer();
  protected JTextArea    text   = null;

  //  Public Methods
  ///////////////////////////////////////////////////////////////////////////
  public void setJTextArea( JTextArea text ) {
    synchronized( buffer ) {
      this.text = text;
      if( text != null )
        text.append( buffer.toString() );
    }
  }

  /**
   *  Appends the message object to the log.
   */
  public void logNote( Object message ) {
    final String line = message+"\n";
    synchronized( buffer ) {
      buffer.append( line );
      if( text != null ) {
        // Swing isn't thread safe...
        SwingUtilities.invokeLater( new Runnable() {
            public void run() {
              text.append( line );
            }
          } );
      }
    }
  }

  /**
   *  Appends the error message object to the log.
   */
  public void logError( Object message, Throwable error ) {
    StringBuffer sb = new StringBuffer();
    sb.append( "ERROR: " );
    sb.append( message.toString() );
    if( error == null ) {
      sb.append('\n');
    } else {
      sb.append( ": " );
      sb.append( error.getMessage() );
      sb.append( '\n' );

      StringWriter sw = new StringWriter();
      PrintWriter pw = new PrintWriter( sw );
      error.printStackTrace( pw );
      pw.flush();
      buffer.append( sw.getBuffer().toString() );
    }
    String line = sb.toString();
    synchronized( buffer ) {
      buffer.append( line );
      if( text != null )
        text.append( line );
    }
  }

  /**
   *  Flushses all output.  It is a good thing to call between errors.
   */
  public void flush() {
  }
}
