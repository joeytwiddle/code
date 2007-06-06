//  us.ca.la.anm.util.io.DebugLog
//  Copyright 2000, Andrew n. marshall
//
//  Personal and Educational use is hereby granted.
//  Permission required for commercial use and redistribution.


package us.ca.la.anm.util.io;

import java.lang.*;


/**
 *  A simple interface for logging discrete message objects of arbitrary type.
 *  Messages are logged as either debug notes or errors, where errors can also
 *  include an additional Throwable object.
 *
 *  Most implementation utilize the {@link Object#toString()} method to record
 *  messages.
 *
 *  @author <a href="mailto:amarshal#DebugLog@isi.edu">Andrew n marshall</a>
 */
public interface DebugLog {
  //  Public Methods
  ///////////////////////////////////////////////////////////////////////////

  /**
   *  Appends the message object to the log.
   */
  public void logNote( Object message );

  /**
   *  Appends the error message object to the log.
   */
  public void logError( Object message );

  /**
   *  Appends the error message object and error to the log.  Implementations
   *  may withhold autoflushing on this method in order to group messages of the
   *  same error.  Therefore, developers are encouraged to call {@link #flush()}
   *  if the Throwable is not going to be thrown again after this method, it is
   *  not going to be thrown again.
   */
  public void logError( Object message, Throwable error );

  /**
   *  Flushses all output.  It is a good thing to call between errors.
   */
  public void flush();


  public static abstract class Base implements DebugLog {
    //  Public Methods
    ///////////////////////////////////////////////////////////////////////
    public void logError( Object message ) {
      logError( message, null );
    }
  };
}
