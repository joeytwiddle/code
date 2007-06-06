//  us.ca.la.anm.util.io.PrintWriterLog
//  Copyright 2000, Andrew n. marshall
//
//  Personal and Educational use is hereby granted.
//  Permission required for commercial use and redistribution.


package us.ca.la.anm.util.io;

import java.io.*;
import java.lang.*;

import us.ca.la.anm.util.io.*;


/**
 *  @author <a href="mailto:amarshal#PrintWriterLog@isi.edu">Andrew n marshall</a>
 */
public class PrintWriterLog extends DebugLog.Base {
  // Public Constants
  ///////////////////////////////////////////////////////////////////////////
  public static final PrintWriterLog DEFAULT = new PrintWriterLog( new PrintWriter( System.err, true ) );

  // Private Data
  ///////////////////////////////////////////////////////////////////////////
  protected PrintWriter _pw;

  protected Throwable _curError = null;

  // Constructors
  ///////////////////////////////////////////////////////////////////////////
  public PrintWriterLog( PrintStream ps ) {
    this( new PrintWriter( ps, true ) );
  }

  public PrintWriterLog( PrintWriter pw ) {
    _pw = pw;
  }

  // Public Methods
  ///////////////////////////////////////////////////////////////////////////
  public void logNote( Object message ) {
    _pw.println( "DEBUG: "+message );
  }

  public synchronized void logError( Object message, Throwable error ) {
    if( _curError != null &&
        _curError != error )
      _curError.printStackTrace( _pw );

    if( error == null ) {
      _pw.println( "ERROR: "+message );
    } else {
      _pw.println( "ERROR: "+message+": "+error );
    }

    _curError = error;
  }

  public void flush() {
    if( _curError != null ) {
      _curError.printStackTrace( _pw );
      _curError = null;
    }

    _pw.flush();
  }
}
