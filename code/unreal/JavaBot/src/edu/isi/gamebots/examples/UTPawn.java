package edu.isi.gamebots.examples;

import java.lang.*;
import java.util.Map;

import us.ca.la.anm.util.io.DebugLog;

import edu.isi.gamebots.client.Message;
import edu.isi.gamebots.examples.*;


/**
 *  An object representing a player.
 */
class UTPawn extends UTActor {
  //  Public Data
  public String   name;
  public int      team = 255;


  //  Public Methods
  public UTPawn( String id, UTClass utClass, DebugLog log ) {
    super( id, utClass, log );
  }

  public void update( Message message ) {
    super.update( message );

    String value = message.getProperty( PLAYER_NAME );
    if( value != null )
      name = value;

    value = message.getProperty( PLAYER_TEAM );
    if( value != null ) {
      try {
        team = Integer.parseInt( value );
      } catch( NumberFormatException error ) {
        log.logError( "Invalid team number", error );
      }
    }
  }

  public String toString() {
    return super.toString()+" \""+name+"\"";
  }

  public String propertiesToString() {
    StringBuffer sb = new StringBuffer();

    sb.append( super.propertiesToString() );
    boolean listStarted = !sb.toString().equals("");
    if( rot != null ) {
      if( listStarted )
        sb.append( ", " );
      sb.append( "Team=" );
      sb.append( team );
      listStarted = true;
    }

    return sb.toString();
  }
}

