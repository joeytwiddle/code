package edu.isi.gamebots.examples;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.*;
//import java.lang.*;
import java.lang.*;
import java.util.*;

import javax.swing.*;

import edu.isi.gamebots.client.GamebotsConstants;
import edu.isi.gamebots.examples.*;


/**
 *  Mapping component for client using the UTActor class.  It uses the
 *  <a href="http://www.javasoft.com/products/jdk/1.2/docs/guide/2d/index.html">Java2D API</a>
 *  to draw the map.
 */
class UTMapUI extends JComponent
              implements GamebotsConstants {
  // Private Constants
  ///////////////////////////////////////////////////////////////////////////
  protected static final double minSymbolSize = 8d;
  protected static final double symbolSize = 12d;

  protected static final Color COLOR_TEAM_GOLD = new Color( 0xCC9900 );  // Gold
  protected static final Color COLOR_TEAM_DEFAULT = Color.black;

  protected static final Color COLOR_BACKGROUND = Color.white;
  protected static final Color COLOR_AXIS = Color.lightGray;

  protected static final Color COLOR_PICKUP = Color.cyan;

  protected static final Color COLOR_NAVPOINT = Color.lightGray;

/*
  protected static final Color COLOR_NODE = Color.gray;
  protected static final Color COLOR_NODE_VISIBLE = Color.darkGray;
  protected static final Color COLOR_NODE_TARGET = Color.pink;
  protected static final Color COLOR_NODE_TARGET_VISIBLE = Color.magenta;
  protected static final Color COLOR_PATH = new Color( 0xEEEEEE );  // Very light gray
*/

  protected static final Line2D xAxis = new Line2D.Double( -Double.MAX_VALUE, 0, Double.MAX_VALUE, 0 );
  protected static final Line2D yAxis = new Line2D.Double( 0, -Double.MAX_VALUE, 0, Double.MAX_VALUE );
  protected static final Ellipse2D playerCircle = new Ellipse2D.Double( -symbolSize/2, -symbolSize/2,
                                                                        symbolSize,    symbolSize );
  protected static final Ellipse2D navPointCircle = new Ellipse2D.Double( -symbolSize/4, -symbolSize/4,
                                                                          symbolSize/2,  symbolSize/2 );

  // Private Data
  ///////////////////////////////////////////////////////////////////////////
  protected Object scaleLock = new Object();
  protected double scale, symbolScale;

  protected Object centerLock = new Object();
  protected Vector3D center = new Vector3D( 0d, 0d, 0d );

  protected List actorSets = new LinkedList();  // Set of ActorSets

  //  Public Methods
  ///////////////////////////////////////////////////////////////////////////
  public UTMapUI() {
    setDoubleBuffered( true );
    setScale( minSymbolSize/(2d*PLAYER_RADIUS) );
  }

  public void addActorSet( Set actors, Object lock ) {
    synchronized( actorSets ) {
      ActorSet actorSet;
      if( lock != null )
        actorSet = new ActorSet( actors, lock );
      else
        actorSet = new ActorSet( actors );

      actorSets.add( actorSet );
    }
  }

  public void addActorSet( Set actors ) {
    synchronized( actorSets ) {
      actorSets.add( new ActorSet( actors ) );
    }
  }

  public void removeActorSet( Set actors ) {
    synchronized( actorSets ) {
      Iterator i = actorSets.iterator();

      ActorSet actorSet;
      while( i.hasNext() ) {
        actorSet = (ActorSet) i.next();
        if( actorSet.set == actors ) {
          i.remove();
          return;
        }
      }
    } // end sync
  }

  public void setCenter( Vector3D center, Object lock ) {
    synchronized( centerLock ) {
      if( lock != null )
        centerLock = lock;
      else
        centerLock = new Object();

      this.center = center;
    }
  }

  public double getScale() {
    return scale;
  }

  public void setScale( double scale ) {
    synchronized( scaleLock ) {
      this.scale = scale;
      this.symbolScale = Math.max( 2*PLAYER_RADIUS, minSymbolSize/scale )/symbolSize;
    }
    repaint();
  }

  //  Private Methods
  ///////////////////////////////////////////////////////////////////////////
  protected void paintComponent( Graphics g ) {
    Color oldColor = g.getColor();
    Font  oldFont  = g.getFont();

    Font font = getFont();
    g.setFont( font );
    FontMetrics fm = getFontMetrics( font );
    Dimension size = getSize();

    if( g instanceof Graphics2D ) {
      // Clear background
      g.setColor( COLOR_BACKGROUND );
      g.fillRect( 0, 0, size.width, size.height );

      if( paintComponent2D( (Graphics2D) g.create() ) )
        repaint();
    } else {
      // Clear background
      g.setColor( getBackground() );
      g.fillRect( 0, 0, size.width, size.height );

      g.setColor( getForeground() );
      g.drawString( "Requires Graphics2D", 2, fm.getAscent()+2 );
    }

    g.setColor( oldColor );
    g.setFont( oldFont );
  }

  protected boolean paintComponent2D( Graphics2D g ) {
    Graphics2D g2;
    Iterator i, i2;
    UTActor actor;
    boolean needRepaint = false;

    Dimension size = getSize();
    Rectangle2D.Double bounds = new Rectangle2D.Double( 0, 0, size.width, size.height );

    // Atomic copy (need a consistent set of variables, but won't write to them;
    final double scale, symbolScale;
    synchronized( scaleLock ) {
      scale = this.scale;
      symbolScale = this.symbolScale;
    }

    final double x, y;
    synchronized( centerLock ) {
      x = center.x;
      y = center.y;
    }

    // Scale && translate
    g.translate( bounds.width/2, bounds.height/2 );
    g.scale( scale, scale );
    g.translate( -x, -y );

    // Draw Origin
    g.setColor( COLOR_AXIS );
    g.draw( xAxis );
    g.draw( yAxis );

    ActorSet actorSet;
//    synchronized( actorSets ) {
      i = actorSets.iterator();

      while( i.hasNext() ) {
        actorSet = (ActorSet) i.next();
        synchronized( actorSet.lock ) {
          try {
            i2 = actorSet.set.iterator();
            while( i2.hasNext() ) {
              actor = (UTActor) i2.next();

              g2 = (Graphics2D) g.create();
                g2.translate( actor.loc.x, actor.loc.y );
                g2.scale( symbolScale, symbolScale );
                drawActor( g2, actor );
              g2.dispose();
            }
          } catch( RuntimeException error ) {
            // Probably just the set being manipulated..
            needRepaint = true;
          }
        }
      }
//    }  // end synch

    return needRepaint;
  }

  protected void drawActor( Graphics2D g, UTActor actor ) {
    if( actor instanceof UTPawn )
      drawPlayer( g, (UTPawn) actor );
    else if( actor.instanceOf( UTClass.PICKUP ) )
      drawPickup( g, actor );
    else
      drawNavPoint( g, actor );
  }

  protected void drawPlayer( Graphics2D g, UTPawn player ) {
    switch( player.team ) {
    case TEAM_RED:
      g.setColor( Color.red );
      break;
    case TEAM_BLUE:
      g.setColor( Color.blue );
      break;
    case TEAM_GREEN:
      g.setColor( Color.green );
      break;
    case TEAM_GOLD:
      g.setColor( COLOR_TEAM_GOLD );
      break;
    default:
      g.setColor( COLOR_TEAM_DEFAULT );
      break;
    }

    double pitch, yaw;
//    synchronized( player ) {
      Rotation3D rot = player.rot;
//      synchronized( rot ) {
        pitch = rot.pitch;
        yaw   = rot.yaw;
//      }
//    }

    drawPlayerSymbol( g, pitch, yaw );
  }

  protected void drawPlayerSymbol( Graphics2D g, double pitch, double yaw ) {
    double length = symbolSize*Math.cos( pitch );
    double x = length * Math.cos( yaw );
    double y = length * Math.sin( yaw );

    g.draw( playerCircle );
    g.draw( new Line2D.Double( 0d, 0d, x, y ) );
  }

  protected void drawPickup( Graphics2D g, UTActor pickup ) {
    g.setColor( COLOR_PICKUP );
    g.draw( navPointCircle );
  }

  protected void drawNavPoint( Graphics2D g, UTActor navPoint ) {
    g.setColor( COLOR_NAVPOINT );
    g.draw( navPointCircle );
  }

  //  Inner Classes
  ///////////////////////////////////////////////////////////////////////////
  protected class ActorSet {
    public final Set    set;
    public final Object lock;

    public ActorSet( Set set, Object lock ) {
      this.set  = set;
      this.lock = lock;
    }

    public ActorSet( Set set ) {
      this.set  = set;
      this.lock = this;
    }
  }
}
