package edu.isi.gamebots.examples;

import java.lang.*;
import java.util.Map;

import us.ca.la.anm.util.io.DebugLog;

import edu.isi.gamebots.client.Bot;
import edu.isi.gamebots.client.GamebotsConstants;
import edu.isi.gamebots.client.Message;
import edu.isi.gamebots.examples.*;


/**
 *  An object representing an game actor.
 */
class UTActor implements GamebotsConstants {
  //  Public Data
  public final String   id;
  public final UTClass  utClass;
  public Vector3D loc;
  public Rotation3D rot;
  public Vector3D vel;

  //  Private Data
  protected final DebugLog log;


  //  Public Methods
  public UTActor( String id, UTClass utClass, DebugLog log ) {
    this.id      = id;
    this.utClass = utClass;
    this.log     = log;
  }

  public void update( Message message ) {
    double[] vector;

//    synchronized( this ) {
      String value = message.getProperty( LOCATION );
      if( value != null ) {
        if( loc == null )
          loc = new Vector3D();
        try {
//          synchronized( loc ) {
            vector = Bot.parseVector( value );

            loc.x = vector[0];
            loc.y = vector[1];
            loc.z = vector[2];
//          }
        } catch( RuntimeException error ) {
          // ignore???
        }
      }

      value = message.getProperty( ROTATION );
      if( value != null ) {
        if( rot == null )
          rot = new Rotation3D();
        try {
          synchronized( rot ) {
            vector = Bot.parseVector( value );

            rot.pitch = vector[0] * 2 * Math.PI / 65535;
            rot.yaw   = vector[1] * 2 * Math.PI / 65535;
            rot.roll  = vector[2] * 2 * Math.PI / 65535;
          }
        } catch( RuntimeException error ) {
          // ignore???
        }
      }

      value = message.getProperty( VELOCITY );
      if( value != null ) {
        if( vel == null )
          vel = new Vector3D();
        try {
          synchronized( vel ) {
            vector = Bot.parseVector( value );

            vel.x = vector[0];
            vel.y = vector[1];
            vel.z = vector[2];
          }
        } catch( RuntimeException error ) {
          // ignore???
        }
      }
//    } // end sync
  }

  public boolean instanceOf( UTClass utClass ) {
    UTClass ancestor = this.utClass;
    boolean result;
    while( !(result = ancestor.equals( utClass )) &&
           ancestor.parent != null )
      ancestor = ancestor.parent;

    return result;
  }

  public boolean equals( Object obj ) {
    if( obj == null )
      return false;
    if( !(obj instanceof UTActor) )
      return false;
    if( obj == this )
      return true;

    UTActor actor = (UTActor) obj;
    return ( id.equalsIgnoreCase( actor.id ) &&
             utClass.equals( actor.utClass ) );
  }

  public String toString() {
    return "<"+utClass.parent.name+"/"+utClass.name+"> "+id;
  }

  public String propertiesToString() {
    StringBuffer sb = new StringBuffer();
    boolean listStarted = false;
    if( loc != null ) {
      sb.append( "Loc=" );
      sb.append( loc );
      listStarted = true;
    }
    if( rot != null ) {
      if( listStarted )
        sb.append( ", " );
      sb.append( "Rot=" );
      sb.append( rot );
      listStarted = true;
    }
    if( vel != null ) {
      if( listStarted )
        sb.append( ", " );
      sb.append( "Vel=" );
      sb.append( vel );
      listStarted = true;
    }
    return sb.toString();
  }
}
