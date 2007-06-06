package edu.isi.gamebots.examples;

import java.lang.*;
import java.util.*;

import edu.isi.gamebots.examples.*;


/**
 *  An object representing an game class.
 */
class UTClass {
  // Public Constants
  ///////////////////////////////////////////////////////////////////////////
  public static final UTClass ACTOR  = new UTClass( "Actor", null );
  public static final UTClass PAWN   = new UTClass( "Pawn", ACTOR );
  public static final UTClass PICKUP = new UTClass( "Pickup", ACTOR );

/*
  public static final String IS_WEAPON = "isWeapon";
  public static final String IS_HEALTH = "isHealth";
  public static final String IS_AMMO   = "isAmmo";
  public static final String IS_ARMOR  = "isArmor";
*/

  //  Public Data
  ///////////////////////////////////////////////////////////////////////////
  public final String  name;
  public final UTClass parent;

  // Private Data
  ///////////////////////////////////////////////////////////////////////////
//  protected final Map     props;

  // Public Static Methods
  ///////////////////////////////////////////////////////////////////////////
  protected static String classNameFromId( String id ) {
    int start = id.indexOf( '.' )+1;
    int end = id.length()-1;
    while( Character.isDigit( id.charAt(end) ) &&
           end > start )
      --end;
    return id.substring( start, end+1 );
  }

  // Public Methods
  ///////////////////////////////////////////////////////////////////////////
  public UTClass( String name, UTClass parent ) {
    this.name   = name;
    this.parent = parent;
//    this.props  = new HashMap();
  }

/*
  public void setProperty( String key, Object value ) {
    props.put( key, value );
  }

  public Object getProperty( String key ) {
    Object value = props.get( key );
    UTClass utClass = this;
    while( value == null &&
           utClass.parent != null ) {
      utClass = utClass.parent;
      value = utClass.getProperty( key );
    }

    return value;
  }
*/

  public boolean equals( Object obj ) {
    if( obj == null )
      return false;
    if( !(obj instanceof UTClass) )
      return false;
    if( obj == this )
      return true;

    UTClass utClass = (UTClass) obj;
    return ( name.equalsIgnoreCase( utClass.name ) &&
             parent.equals( utClass.parent ) );
  }
}
