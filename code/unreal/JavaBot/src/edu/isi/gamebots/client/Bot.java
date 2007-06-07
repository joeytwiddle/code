//  edu.isi.gamebots.clients.Bot
//  Copyright 2000, University of Southern California,
//                  Information Science Institute
//
//  Personal and Educational use is hereby granted.
//  Permission required for commercial use and redistribution.


package edu.isi.gamebots.client;

import java.io.*;
import java.lang.*;
import java.net.*;
import java.util.*;

import javax.swing.*;
import javax.swing.border.EmptyBorder;

import us.ca.la.anm.gui.*;
import us.ca.la.anm.util.io.*;


/**
 *  <p>
 *  This is the base class for all Java Bots.  It handles most low level
 *  functionality, including bot life cycle hooks, and many helper functions.
 *  </p>
 *
 *  @author <a href="mailto:amarshal#gamebots@isi.edu">Andrew n marshall</a>
 */
public abstract class Bot implements GamebotsConstants {
  // Private Static Data
  ///////////////////////////////////////////////////////////////////////
  private static final int ERROR_MESSAGE = -1;
  private static final int SYNC_MESSAGE  =  1;
  private static final int ASYNC_MESSAGE =  2;
  
  
  // Private Data
  ///////////////////////////////////////////////////////////////////////
  /** 
   * <p>
   * The UTServer
   * </p>
   */  
  protected GamebotsClient client;
  /** 
   * <p>
   * A logging mechanisn
   * </p>
   */  
  protected DebugLog log;
  
  //  Event processing member data.
  private boolean isAlive = true;
  private final Vector eventQueue = new Vector();
  private Thread eventThread;
  
  private String name;
  private int initialTeam;
  
  //  Public Static Methods
  ///////////////////////////////////////////////////////////////////////////
  /**
   * <p>
   * Parses any comma or space delimited list of numbers into an array of
   * doubles.  Particularly useful for parsing location and rotation
   * information.
   * <p>
   * @param in The String object to parse
   * @throws NumberFormatException If there is a non-number in the list
   * @return an array of doubles
   */
  public static double[] parseVector( String in ) throws NumberFormatException {
    StringTokenizer st = new StringTokenizer( in, ", " );
    double[] result = new double[st.countTokens()];
    
    int i=0;
    while( st.hasMoreTokens() )
      result[i++] = Double.parseDouble( st.nextToken() );
    
    return result;
  }
  
  //  Public Methods
  ///////////////////////////////////////////////////////////////////////
  /**
   * <p>
   * Creates a new Bot
   * </p>
   */  
  public Bot() {
    eventThread = new Thread( eventProcessor );
    eventThread.start();
  }
  
  /**
   * <p>
   * Main initialization method for {@link BotRunnerApp}.  All Bot specific
   * initialization should occur in {@link #init}.
   * </p>
   * @param name Name of the bot
   * @param client UTServer that bot talks to
   * @param team Team that the bot is on
   * @param log Where to log debug messages to (can specify System.out to log to the console)
   */
  public final void init( String name, GamebotsClient client, int team, DebugLog log ) {
    this.name = name;
    this.client = client;
    this.initialTeam = team;
    this.log = log;
    
    client.addListener( clientListener );
    
    init();
  }
  
  /**
   * <p>
   * Get the bot's name
   * </p>
   * @return the Bot name.
   */
  public final String getName() {
    return name;
  }
  
  /**
   * <p>
   * Get the server that this bot is running on.
   * </p>
   * @return the bot's {@link GamebotsClient}.
   */
  public final GamebotsClient getClient() {
    return client;
  }
  
  /**
   * <p>
   * Return the name of the team that the bot was initialized to.
   * </p>
   * @return <CODE>int</CODE> team that the bot belongs to
   */  
  public final int getInitialTeam() {
    return initialTeam;
  }
  
  /**
   * <p>
   * Returns a user interface for the bot. This is needed if you want to use the
   * BotRunnerApp to start and view the status of your bots. The JComponent that
   * this returns is what gets added to a new tab in the interface.
   * </p>
   * @return a Swing user interface for the bot.
   */
  public JComponent getView() {
    return new JLabel( getName()+" ("+getClass().getName()+")" );
  }
  
  /**
   *  This initiates a connection with the server.  It is called from the
   *  {@link BotRunnerApp}.
   */
  public final void connect() {
    try {
      client.connect();
    } catch( IOException error ) {
      log.logError( error );
      log.flush();
    }
  }
  
  /**
   *  This forces disconnect with the server.  It is called from the
   *  {@link BotRunnerApp}.
   */
  public final void disconnect() {
    client.disconnect();
  }
  
  // Event Handlers
  /**
   * <p>
   * Event Handler. Override this method to define what to do when the bot is
   * distroyed/disconnected by the user.
   * </p>
   */  
  public final void destroy() {
    isAlive = false;
    eventThread.interrupt();
    
    destroyImpl();
  }
  
  /** 
   * <p>
   * The bot runs to a perticular target.
   * </p>
   * @param target The UnrealTournament ID to run to
   */  
  public void runTo( String target ) {
    if( target == null )
      throw new IllegalArgumentException( "Target cannot be null." );
    
    Properties props = new Properties();
    props.setProperty( ARG_TARGET, target );
    client.sendMessage( RUNTO, props );
  }
  
   /** 
   * <p>
   * The bot asks for a path to an xyz coordinate
   * </p>
   * @param target The UnrealTournament ID to run to
   */  
  public void getPath(String pathName, double x, double y, double z) {    
    Properties props = new Properties();
    props.setProperty( "Id", pathName);
    props.setProperty( "Location", x + " " + y + " " + z);
    client.sendMessage( "GETPATH", props );
  }
  
  /**
   * <p>
   * The bot runs to a perticular location
   * </p>
   * @param x global x coordinate in the world
   * @param y global y coordinate in the world
   * @param z global z coordinate in the world
   */  
  public void runTo( double x, double y, double z ) {
    Properties props = new Properties();
    props.setProperty( LOCATION, x+" "+y+" "+z );
    client.sendMessage( RUNTO, props );
  }
  
  /**
   * <p>
   * Return the log for the bot
   * <p>
   *
   * @return The <CODE>DebugLog</CODE> of the agent
   */  
  public DebugLog getLog(){
    return log;
  }
  
  /**
   * <p>
   * Turn a particular amount in three dimentional space. Yaw is side to side, pitch
   * up and down, and Roll the equivelent of doing a cartwheel. You probably won't
   * need Roll, so don't sweat it. A full rotation using UT's measurements is 65535.
   * To convert the values you are sent to radiens, divide by 65535 and multiply by
   * 2 * pi.  This is already converted for you to radians though.
   * UnrealTournament units.
   * </p>
   * @param pitch Amount to look up or down in radians
   * @param yaw Amount to turn side to side in radians
   * @param roll Amount of 'roll' rotation. Not sure what this does.
   */  
  public void turnTo( double pitch, double yaw, double roll ) {
    Properties props = new Properties();
    // TO-DO: This will soon become yaw,pitch,roll
    int p = ((int) (pitch*65536/(2*Math.PI)))%65536;
    int y = ((int) (yaw*65536/(2*Math.PI)))%65536;
    int r = ((int) (roll*65536/(2*Math.PI)))%65536;
    props.setProperty( ROTATION, p+" "+y+" "+r );
    client.sendMessage( TURNTO, props );
  }
  
  // the following bot commands added by Ryan Rozich - Texas A&M University for completeness
  
  /**
   * <p>
   * Turn towards a particular location in space
   * </p>
   * @param x The global x coordinate to turn toward
   * @param y The global y coordinate to turn toward
   * @param z The global z coordinate to turn toward
   */  
  public void turnToLoc(double x, double y, double z){
    Properties props = new Properties();
    // TO-DO: This will soon become yaw,pitch,roll
    props.setProperty( LOCATION, x+" "+y+" "+z );
    client.sendMessage( TURNTO, props );
  }
  
  /**
   * <p>
   * Turn towards a particular location in space
   * </p>
   * @param coords Same as turnTo(double, double, double) except this takes the three corrdinates as a single string.
   */  
  public void turnToLoc(String coords){
    Properties props = new Properties();
    // TO-DO: This will soon become yaw,pitch,roll
    props.setProperty( LOCATION, coords);
    client.sendMessage( TURNTO, props );
  }
  
  /**
   * <p>
   * Set the agent to walk instead of run. It seems intuitive that when true is sent,
   * the agent should walk. But in our tests, it seems the agent is set to run when
   * true is sent and walks when false is sent.
   * </p>
   * @param doWalk We think that <CODE>true</CODE> means run and <CODE>false</CODE> means walk.
   */  
  public void setWalk(boolean doWalk){
    Properties props = new Properties();
    if(doWalk) props.setProperty(ARG_WALK, TRUE);
    else props.setProperty(ARG_WALK, FALSE);
    
    client.sendMessage(SETWALK,props);
  }
  
  /**
   * <p>
   * Say something that is heard by other players.
   * </p>
   * @param message The message to send
   * @param forEveryone True if this is sent to both teams, false if only to 
   * your team
   */
  public void say(String message, boolean forEveryone){
    Properties props = new Properties();
    if(forEveryone) props.setProperty(ARG_GLOBAL, TRUE);
    else props.setProperty(ARG_GLOBAL, FALSE);
    props.setProperty(ARG_TEXT, message);
    
    client.sendMessage(MESSAGE,props);
  }
  
  /**
   * <p>
   * Stop moving. If the agent is en route to a location (via the runTo) command
   * or is otherwise moving and can stop, the agent will stop.
   * </p>
   */  
  public void stop(){
    client.sendMessage(STOP,null);
  }
  
  /**
   * <p>
   * The agent jumps. I think that it is a bug that the bot is able to jump again
   * while in the middle of a jump. In a room with high celings this gives bots
   * magical flying ability.
   * </p>
   */  
public void jump(){
    client.sendMessage(JUMP,null);
  }
  
  /**
   * <p>
   * Strafe. This causes the agent to run to a particular location while looking at
   * another.
   * </p>
   * @param x Global x coordinate run to.
   * @param y Global y coordinate run to.
   * @param z Global z coordinate run to.
   * @param faceObjectID Object ID to face while running
   */  
  public void strafe(double x, double y, double z, String faceObjectID){
    Properties props = new Properties();
    props.setProperty(LOCATION, x+" "+y+" "+z);
    props.setProperty(ARG_TARGET, faceObjectID);
    client.sendMessage(STRAFE,props);
  }
  
  /**
   * <P>
   * Rotate the agent by a particular amount. A full rotation using UT's measurements
   * is 65535. To convert the values you are sent to radiens, divide by 65535 and multiply by 2 * pi.
   * </P>
   * @param amount Amount in UT units to rotate
   */  
  public void rotate(double amount){
    Properties props = new Properties();
    props.setProperty(ARG_AMOUNT, Double.toString(amount));
    client.sendMessage(ROTATE,props);
  }

  /**
   * <p>
   * Agent shoots their current weapon (if they have one). Sometimes the agent will
   * fire one or two shots and sometimes they fire more.
   * </p>
   */  
  public void shoot(){
    client.sendMessage(SHOOT,null);
  }
  
  /**
   * <p>
   * Shoot at a particular target. The bot will auto aim. As long as the target
   * stays within the bots visual range, the bot will keep shooting the target until
   * it runs out of ammo. Use the {{@link #stopShoot() stopShoot} command to stop shooting.
   * </p>
   * @param x Global x coordinate to fire toward
   * @param y Global y coordinate to fire toward
   * @param z Global z coordinate to fire toward
   * @param targetID Global UT target ID to fire at
   * @param altFire <CODE>true</CODE> to use the weapon's alternate fire feature.
   */  
  public void shoot(double x, double y, double z, String targetID, boolean altFire){
    Properties props = new Properties();
    props.setProperty(LOCATION, x+" "+y+" "+z);
    props.setProperty(ARG_TARGET, targetID);
    if(altFire) props.setProperty(ARG_ALT, TRUE);
    else props.setProperty(ARG_ALT, FALSE);
    client.sendMessage(SHOOT,props);
  }
  
  /**
   * <p>
   * Shoot at a particular target. The bot will auto aim. As long as the target
   * stays within the bots visual range, the bot will keep shooting the target until
   * it runs out of ammo. Use the {@link #stopShoot() stopShoot} command to stop shooting.
   * </p>
   * @param targetID The UT id of the target to shoot at
   * @param altFire <CODE>true</CODE> to use the weapon's alternate fire feature.
   */  
  public void shoot(String targetID, boolean altFire){
    Properties props = new Properties();
    props.setProperty(ARG_TARGET, targetID);
    if(altFire) props.setProperty(ARG_ALT, TRUE);
    else props.setProperty(ARG_ALT, FALSE);
    client.sendMessage(SHOOT,props);
  }
  
  /**
   * <p>
   * Changes to another weapon that the agent posseses.
   * </p>
   * @param weaponID The UT id of the weapon to switch to
   */  
  public void changeWeapon(String weaponID){
    Properties props = new Properties();
    props.setProperty(ACTOR_ID, weaponID);
    client.sendMessage(CHANGEWEAPON, props);
  }
  
  /**
   * <p>
   * If currently shooting its weapon, the bot will stop.
   * </p>
   */  
  public void stopShoot(){
    client.sendMessage(STOP_SHOOT, null);
  }
  
  //  Private Methods
  ///////////////////////////////////////////////////////////////////////////
  
  /**
   * <p>
   * Sets the bot's name.  Currently does nothing if the Bot is already
   * connected.
   *
   * <p>Future: request name changes to the server when connected.
   * @param newName Agent name
   */
  protected void setName( String newName ) {
    if( newName != null &&
    newName != name ) {
      if( client.isConnected() ) {
        // TO-DO: Send a message to the server...
      } else {
        name = newName;
      }
    }
  }
  
  // Event Handlers
  /**
   * <p>
   * Handles server message events
   * </p>
   */  
  protected GamebotsClient.Listener clientListener = new GamebotsClient.Listener() {
    //  Connection messages do not go through the event loop
    public void connected() {
      Bot.this.connected();
    }
    
    public void receivedAsyncMessage( Message message ) {
      //      log.logNote( message );
      //      log.flush();
      synchronized( eventQueue ) {
        eventQueue.add( new MessagePair( ASYNC_MESSAGE, message ) );
        eventQueue.notifyAll();
      }
    }
    
    public void receivedSyncMessage( MessageBlock block ) {
      //      log.logNote( block );
      //      log.flush();
      synchronized( eventQueue ) {
        eventQueue.add( new MessagePair( SYNC_MESSAGE, block ) );
        eventQueue.notifyAll();
      }
    }
    
    public void receivedError( Throwable error ) {
      //      log.logNote( error );
      //      log.flush();
      synchronized( eventQueue ) {
        eventQueue.add( new MessagePair( ERROR_MESSAGE, error ) );
        eventQueue.notifyAll();
      }
    }
    
    //  Disconnection messages do not go through the event loop
    public void disconnected() {
      Bot.this.disconnected();
    }
  };
  
  /** <p>
   * Processes events as they arrive
   * </p>
   */  
  protected Runnable eventProcessor = new Runnable() {
    public void run() {
      MessagePair pair;
      while( isAlive ) {
        try {
          synchronized( eventQueue ) {
            while( true ) {
              while( eventQueue.isEmpty() )
                eventQueue.wait();
              pair = (MessagePair) eventQueue.firstElement();
              eventQueue.remove( pair );
              try {
                switch( pair.type ) {
                  case ASYNC_MESSAGE:
                    Bot.this.receivedAsyncMessage( (Message) pair.message );
                    break;
                  case SYNC_MESSAGE:
                    Bot.this.receivedSyncMessage( (MessageBlock) pair.message );
                    break;
                  case ERROR_MESSAGE:
                    Bot.this.receivedError( (Throwable) pair.message );
                    break;
                }
              } catch( RuntimeException error ) {
                Bot.this.receivedError( error );
              }
            }  //  end message loop
          }  //  end sync
        } catch( InterruptedException error ) {
          // breaking out of synchronized loop...
        }
      }  //  end alive loop
    }
  };
  
  /**
   * <p>
   * Bot implementations override this to specify what to do (if anything) when the
   * bot is initialized.
   * </p>
   */  
  protected void init() {
    // Bot implementatiosn don't _need_ to do anything.
  }
  
  /**
   * <p>
   * Bot implementations override this to specify what to do (if anything) after the
   * bot established a server connection.
   * </p>
   */  
  protected void connected() {
    // Bot implementatiosn don't _need_ to do anything.
  }
  
  /**
   * <P>
   * Bot implementations must override this method. This specifies what to do when an
   * asynchronous message is recieved. A description of the difference between sync
   * and async messages can be found at <a href="http://www.planetunreal.com/gamebots/docapi.html">
   * http://www.planetunreal.com/gamebots/docapi.html</a>. Although this is not
   * nessesarily a complete reference, it is a good general intro.
   * </P>
   * @param message The <CODE>Message</CODE> object to handle
   */  
  protected abstract void receivedAsyncMessage( Message message );
  
  /**
   * <P>
   * Bot implementations must override this method. This specifies what to do when an
   * synchronous message is recieved. A description of the difference between sync
   * and async messages can be found at <a href="http://www.planetunreal.com/gamebots/docapi.html">
   * http://www.planetunreal.com/gamebots/docapi.html</a>. Although this is not
   * nessesarily a complete reference, it is a good general intro.
   * </P>
   * @param message The synchronous <CODE>MessageBlock</CODE> to handle
   */  
  protected abstract void receivedSyncMessage( MessageBlock message );
  
  /**
   * <p>
   * Event handling. What to do when a bot throws an error. Default is to disconnect.
   * </p>
   * @param error The error that was recieved to handle.
   */  
  protected final void receivedError( Throwable error ) {
    log.logError( "Bot \""+name+"\": "+error, error );
    disconnect();
  }
  
  /**
   * <p>
   * Event handling. Bot implementation overrides this to tell what to do when
   * disconnected from the server.
   * </p>
   */  
  protected void disconnected() {
    // Bot implementatiosn don't _need_ to do anything.
  }
  
  /**
   * <p>
   * Event handling. Describes what to do when the bot is destroyed.
   * </p>
   */  
  protected void destroyImpl() {
    // Bot implementatiosn don't _need_ to do anything.
  }
  
  /**
   * <p>
   * check to see if you can move directly to a destination without hitting an obstruction, falling in a pit, etc... 
   * </p>
   *
   * @param id message id made up by you and echoed in respose so you can match up response with querry 
   * @param target the unique id of a player/object/nav point/whatever. Must be visible.
   */
  public void checkReach(final java.lang.String id, final java.lang.String target) {
    Properties props = new Properties();
    props.setProperty( "Id", id);
    props.setProperty( "Target", target);
    client.sendMessage( "CHECKREACH", props );
  }  
  
  /**
   * <p>
   * check to see if you can move directly to a destination without hitting an obstruction, falling in a pit, etc... 
   * </p>
   *
   * @param id message id made up by you and echoed in respose so you can match up response with querry 
   * @param fromX Location you want to go to. Normal location rules.
   * @param fromY Location you want to go to. Normal location rules.
   * @param fromZ Location you want to go to. Normal location rules.
   */
  public void checkReach(final java.lang.String id, double fromX, double fromY, double fromZ) {
    Properties props = new Properties();
    props.setProperty( "Id", id);
    props.setProperty( "Location", fromX + " " + fromY + " " + fromZ);
    client.sendMessage( "CHECKREACH", props );
  }
  
  //  Private Inner Classes
  ///////////////////////////////////////////////////////////////////////////
  private class MessagePair {
    /**
     * <p>
     * Type of message recieved. ERROR, SYNC, or ASYNC
     * </p>
     */    
    public final int type;
    /** <p>
     * The actual message.
     * <p>
     */    
    public final Object message;
    
    /**
     * <p>
     * Constructs a new MessagePair object.
     * </p>
     * @param type The type of message
     * @param message The message object
     */    
    public MessagePair( int type, Object message ) {
      this.type = type;
      this.message = message;
    }
  }
}
