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

import edu.isi.gamebots.client.*;
import edu.isi.gamebots.examples.*;


/**
 *  This example implementation the Bot class shows basic message handling, Bot
 *  control, and and uses {@link UTMapUI} for a map in its user interface.
 *
 *  @author <a href="mailto:amarshal#gamebots@isi.edu">Andrew n marshall</a>
 */
public class ExampleBot extends Bot {
  // Private Static Data
  ///////////////////////////////////////////////////////////////////////////
  protected static final int MAX_OUTPUT = 100;
  protected static final double PLAYER_RADIUS = 18.0d;
  protected static final double THETA_DELTA = (2d*Math.PI)/60d;  // Angular Delta (6 deg)
  protected static final int NONE = 0;

  // state
  protected static final int EXPLORING = 1;
  protected static final int HEALING = 2;
  protected static final int HUNTING = 3;

  // interference
  protected static final int INTERFERENCE_BUMP = 1;
  protected static final int INTERFERENCE_WALL = 2;
  protected static final int INTERFERENCE_DAMAGE = 3;

  // directional tendancy
  protected static final int LEFT = 1;
  protected static final int RIGHT = 2;

  /**
   *  Compares Paths based on theta.
   *
   *  Note: this comparator imposes orderings that are inconsistent with equals(...).
   */
  protected static final Comparator pathComparator = new Comparator() {
    public int compare( Object aObj, Object bObj ) {
      Path a = (Path) aObj;
      Path b = (Path) bObj;

      if( a.theta < b.theta )
        return -1;
      if( a.theta == b.theta )
        return 0;
      else // a.theta > b.theta
        return 1;
    }
  };

  // Private Data
  ///////////////////////////////////////////////////////////////////////
  protected NodeMap map = new NodeMap();

  protected Random random = new Random();

  protected Thread runnerThread;

  protected boolean logIntents  = false;
  protected boolean logMessages = false;
  protected boolean logSelf     = false;

  protected boolean didInit = false;

  protected Object stateLock = new Object();
  protected long stateChangeTime = 0;
  protected int state = EXPLORING;
  protected int direction = NONE;  // for rot/straft tendancies..

  /**
   *  This lock is used to synchronized reads from/writes to the various state
   *  variables.
   */
  protected final Object selfLock = new Object();
  protected double x, y, z;  // in world coordinates
  protected double yaw, pitch, roll;  // in Radians
  protected int health, armor, ammo;
  protected int team = 255;
  protected String weapon;
  protected Vector3D here;  // Location as of the last vision update

  protected boolean interfered = false;
  protected long interferenceTime = 0;
  protected int  interferenceType = NONE;

  protected long lastRotIncTime = 0;
  protected double yawTarget;

  protected Object nodeInfoLock = new Object();
  protected java.util.Map idToNode = new HashMap();  // Id String to Node object
  protected Collection knownNodes = idToNode.values();  // automatic updates
  protected Set exploredNodes = new HashSet();
  protected Set visibleNodes = new HashSet();
  protected Set reachableNodes = new HashSet();

  protected Node target, lastTarget;
  protected long targetAcquiredTime = 0;
  protected long targetLostTime = 0;

  //  Public Methods
  ///////////////////////////////////////////////////////////////////////
  public JComponent getView() {
    return map;
  }

  //  Protected Methods
  ///////////////////////////////////////////////////////////////////////

  // runner
  protected Runnable runner = new Runnable() {
    public void run() {
      Thread thread = Thread.currentThread();
      while( thread == runnerThread ) {
        try {
          if( didInit ) {
            synchronized( stateLock ) {
              switch( state ) {
              default:
              case EXPLORING:
                explore();
                break;

              case HEALING:
                heal();
                break;

              case HUNTING:
                hunt();
                break;
              }
            } // end sync
          } // end if
          thread.sleep( 1000l );
        } catch( InterruptedException error ) {
          // ignore
        }
      }
    }
  };

  // Event Handlers
  protected void connected() {
    super.connected();

    log.logNote( "Connected... ("+new Date()+")" );

    runnerThread = new Thread( runner );
    runnerThread.start();

    map.repaint();
  }

  protected void receivedAsyncMessage( Message message ) {
    if( didInit ) {
      if( message.getType().equals( BUMP ) ) {
        interferenceTime = System.currentTimeMillis();
        interferenceType = INTERFERENCE_BUMP;
        interfered = true;
      } else if( message.getType().equals( WALL ) ) {
        interferenceTime = System.currentTimeMillis();
        interferenceType = INTERFERENCE_WALL;
        interfered = true;
      } else if( message.getType().equals( DAMAGE ) ) {
        interferenceTime = System.currentTimeMillis();
        interferenceType = INTERFERENCE_DAMAGE;
        interfered = true;
      }
    } else if( message.getType().equals( INFO ) ) {
      // Init
      //  Should check to make sure it is only the first...
      Properties props = new Properties();
        props.setProperty( client.PLAYER_NAME, getName() );
        int team = getInitialTeam();
        if( team != TEAM_ANY )
          props.setProperty( client.PLAYER_TEAM, Integer.toString(team) );
      client.sendMessage( client.INIT, props );
      didInit = true;
    }

    if( logMessages )
      log.logNote( message );
  }

  protected void receivedSyncMessage( MessageBlock block ) {
    StringBuffer sb = new StringBuffer();
    if( logMessages ) {
      sb.append( block );
    }

    synchronized( selfLock ) {
      if( here == null ||
          ( here.x != x ||
            here.y != y ||
            here.z != z ) )
        here = new Vector3D( x, y, z );
    }
    Message message;
    String type, value;
    Node node;

    synchronized( nodeInfoLock ) {
      visibleNodes.clear();
      reachableNodes.clear();

      Iterator i = block.getMessages();
      while( i.hasNext() ) {
        message = (Message) i.next();
        type = message.getType();
        if( type.equals( END ) ) {
          // ignore
        } else if( message.getType().equals( SELF ) ) {
          if( logSelf )
            log.logNote( message );
          updateSelf( message );
          if( logSelf )
            log.logNote( "DEBUG: Loc: "+x+" "+y+" "+z+";  Rot: "+pitch+" "+yaw+" "+roll );
        } else if( type.equals( PLAYER ) ) {
          // TO-DO
        } else {
          node = (Node) idToNode.get( message.getProperty( ACTOR_ID ) );
          if( node == null ) {
            node = new Node( message );
            if( node.id != null )
              idToNode.put( node.id, node );
            else if( logMessages )
              sb.append( "\n * No ID: " );
          }
          value = message.getProperty( ACTOR_REACHABLE );
          if( value != null &&
              value.equals( TRUE ) ) {
            node.reachableFrom( here );
            reachableNodes.add( node );
          }
          visibleNodes.add( node );
        }
        if( logMessages ) {
          sb.append('\n');
          sb.append( message );
        }
      }

      runnerThread.interrupt();
      if( logMessages )
        log.logNote( sb.toString() );
    }

    map.repaint();
  }

  protected void disconnected() {
    didInit = false;

    log.logNote( "Disconnected... ("+new Date()+")" );

    if( runnerThread != null ) {
      Thread oldThread = runnerThread;
      runner = null;
      oldThread.interrupt();
    }

    map.repaint();
  }

  protected void updateSelf( Message message ) {
    String value;
    double vector[];

    synchronized( selfLock ) {
      value = message.getProperty( PLAYER_NAME );
      if( value != null )
        setName( value );

      value = message.getProperty( PLAYER_TEAM );
      if( value != null ) {
        try {
          team = Integer.parseInt( value );
        } catch( NumberFormatException error ) {
          // Log and ignore
          log.logError( "Illegal \""+PLAYER_TEAM+"\" value: "+value, error );
        }
      }

      value = message.getProperty( LOCATION );
      if( value != null ) {
        vector = parseVector( value );
        x = vector[0];
        y = vector[1];
        z = vector[2];
      }

      value = message.getProperty( ROTATION );
      if( value != null ) {
        vector = parseVector( value );
        pitch = vector[0] * 2 * Math.PI / 65535;
        yaw = vector[1] * 2 * Math.PI / 65535;
        roll = vector[2] * 2 * Math.PI /   65535;
      }

      value = message.getProperty( PLAYER_AMMO );
      if( value != null ) {
        try {
          ammo = Integer.parseInt( value );
        } catch( NumberFormatException error ) {
          // Log and ignore
          log.logError( "Illegal \""+PLAYER_AMMO+"\" value: "+value, error );
        }
      }

      value = message.getProperty( PLAYER_ARMOR );
      if( value != null ) {
        try {
          armor = Integer.parseInt( value );
        } catch( NumberFormatException error ) {
          // Log and ignore
          log.logError( "Illegal \""+PLAYER_ARMOR+"\" value: "+value, error );
        }
      }

      value = message.getProperty( PLAYER_HEALTH );
      if( value != null ) {
        try {
          health = Integer.parseInt( value );
        } catch( NumberFormatException error ) {
          // Log and ignore
          log.logError( "Illegal \""+PLAYER_HEALTH+"\" value: "+value, error );
        }
      }

      value = message.getProperty( PLAYER_WEAPON );
      if( value != null &&
          value != weapon )
        weapon = value;
    }

    synchronized( nodeInfoLock ) {
      if( target != null &&
          target.loc.near( x, y, z, PLAYER_RADIUS ) ) {
        exploredNodes.add( target );
        if( target != null )
          lastTarget = target;
        target = null;
      }
    }
    map.repaint();
  }

  protected void explore() {
    long now = System.currentTimeMillis();
    // Assumes synchronized on stateLock
    if( state != EXPLORING ) {
      state = EXPLORING;
      stateChangeTime = now;
    }

    // Atomic copy (need a consistent set of variables, but won't write to them;
    final double x, y, z, yaw, pitch, roll;
    synchronized( selfLock ) {
      x = ExampleBot.this.x;
      y = ExampleBot.this.y;
      z = ExampleBot.this.z;
      yaw = ExampleBot.this.yaw;
      pitch = ExampleBot.this.pitch;
    }

    synchronized( nodeInfoLock ) {
      if( reachableNodes == null )
        return;  // No game update yet.

      if( target != null ) {
        targetLostTime = now;
      } else if( reachableNodes.contains( target ) ) {
        long delay = interferenceTime - targetAcquiredTime;
        if( ( interferenceType == INTERFERENCE_WALL &&
              delay > 1000 ) ||
            ( interferenceType != NONE &&
              delay > 3000 ) ||
            ( delay > 6000 ) ) {  // Give up after some time
          if( logIntents )
            log.logNote( "Giving up on target. Interference Type: "+interferenceType+", delay: "+delay );
          findNewTarget();
        } else {
          // Otherwise continue persuing target.
          runTo( target.id );
        }
        return;
      }
      if( reachableNodes.isEmpty() ) {
        if( logIntents )
          log.logNote( "No visible nodes." );
        findNewTarget();
      } else {
        Set options = new HashSet( reachableNodes );
        options.removeAll( exploredNodes );
        if( options.isEmpty() ) {
          if( random.nextBoolean() ) {
            if( logIntents )
              log.logNote( "No unexplored nodes." );
            findNewTarget();
            return;
          }
          options = reachableNodes;
          if( lastTarget != null )
            reachableNodes.remove( lastTarget );
        }
        Node node;
        Iterator i = options.iterator();
        while( i.hasNext() ) {
          node = (Node) i.next();
          if( node.loc.near( x, y, z, PLAYER_RADIUS ) )
            i.remove();
        }
        if( options.isEmpty() ) {
          if( logIntents )
            log.logNote( "No distant nodes." );
          findNewTarget();
          return;
        }

        node = (Node) options.toArray()[ random.nextInt( options.size() ) ];
        if( target == null ) {
          if( lastTarget == node )
            if( logIntents )
              log.logNote( "No visible nodes." );
            findNewTarget();
        } else {
          if( target == node )
            findNewTarget();
        }

        direction = NONE;
        interferenceType = NONE;
        targetAcquiredTime = now;
        if( target != null )
          lastTarget = target;
        target = node;

        runTo( target.id );

        log.logNote( "Exploring new target: "+target );
      }
    }
  }

  protected void heal() {
  }

  protected void hunt() {
  }

  protected void findNewTarget() {
    interferenceType = NONE;
    synchronized( nodeInfoLock ) {
      if( target != null )
        lastTarget = target;
      target = null;
    }
    interfered = false;
    incRotation();
  }

  protected void incRotation() {
    long now = System.currentTimeMillis();
    if( now-lastRotIncTime < 200 ) {
//      log.logNote( "Don't turn yet." );
      return;
    }
    if( direction == NONE ) {
      if( random.nextBoolean() )
        direction = LEFT;
      else
        direction = RIGHT;
    }
    double newYaw;
    if( direction == LEFT )
      newYaw = yaw+(Math.PI/6);  // 15 deg
    else
      newYaw = yaw-(Math.PI/6);  // 15 deg
    if( newYaw == yawTarget ) {
      if( logIntents )
        log.logNote( "Stuck in rotation: Jittering." );
      jitter();  // get unstuck
    } else {
      if( logIntents )
        log.logNote( "Rotating to "+newYaw );
      turnTo( pitch, newYaw, 0 );
      yawTarget = newYaw;
      lastRotIncTime = now;
    }
  }

  protected void jitter() {
    switch( random.nextInt(8) ) {
    case 0:
      runTo( x+(PLAYER_RADIUS/2), y, z );
      break;
    case 1:
      runTo( x+(PLAYER_RADIUS/2), y+(PLAYER_RADIUS/2), z );
      break;
    case 2:
      runTo( x, y+(PLAYER_RADIUS/2), z );
      break;
    case 3:
      runTo( x-(PLAYER_RADIUS/2), y+(PLAYER_RADIUS/2), z );
      break;
    case 4:
      runTo( x-(PLAYER_RADIUS/2), y, z );
      break;
    case 5:
      runTo( x-(PLAYER_RADIUS/2), y-(PLAYER_RADIUS/2), z );
      break;
    case 6:
      runTo( x, y-(PLAYER_RADIUS/2), z );
      break;
    default:
      runTo( x+(PLAYER_RADIUS/2), y-(PLAYER_RADIUS/2), z );
      break;
    }
  }

  //  Inner Classes
  ///////////////////////////////////////////////////////////////////////////
  protected class Path {
    public final Vector3D to;   // Usually the location of a node
    public final Vector3D from;
    public final double theta;  // Angle on the x-y plane, from x axis
    public final double phi;    // Angle from the x-y plane
    public final double distSquared;

    public final Line2D line;

    public Path( Vector3D to, Vector3D from ) {
      this.to = to;
      this.from = from;

      double dx = to.x-from.x;
      double dy = to.y-from.y;
      double dz = to.z-from.z;
      theta = Math.atan2( dy, dx );
      double xyRadiusSquared = dx*dx + dy*dy;
      phi = Math.atan2( dz, Math.sqrt( xyRadiusSquared ) );
      distSquared = xyRadiusSquared + dz*dz;

      line = new Line2D.Double( from.x, from.y, to.x, to.y );
    }
  }

  protected class Node {
    // Public Data
    ///////////////////////////////////////////////////////////////////////
    public final String   id;
    public final int      type = NONE; // TO-DO: need to define node types
    public final Vector3D loc;
    public final SortedSet paths = new TreeSet( pathComparator );

    // Public Methods
    ///////////////////////////////////////////////////////////////////////
    public Node( Message message ) {
      String value = message.getProperty( LOCATION );
      Vector3D temp = null;
      if( value != null ) {
        try {
          double[] vector = parseVector( value );
          temp = new Vector3D( vector[0], vector[1], vector[2] );
        } catch( RuntimeException error ) {
          // temp
        }
      }
      loc = temp;
      id = message.getProperty( ACTOR_ID );
    }

    public boolean equals( Object obj ) {
      if( !(obj instanceof Node) )
        return false;
      Node node = (Node) obj;
      if( id == null )
        return node.id == null &&
               loc.near( node.loc, PLAYER_RADIUS );
      return id.equals( node.id );
    }

    public String toString() {
      return "{Node "+ACTOR_ID+"="+id+", "+LOCATION+"="+loc+"}";
    }

    public void reachableFrom( Vector3D from ) {
      synchronized( paths ) {
        Path path = new Path( loc, from );
        if( !paths.contains( path ) ) {
          SortedSet sub = paths.headSet( path );
          if( !sub.isEmpty() ) {
            Path prev = (Path) sub.last();
            if( prev != null &&
                (path.theta - prev.theta < THETA_DELTA) &&
                (path.distSquared > prev.distSquared) )
              paths.remove( prev );
          }
          sub = paths.tailSet( path );
          if( !sub.isEmpty() ) {
            Path next = (Path) sub.first();
            if( next != null &&
                (next.theta - path.theta < THETA_DELTA) &&
                (path.distSquared > next.distSquared) )
              paths.remove( next );
          }
          // TO-DO: special case theta's near +/-pi?
          paths.add( path );
        }
      }
    }
  }

  protected class NodeMap extends JComponent {
    // Private Constants
    ///////////////////////////////////////////////////////////////////////
    protected final double minSymbolSize = 8d;
    protected final double symbolSize = 12d;

    protected final Color COLOR_TEAM_GOLD = new Color( 0xCC9900 );  // Gold
    protected final Color COLOR_TEAM_DEFAULT = Color.black;

    protected final Color COLOR_BACKGROUND = Color.white;
    protected final Color COLOR_AXIS = Color.lightGray;

    protected final Color COLOR_NODE = Color.gray;
    protected final Color COLOR_NODE_VISIBLE = Color.darkGray;
    protected final Color COLOR_NODE_TARGET = Color.pink;
    protected final Color COLOR_NODE_TARGET_VISIBLE = Color.magenta;
    protected final Color COLOR_PATH = new Color( 0xEEEEEE );  // Very light gray

    // Private Data
    ///////////////////////////////////////////////////////////////////////
    protected Object scaleLock = new Object();
    protected double scale, symbolScale;

    protected Line2D xAxis = new Line2D.Double( -Double.MAX_VALUE, 0, Double.MAX_VALUE, 0 );
    protected Line2D yAxis = new Line2D.Double( 0, -Double.MAX_VALUE, 0, Double.MAX_VALUE );
    protected Ellipse2D playerCircle = new Ellipse2D.Double( -symbolSize/2, -symbolSize/2,
                                                             symbolSize,    symbolSize );
    protected Ellipse2D navPointCircle = new Ellipse2D.Double( -symbolSize/4, -symbolSize/4,
                                                               symbolSize/2,  symbolSize/2 );

    //  Public Methods
    ///////////////////////////////////////////////////////////////////////
    public NodeMap() {
      setDoubleBuffered( true );
      setScale( minSymbolSize/(2d*PLAYER_RADIUS) );
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
    ///////////////////////////////////////////////////////////////////////
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

      if( !client.isConnected() ) {
        final String message = "Disconnected";
        int width = fm.stringWidth( message );

        g.setColor( Color.red );
        g.drawString( message, size.width-width-2, size.height-fm.getDescent()-2 );
      }


      g.setColor( oldColor );
      g.setFont( oldFont );
    }

    protected boolean paintComponent2D( Graphics2D g ) {
      Graphics2D g2, g3;
      Iterator i, i2;
      Node node;
      Path path;
      boolean needRepaint = false;

      Dimension size = getSize();
      Rectangle2D.Double bounds = new Rectangle2D.Double( 0, 0, size.width, size.height );

      // Atomic copy (need a consistent set of variables, but won't write to them;
      final double scale, symbolScale;
      synchronized( scaleLock ) {
        scale = this.scale;
        symbolScale = this.symbolScale;
      }
      final double x, y, z, yaw, pitch;
      synchronized( selfLock ) {
        x = ExampleBot.this.x;
        y = ExampleBot.this.y;
        z = ExampleBot.this.z;
        yaw = ExampleBot.this.yaw;
        pitch = ExampleBot.this.pitch;
      }

      // Scale && translate
      g.translate( bounds.width/2, bounds.height/2 );
      g.scale( scale, scale );
      g2 = (Graphics2D) g.create();
        g2.translate( -x, -y );

        // Draw Origin
        g2.setColor( COLOR_AXIS );
        g2.draw( xAxis );
        g2.draw( yAxis );

//        synchronized( nodeInfoLock ) {
        try {
          // Draw Paths...
          i = knownNodes.iterator();
          while( i.hasNext() ) {
            node = (Node) i.next();

            i2 = node.paths.iterator();
            g2.setColor( COLOR_PATH );
            while( i2.hasNext() ) {
              path = (Path) i2.next();
              g2.draw( path.line );
            }
          }

          // Draw Nodes...
          i = knownNodes.iterator();
          while( i.hasNext() ) {
            node = (Node) i.next();

            g3 = (Graphics2D) g2.create();
              g3.translate( node.loc.x, node.loc.y );
              g3.scale( symbolScale, symbolScale );
              drawNode( g3, node );
            g3.dispose();
          }
        } catch( RuntimeException error ) {
          // Probably just a problem with not blocking while accessing the node Info
          needRepaint = true;
        }

        // Draw Players...

      g2.dispose();

      // Draw Self
      g2 = (Graphics2D) g.create();
        g2.scale( symbolScale, symbolScale );
        drawPlayer( g2, team, yaw, pitch );
      g2.dispose();

      return needRepaint;
    }

    protected void drawPlayer( Graphics2D g, int team, double yaw, double pitch ) {
      switch( team ) {
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

      double length = symbolSize*Math.cos( pitch );
      double x = length * Math.cos( yaw );
      double y = length * Math.sin( yaw );

      g.draw( playerCircle );
      g.draw( new Line2D.Double( 0d, 0d, x, y ) );
    }

    protected void drawNode( Graphics2D g, Node node ) {
      switch( node.type ) {
      default:
        if( visibleNodes.contains(node) ) {
          if( node == target )
            g.setColor( Color.magenta );
          else
            g.setColor( Color.darkGray );
        } else {
          if( node == target )
            g.setColor( Color.pink );
          else
            g.setColor( Color.lightGray );
        }
        g.draw( navPointCircle );
        break;
      }
    }
  }
}
