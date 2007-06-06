//  edu.isi.gamebots.clients.Message
//  Copyright 2000, University of Southern California,
//                  Information Science Institute
//
//  Personal and Educational use is hereby granted.
//  Permission required for commercial use and redistribution.


package edu.isi.gamebots.client;

import java.lang.*;
import java.util.*;

import edu.isi.gamebots.client.*;


/**
 *  @author <a href="mailto:amarshal#gamebots@isi.edu">Andrew n marshall</a>
 */
public class Message extends EventObject {
  protected final String type;
  protected final Properties props;
  protected final boolean complete;

  Message( GamebotsClient client,
           String type,
           Properties props,
           boolean complete ) {
    super( client );

    this.type = type;
    this.props = props;
    this.complete = complete;
  }

  public String getType() {
    return type;
  }

  public Set getKeySet() {
    return props.keySet();
  }

  public String getProperty( String key ) {
    return props.getProperty( key );
  }

  public Set getPropertySet() {
    return props.entrySet();
  }

  public boolean isComplete() {
    return complete;
  }

  public String toString() {
    return toString( new StringBuffer(), 0 ).toString();
  }

  public StringBuffer toString( StringBuffer sb, int indent ) {
    for( int i=0; i<indent; i++ )
      sb.append( ' ' );
    sb.append( type+": "+props );
    return sb;
  }
}
