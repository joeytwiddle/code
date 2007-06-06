package edu.cmu.gamebots;

//  edu.isi.gamebots.examples.ExampleBot
//  Copyright 2000, University of Southern California,
//                  Information Science Institute
//
//  Personal and Educational use is hereby granted.
//  Permission required for commercial use and redistribution.

// CMU Gamebots Team:
// Minor hacks added to make it play domination a little better...
// Mods by [srs] Steve Schaffer (srs3+@andrew.cmu.edu)
//       [chris] Chris Sollitto (cs3@andrew.cmu.edu)


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
 *  @author <a href="mailto:amarshal+gamebots@isi.edu">Andrew n marshall</a>
 *  @author <a href="mailto:srs3+@andrew.cmu.edu">Steve Schaffer</a>
 *  @author <a href="mailto:cs3@andrew.cmu.edu'>Chris Sollitto</a>
 */
public class CMU_JBot extends Bot {
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

  //[srs]
  protected static final int DOMINATING = 4;
  protected int shotTime = 0;
  protected boolean gotWeapon = false;

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

  protected long domTargetChangeTime = 0; //[srs]
  protected long pathRelock = 0;
  protected int domTarget = 1;
  protected int domsfound = 0;
  protected String dom1id = "-";
  protected String dom2id = "-";
  protected String dom3id = "-";
  protected Vector3D dom1loc;
  protected Vector3D dom2loc;
  protected Vector3D dom3loc;
  protected int dom1owner = TEAM_NONE;
  protected int dom2owner = TEAM_NONE;
  protected int dom3owner = TEAM_NONE;
  protected Vector3D previousHere;
  protected Message domPath;
  protected int domPathStep = -1;
  protected boolean askPath = false;

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

              case DOMINATING: //[srs]
                dominate();
                break;
              }
            } // end sync
          } // end if

          // [srs] thread.sleep( 1000l );
          //[srs] keep from shooting at walls, etc.
          for( int i = 0; i<6; i++ )
          {
             if(didInit && shotTime>0)
             {
               shotTime--;
               if(shotTime==0)
               {
                stopShooting();
               }
             }
             thread.sleep( 1000 );
          }

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
    long now = System.currentTimeMillis();
//    log.logNote("[srs] --ASYNCH-- ("+message.getType()+") @"+now);
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
      else if( message.getType().equals( DIE ) )
      { //[srs]
          log.logNote("[srs] DIED!");
           gotWeapon = false; //flag need weapon
           shotTime = 0; //stop shooting
           domTargetChangeTime=now;
           askPath = false;
           domPath = null;
           domPathStep = -1;
      }
      else if( message.getType().equals( SEE ) )
      { //[srs]
          if (Integer.parseInt(message.getProperty("Team"))!=team)
            shootAt(message.getProperty( ACTOR_ID )); //shoot any enemies
      }
      else if( message.getType().equals( ITEM ) )
      { //[srs] get item
        String i = message.getProperty( ITEM_ID );
        if(i.indexOf(WEAPON_1)!=-1 || i.indexOf(WEAPON_2)!=-1 )
        {
         gotWeapon = true; //flag we are armed
        }
      }
    else if (message.getType().equals( PATH ) )
      {
          log.logNote( "[srs] Recieved Path ######  " );
         if(askPath==true) domPath = message; //store it

      }
    }else if( message.getType().equals( INFO ) ) {
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
    long now = System.currentTimeMillis();
//    log.logNote("[srs] SYNCH -- "+now );

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
        } else if( type.equals( PLAYER ) )
        {
          /* ---- CHRIS added ---- */
          if (Integer.parseInt(message.getProperty("Team"))!=team)
            shootAt(message.getProperty( ACTOR_ID ));
          /*  ------------------- */

      }
      else if( type.equals( GAMESTATE ) )
      { //[srs] get updated dom info

        if(!dom1id.equals("-"))
        { String s = message.getProperty( dom1id ); //actually the parser is a bit broken on this issue... but we'll deal with it
          if(s!=null) dom1owner = Integer.parseInt(s);
        }
        if(!dom2id.equals("-"))
        { String s = message.getProperty( dom2id );
          if(s!=null) dom2owner = Integer.parseInt(s);
        }
        if(!dom3id.equals("-"))
        { String s = message.getProperty( dom3id );
          if(s!=null) dom3owner = Integer.parseInt(s);
        }

      }
      else { //some kind of nav node (or dom, or mov) [srs]
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

          if( type.equals( NAV )|| type.equals( MOV ) )
          {
              node.type = NODE_NAVIGATION;
              node.owner = TEAM_NONE;
          }
          else if ( type.equals( DOM ) )
          {
              node.type = NODE_DOMINATION;
              node.owner = Integer.parseInt( message.getProperty( OWNER ) );

              //[srs] store away locs of all doms found
              if( domsfound < 3 ) //[srs] this might be a previous unknown one
              {
                String id = message.getProperty( ACTOR_ID );
                if( !id.equals(dom1id) && !id.equals(dom2id) && !id.equals(dom3id) ) //[srs] haven't seen before
                {
                    value = message.getProperty( LOCATION );
                    if( value != null ) {
                        double vector[] = parseVector( value );

                        if(domsfound==0) {dom1id=id; dom1loc = new Vector3D(vector[0],vector[1],vector[2]);}
                        if(domsfound==1) {dom2id=id; dom2loc = new Vector3D(vector[0],vector[1],vector[2]);}
                        if(domsfound==2) {dom3id=id; dom3loc = new Vector3D(vector[0],vector[1],vector[2]);}
                        domsfound++;
                    }
                }
              }
          }
          else if ( type.equals( INV ) )
          {
              node.type = NODE_INVENTORY;
              node.owner = TEAM_NONE;
          }


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
        gotWeapon = true; //[srs]
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

//    log.logNote( "[srs] Exploring... (found "+domsfound+" doms)" );

    if( domsfound>=3 && now-stateChangeTime>15000 )
    { dominate(); return; //don't need to explore if we found all doms [srs]
    }

    if( domsfound>0 && now-stateChangeTime>40000 )
    {
     dominate(); return; //if it's been a real long time exploring - start dom
    }

    // Atomic copy (need a consistent set of variables, but won't write to them;
    final double x, y, z, yaw, pitch, roll;
    synchronized( selfLock ) {
      x = CMU_JBot.this.x;
      y = CMU_JBot.this.y;
      z = CMU_JBot.this.z;
      yaw = CMU_JBot.this.yaw;
      pitch = CMU_JBot.this.pitch;
    }

    synchronized( nodeInfoLock ) {
      if( reachableNodes == null )
      {
//[srs]            return;  // No game update yet.
          findNewTarget();//[srs] try to find some reachable ones.
      }

      if( target == null ) {
        targetLostTime = now;
      }
      //[srs] //elseif..>
      if( reachableNodes.contains( target ) ) {
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
        Node node;

//    log.logNote( "[srs]Looking for interesting..." );
         boolean retargetNow = false;
         Iterator t = reachableNodes.iterator();
         while( t.hasNext() )
         {
             node = (Node) t.next();
             if( node.type == NODE_DOMINATION && node.owner!=team )
             { //opponent's dom point - go for it!
    log.logNote( "[srs] Opponent's DOM!" );
                 retargetNow = true;
                 //state = DOMINATING  -- to do
             }
             else if( gotWeapon = false && node.type == NODE_INVENTORY )
             { //need a weapon? (but only if there's not a dom)
    log.logNote( "[srs]Want a weapon!" );
                 retargetNow = true;
             }

             if(retargetNow==true)
             { //lock on to the point
               direction = NONE; interferenceType = NONE; targetAcquiredTime = now;
               if( target != null ) lastTarget = target;
               target = node; runTo( target.id );
               return;      //[srs] if no retarget, just fall through. else end here.
             }
         }


         { // {} by [srs]  pick random node
//    log.logNote( "[srs]Generic explore...." );

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
           { if( logIntents )
               log.logNote( "No visible nodes." );
             findNewTarget();
           } // {} by [srs]
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

         log.logNote( "[srs] Exploring... (found "+domsfound+" doms) new targ:"+target );

        }

      }
    }
  }

  protected void heal() {
  }

  protected void hunt() {
  }

  protected void dominate() //[srs] go do dom points and take control of them
  {
     Thread thread = Thread.currentThread();
     long now = System.currentTimeMillis();

    if( state != DOMINATING ) {
      state = DOMINATING;
      stateChangeTime = now;
    }

//    log.logNote( "[srs] -DOMINATING-! ("+domsfound+" doms found)" );

    if( domsfound==0 || (domsfound<3 && now-stateChangeTime>20000 ) )
        //[srs] havent found all of them and within reasonable time (or don't know any)
    {
        log.logNote( "[srs] Not dominating ");

      explore(); return;
    }

    if(askPath==false && (now-domTargetChangeTime)>3000)
    {
        domTarget = chooseDomTarget();

        log.logNote( "[srs] DomTarget : "+domTarget );
    //    if(random.nextInt(100)>77) {    log.logNote( "[srs] not dominating: " ); explore();return;} //still explore with some prob.


    // ask for a path to the doms
        log.logNote( "[srs] Getting path... ");
        askPath=true; domPathStep=-1;
        if(domTarget==1) pathTo( dom1loc.x, dom1loc.y, dom1loc.z );
        if(domTarget==2) pathTo( dom2loc.x, dom2loc.y, dom2loc.z );
        if(domTarget==3) pathTo( dom3loc.x, dom3loc.y, dom3loc.z );

        try{thread.sleep(5000); }  //give server a chance to find us a path
        catch( InterruptedException error ) {
            // ignore + I HATE JAVA, BTFW
        }
    }


    if(askPath==true && domPath!=null) //we got a path back - should be following it.
    {
       Vector3D loc;
       if(domPathStep>=0) loc = parseStep(domPath,domPathStep);
       else loc = parseStep(domPath,0);

       if(loc!=null)
       {
//           log.logNote("[srs] Following Path to "+loc.x+","+loc.y+","+loc.z);
//           runTo(loc.x,loc.y,loc.z);
       }

       if(loc==null) //if we've reached the end of the path
       {
           log.logNote("[srs] Path complete!");
            askPath=false;
            domPathStep=-1;
            domPath=null;
            return;
       }
       else if(domPathStep==-1)
       {
           if(loc!=null) {
               log.logNote("[srs] Starting path: runto("+loc.x+","+loc.y+","+loc.z+")");
                runTo(loc.x,loc.y,loc.z);
                pathRelock = now;
           }
          domPathStep++;
       }
       else if (loc.near( x, y, z, PLAYER_RADIUS ))
       {
           log.logNote("[srs] Path point complete: "+domPathStep);
           domPathStep++;

           loc = parseStep(domPath,domPathStep);
           if (loc!=null)
           {    log.logNote("[srs] Continuing path: runto("+loc.x+","+loc.y+","+loc.z+")");
                runTo(loc.x,loc.y,loc.z);
                pathRelock = now;
           }
       }
       else
       {
           if(previousHere==null) previousHere = new Vector3D(0,0,0);
           if( previousHere.near(x,y,z, PLAYER_RADIUS) && now-pathRelock>10000 ) { //try and get out of stuck on ramp, etc
               log.logNote("[srs] Stuck on path.. jitter!"); jitter();
               try{thread.sleep(100000);} catch( InterruptedException error ) {}; pathRelock=0;
               askPath=false; dominate(); return;
           }
           previousHere.x = x; previousHere.y = y; previousHere.z = z;

       }


    }

  }

  protected Vector3D parseStep(Message message, int step)
  {
      Vector3D temp = null;

      String s = message.getProperty( Integer.toString(step) );
      if(s==null) return null;

      String loc = s.substring(s.indexOf(" "));
      if(loc==null) return null;

      try { double[] vector = parseVector( loc );
          temp = new Vector3D( vector[0], vector[1], vector[2] );
        } catch( RuntimeException error ) {  }
      return temp;

  }

  //[srs]
  protected void pathTo( double x, double y, double z )
  {
    log.logNote( "[srs] Getting Path to: "+x+","+y+","+z );
    Properties props = new Properties();
      props.setProperty( LOCATION, x+" "+y+" "+z );
      props.setProperty( ACTOR_ID, Integer.toString(111) ); //"unique" id
    client.sendMessage( GETPATH, props );
  }

  //[srs]
 protected int chooseDomTarget()
 {
 //    log.logNote( "[srs]Choosing new dom: " );
    if(domsfound>=1 && dom1owner!=team && random.nextBoolean() ) return 1;
    if(domsfound>=3 && dom3owner!=team && random.nextBoolean() ) return 3;
    if(domsfound>=2 && dom2owner!=team && random.nextBoolean() ) return 2;

    int v = random.nextInt(2)+1;
    if( v<=domsfound ) return v;

    return 1;
 }

  /* ---- CHRIS added ---- */
  protected void shootAt( String target )
  {
    //[srs] shoot only one thing at a time
    if(shotTime>0) stopShooting();

    if( target == null )
      throw new IllegalArgumentException( "Target cannot be null." );

    Properties props = new Properties();
      props.setProperty( ARG_TARGET, target );
    client.sendMessage( SHOOT, props );
    shotTime = 2; //[srs] shoot for 2 ticks
    //log.logNote( "[srs]Shooting at: "+target );

  }
  /* --------------------- */

  //[srs]
  protected void stopShooting(  )
  {
     Properties props = new Properties();
     client.sendMessage( STOP_SHOOT, props );
  }


  protected void findNewTarget() {
//    log.logNote( "[srs]Finding new target!" );

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
    public final String id;
    public int type = NONE; // [srs] done.
    public int owner = TEAM_NONE;  //[srs] if a dom node

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
        x = CMU_JBot.this.x;
        y = CMU_JBot.this.y;
        z = CMU_JBot.this.z;
        yaw = CMU_JBot.this.yaw;
        pitch = CMU_JBot.this.pitch;
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
