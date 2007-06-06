//  edu.isi.gamebots.examples.ExampleBot
//  Copyright 2000, University of Southern California,
//                  Information Science Institute
//
//  Personal and Educational use is hereby granted.
//  Permission required for commercial use and redistribution.


package edu.isi.gamebots.examples;

import java.awt.*;
import java.awt.geom.*;
import java.lang.*;
import java.util.*;

import javax.swing.*;

import us.ca.la.anm.util.io.*;

import edu.isi.gamebots.client.*;
import edu.isi.gamebots.examples.*;


/**
 *  This example implementation the VizTool class which shows basic message
 *  handling and uses {@link UTMapUI} for a map in its user interface.
 *
 *  @author <a href="mailto:amarshal#gamebots@isi.edu">Andrew n marshall</a>
 */
public class ExampleVizTool implements VizTool {
  //  Private Constants
  ////////////////////////////////////////////////////////////////////////////
  public final static double VISION_DIST   = 400.0d;

  //  Private Data
  ////////////////////////////////////////////////////////////////////////////
  protected DebugLog log;

  protected boolean isConnected = false;

  protected final Object classLock = new Object();
  protected java.util.Map knownClasses = new Hashtable();  // String class name to UTClass

  protected final Object actorLock = new Object();
  protected java.util.Map knownObjs = new Hashtable(); // String id to UTActor
  protected Set navPoints = new HashSet();
  protected Set players = new HashSet();
  protected Set pickups = new HashSet();

  protected VizMap vizMap = new VizMap();
  protected static final Shape visionArc = new Arc2D.Double( -VISION_DIST, -VISION_DIST, 2*VISION_DIST, 2*VISION_DIST, -45, 90, Arc2D.PIE );

  private Set nextNavPoints = new HashSet();
  private Set nextPlayers = new HashSet();
  private Set nextPickups = new HashSet();



  //  Public Methods
  ////////////////////////////////////////////////////////////////////////////
  public ExampleVizTool() {
    vizMap.addActorSet( navPoints );
    vizMap.addActorSet( players );
    vizMap.addActorSet( pickups );
  }

  public void init( DebugLog log ) {
    this.log = log;
  }

  public JComponent getView() {
    return vizMap;
//return new JLabel( getClass().getName() );
  }

  public GamebotsClient.Listener getListener() {
    return listener;
  }

  public void destroy() {
    // do nothing
  }

  //  Private Methods
  ////////////////////////////////////////////////////////////////////////////
  protected UTActor getActor( Message message ) {
    String type = message.getType();
    if( type.equals( END ) )
      return null;
    String id  = message.getProperty( ACTOR_ID );
    UTActor actor;
    synchronized( actorLock ) {
      actor = (UTActor) knownObjs.get( id );
      if( actor != null )
        return actor;

      String classname = message.getProperty( ACTOR_CLASS );
      if( classname == null )
        classname = UTClass.classNameFromId( id );

      UTClass utClass = (UTClass) knownClasses.get( classname );
      if( type.equals( INV ) ) {
        if( utClass == null )
          utClass = new UTClass( classname, UTClass.PICKUP );
        actor = new UTActor( id, utClass, log );
      } else if( type.equals( PLAYER ) ) {
log.logNote( "Player message: "+message );
        if( utClass == null )
          utClass = new UTClass( classname, UTClass.PAWN );
        actor = new UTPawn( id, utClass, log );
      } else {
        if( utClass == null ) {
          utClass = new UTClass( classname, UTClass.ACTOR );
          knownClasses.put( classname, utClass );
        }
        actor = new UTActor( id, utClass, log );
      }
      knownObjs.put( id, actor );
    } // end sync

    return actor;
  }

  // Event Handlers
  protected GamebotsClient.Listener listener = new GamebotsClient.Listener() {
    public void connected() {
      log.logNote( "Connected: "+new Date() );
      isConnected = true;
      vizMap.repaint();
    }

    public void receivedAsyncMessage( Message message ) {
      log.logNote( message );
    }

    public void receivedSyncMessage( MessageBlock block ) {
      nextNavPoints.clear();
      nextPlayers.clear();
      nextPickups.clear();

      Iterator i = block.getMessages();
      Message message;
      String id;
      UTActor actor;
      while( i.hasNext() ) {
        message = (Message) i.next();
        actor = getActor( message );
        if( actor != null ) {
          actor.update( message );
          if( actor.utClass.parent == UTClass.PAWN ) {
            nextPlayers.add( actor );
          } else if( actor.utClass.parent == UTClass.PICKUP ) {
            nextPickups.add( actor );
          } else { // Normal Actor/Nav-Point
            nextNavPoints.add( actor );
          }
        }
      }

      Set newNavPoints;
      Set newPlayers, missingPlayers;
      Set newPickups, missingPickups;
      synchronized( actorLock ) {
        // Diff NavPoints
        newNavPoints = new HashSet( nextNavPoints );
        newNavPoints.removeAll( navPoints );
        navPoints.addAll( newNavPoints );

        // Diff Players
        newPlayers = new HashSet( nextPlayers );
        newPlayers.removeAll( players );
        missingPlayers = new HashSet( players );
        missingPlayers.removeAll( nextPlayers );
        players.removeAll( missingPlayers );
        players.addAll( newPlayers );

        // Diff Pickups
        newPickups = new HashSet( nextPickups );
        newPickups.removeAll( pickups );
        missingPickups = new HashSet( pickups );
        missingPickups.removeAll( nextPickups );
        pickups.removeAll( missingPickups );
        pickups.addAll( newPickups );
      }

      if( !newNavPoints.isEmpty() ||
          !missingPlayers.isEmpty() ||
          !missingPickups.isEmpty() ||
          !newPlayers.isEmpty() ||
          !newPickups.isEmpty() ) {
        StringBuffer sb = new StringBuffer();
        block.getMessageString( sb, 0 );

        i = newNavPoints.iterator();
        while( i.hasNext() ) {
          actor = (UTActor) i.next();
          sb.append( "\n  + " );
          sb.append( actor );
          sb.append( "\n      " );
          sb.append( actor.propertiesToString() );
        }

        i = missingPlayers.iterator();
        while( i.hasNext() ) {
          actor = (UTActor) i.next();
          sb.append( "\n  - " );
          sb.append( actor );
          sb.append( "\n      " );
          sb.append( actor.propertiesToString() );
        }

        i = missingPickups.iterator();
        while( i.hasNext() ) {
          actor = (UTActor) i.next();
          sb.append( "\n  - " );
          sb.append( actor );
          sb.append( "\n      " );
          sb.append( actor.propertiesToString() );
        }

        i = newPickups.iterator();
        while( i.hasNext() ) {
          actor = (UTActor) i.next();
          sb.append( "\n  + " );
          sb.append( actor );
          sb.append( "\n      " );
          sb.append( actor.propertiesToString() );
        }

        i = newPlayers.iterator();
        while( i.hasNext() ) {
          actor = (UTActor) i.next();
          sb.append( "\n  + " );
          sb.append( actor );
          sb.append( "\n      " );
          sb.append( actor.propertiesToString() );
        }

        log.logNote( sb.toString() );
      }

      vizMap.repaint();
    }

    public void receivedError( Throwable error ) {
      log.logError( error.toString(), error );
    }

    public void disconnected() {
      log.logNote( "Disconnected: "+new Date() );
      isConnected = false;
      vizMap.repaint();
    }
  };

  protected class VizMap extends UTMapUI {
    protected void paintComponent( Graphics g ) {
      super.paintComponent( g );

      Font font = getFont();
      g.setFont( font );
      FontMetrics fm = getFontMetrics( font );
      Dimension size = getSize();

      if( !isConnected ) {
        final String message = "Disconnected";
        int width = fm.stringWidth( message );

        g.setColor( Color.red );
        g.drawString( message, size.width-width-2, size.height-fm.getDescent()-2 );
      }
    }

    protected void drawPlayerSymbol( Graphics2D g, double pitch, double yaw ) {
      double length = symbolSize*Math.cos( pitch );
      double x = length * Math.cos( yaw );
      double y = length * Math.sin( yaw );

      g.draw( playerCircle );
      g.rotate( yaw );
      g.draw( visionArc );
    }
  }
}

