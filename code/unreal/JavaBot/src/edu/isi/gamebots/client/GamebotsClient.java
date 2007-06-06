//  edu.isi.gamebots.clients.GamebotsClient
//  Copyright 2000, University of Southern California,
//                  Information Science Institute
//
//  Personal and Educational use is hereby granted.
//  Permission required for commercial use and redistribution.


package edu.isi.gamebots.client;

import java.lang.*;
import java.io.*;
import java.net.*;
import java.util.*;

import us.ca.la.anm.util.io.*;


/**
 *  This class is a JavaBean client for the Gamebots server.  It encapsulates
 *  the lowest level network connection and provides an EventListener interface
 *  {@link GamebotsClient.Listener} to handle incoming server messages.
 *
 *  @author <a href="mailto:amarshal#gamebots@isi.edu">Andrew n marshall</a>
 */

public class GamebotsClient implements GamebotsConstants {
  //  Private Static Data
  ///////////////////////////////////////////////////////////////////////////
  /**
   *  Length of the socket read buffer.  Used by {@link #parseMessage}.
   */
  protected final static int BUFFER_LENGTH = 256;
  protected final static int MAX_WAIT = 10000; // in milliseconds

  //  Private Data
  ///////////////////////////////////////////////////////////////////////////

  /**
   *  The client's logging mechanism.
   */
  private DebugLog log = PrintWriterLog.DEFAULT;
  /**
   *  Toggles whether sent messages are logged.
   *  Set by (@link #setLogOutput}. Used by {@link #sendMessage}.
   */
  private boolean logOutput = false;

  /**
   *  Gamebots server's internet address.
   *  Must be initialized via {@link #setServerAddress}.
   */
  private InetAddress serverAddress = null;
  /**
   *  Gamebots server's port number.
   *  Must be initialized via {@link #setServerPort}.
   */
  private int serverPort = DEFAULT_BOT_PORT;

  /**
   *  Synchronization lock for manipulating the connection.
   */
  protected final Object connectionLock = new Object();
  /**
   *  The network connection socket.  It remains null when not connected.
   */
  protected Socket socket = null;
  /**
   *  Thread used to handle incoming messages from the server.
   */
  protected Thread inputThread = null;

  /**
   *  Synchronization lock for manipulating the outgoing connection.
   */
  protected final Object outputLock = new Object();
  /**
   *  The outgoing wruter .  It remains null when not connected.
   */
  protected PrintWriter output = null;

  /**
   *  List of registered {@link GamebotsClient.Listener}s.
   */
  protected List listeners = new ArrayList();

  /**
   *  Time of the last received message.
   */
  protected long lastMessageTime = -1;

  /**
   *  A timer used to detect dropped connections.
   */
  protected Timer pingTimer = new Timer();

  /**
   *  The current PingTask on the {@link #pingTimer} detect dropped connections.
   */
  protected PingTask pingTask = null;
  

  //  Public Methods
  ///////////////////////////////////////////////////////////////////////////
  /**
   *  Sets the {@link DebugLog} used to log events.  If never set, events will
   *  be logged to {@link PrintWriterLog#DEFAULT}.
   */
  public void setLog( DebugLog log ) {
    this.log = log;
  }

  /**
   *  @return true if outgoing messages are being logged.
   */
  public boolean getLogOutput() {
    return logOutput;
  }

  /**
   *  Sets whether outgoing messages are logged.
   *
   *  @see #setLog
   */
  public void setLogOutput( boolean logOutput ) {
    this.logOutput = logOutput;
  }

  /**
   *  Gets the timestamp of the last message received from the server.  If the
   *  client is not connected or no message has been received, it returns -1.
   */
  public long getLastMessageReceivedTimeStamp() {
    return lastMessageTime;
  }

  /**
   *  Registers a {@link GamebotsClient.Listener} with this client.
   *  Does nothing if the listener is already registered.
   */
  public void addListener( Listener listener ) {
    if( listener == null )
      throw new IllegalArgumentException( "listener cannot be null" );
    if( !listeners.contains( listener ) )
      listeners.add( listener );
  }

  /**
   *  Unregisters {@link GamebotsClient.Listener listener} with this client.
   *  Does nothing if the listener is not currently registered.
   */
  public void removeListener( Listener listener ) {
    listeners.remove( listener );
  }

  /**
   *  @return the internet address of the Gamebots server.  If unset, it returns null.
   */
  public InetAddress getServerAddress() {
    return serverAddress;
  }

  /**
   *  Sets the internet address of the Gamebots server. If the client is already
   *  connected, the connection is broken before setting the server.
   *
   *  @throws IllegalArgumentException if serverAddress is null.
   */
  public void setServerAddress( InetAddress serverAddress ) {
    if( serverAddress == null )
      throw new IllegalArgumentException();
    if( serverAddress.equals( this.serverAddress ) )
      return;
    synchronized( connectionLock ) {
      if( isConnected() )
        disconnect();

      this.serverAddress = serverAddress;
    }
  }

  /**
   *  @return the network port of the Gamebots server.
   *  Defaults to {@link GamebotsConstants#DEFAULT_BOT_PORT}.
   */
  public int getServerPort() {
    return serverPort;
  }

  /**
   *  Sets the network port of the Gamebots server. If the client is already
   *  connected, the connection is broken before setting the server.
   *
   *  @throws IllegalStateException is client is currently connected.
   *  @throws IllegalArgumentException if port <= 0.
   */
  public void setServerPort( int port ) {
    if( port <= 0 )
      throw new IllegalArgumentException();
    if( port == this.serverPort )
      return;
    synchronized( connectionLock ) {
      if( isConnected() )
        disconnect();

      this.serverPort = port;
    }
  }

  /**
   *  @return whether the client is currently connected.
   */
  public boolean isConnected() {
    return inputThread != null;
  }

  /**
   *  Initiates a connection to the server.  If the client is already connected,
   *  it does nothing.
   *
   *  @throws IOException if socket cannot be created.
   *  @throws IllegalStateException if the server address is not set.
   */
  public void connect() throws IOException {
    try {
      synchronized( connectionLock ) {
        if( !isConnected() ) {
          if( serverAddress == null )
            throw new IllegalStateException( "Server address not set" );

          socket = new Socket( serverAddress, serverPort );
          inputThread = new Thread( new Runnable() {
            public void run() { handleSocket( socket ); } } );
          inputThread.start();

          synchronized( outputLock ) {
            output = new PrintWriter( socket.getOutputStream() );
          }
        }
      }  // end sync
      fireConnected();
    } catch( IOException error ) {
      log.logError( "Cannot connect to server "+serverAddress.toString()+":"+serverPort, error );
    }
  }
  
  /**
   * Sends a string message to the server
   */
  public boolean sendMessage(String msg){
    synchronized(outputLock){
      if(output == null) return false;
      output.println( msg );
      if( output.checkError() )
        disconnect();
    }
    if( logOutput )
      log.logNote( "SENT: "+msg );
    return true;
  }

  /**
   *  Sends a message to the server with the given type and properties.  May 
   *
   *  @throws IllegalArgumentException if type is null or empty.
   *
   *  @see #setLogOutput
   */
  public boolean sendMessage( String type, Properties properties ) {
    if( type == null )
      throw new IllegalArgumentException( "Message type is null" );
    if( type.equals("") )
      throw new IllegalArgumentException( "Message type is empty" );

    StringBuffer buffer;

    synchronized( outputLock ) {
      if( output == null )
        return false; // Not connected anymore....

      buffer = new StringBuffer( type );
      if( properties != null ) {
        java.util.Map.Entry entry;
        Iterator i = properties.entrySet().iterator();
        while( i.hasNext() ) {
          buffer.append( ' ' );
          entry = (java.util.Map.Entry) i.next();

          buffer.append( " {"+entry.getKey()+" "+entry.getValue()+"}" );
        }
      }

      output.write( buffer.toString() );
      output.write("\r\n");
      output.flush();
      if( output.checkError() )
        disconnect();

    } // end sync
    if( logOutput )
      log.logNote( "SENT: "+buffer );

    return true;
  }

  public void ping() {
    if( pingTask != null )
      return;   // ignore

    sendMessage( PING, null );
    pingTimer.schedule( pingTask = new PingTask(), MAX_WAIT );
  }

  /**
   *  Disconnects the client from the server.  If the client is not connected,
   *  it does nothing.  Any errors thrown by socket.close() are caught and
   *  logged.
   */
  public void disconnect() {
    synchronized( connectionLock ) {
      if( isConnected() ) {
        Socket oldSocket = socket;
        if( oldSocket != null ) {
          synchronized( outputLock ) {
            output = null;
          }
          Thread oldThread = inputThread;
          socket = null;
          long startWaitTime = System.currentTimeMillis();
          while( inputThread != null &&
                 (System.currentTimeMillis() - startWaitTime < MAX_WAIT ) ) { // 10 sec wait
            try {
              oldThread.interrupt();
              connectionLock.wait( 500 );
            } catch( InterruptedException error ) {
              // do nothing
            }
          }
          try {
            oldSocket.shutdownOutput();
          } catch( IOException error ) {
            log.logError( "Cannot close socket output", error );
          }
          try {
            oldSocket.close();
          } catch( IOException error ) {
            log.logError( "Cannot close socket", error );
          }
        }
      }
    }  // end sync

    fireDisconnected();
  }

  //  Private Methods
  ///////////////////////////////////////////////////////////////////////////

  /**
   *  Manages incoming network stream, and sends incoming Messages to
   *  registered {@link Listener}s.
   */
  protected void handleSocket( Socket socket ) {
    Message message;
    MessageBlock syncMessage;
    Iterator i;
    BufferedReader in = null;
    try {
      in = new BufferedReader( new InputStreamReader( socket.getInputStream() ) );

      MESSAGE_LOOP: while( socket == this.socket ) {
        message = parseMessage( in );
        if( message == null )
          break MESSAGE_LOOP;

        lastMessageTime = System.currentTimeMillis();
        if( pingTask != null ) {
          pingTask.cancel();
          pingTask = null;
        }

        i = listeners.iterator();
        if( message instanceof MessageBlock ) {
          syncMessage = (MessageBlock) message;
          while( i.hasNext() )
            ((Listener) i.next()).receivedSyncMessage( syncMessage );
        } else {
          while( i.hasNext() )
            ((Listener) i.next()).receivedAsyncMessage( message );

          //  Check to see if this is the last message
          if( message.getType().equalsIgnoreCase( FINISHED ) )
            break MESSAGE_LOOP;
        }
        Thread.currentThread().yield();
      }
    } catch( IOException error ) {
      fireReceivedSocketError( error );
    } catch( RuntimeException error ) {
      fireReceivedSocketError( error );
    } finally {
      try {
        if( in != null )
          in.close();
      } catch ( IOException error ) {
        // ignore...
      }
      try {
        socket.close();
      } catch ( IOException error ) {
        // ignore...
      }
    }

    synchronized( connectionLock ) {
      inputThread = null;

      connectionLock.notifyAll();
    }
  }

  /**
   *  Builds {@link Message} objects out of the PushbackReader in.
   *
   *  @throws IOException caused by in.read(...)
   */
  protected Message parseMessage( Reader in ) throws IOException {
    char[] buffer = new char[BUFFER_LENGTH];
    int length;
    StringBuffer type = new StringBuffer();
    StringBuffer name = new StringBuffer();
    StringBuffer value = new StringBuffer();
    Properties props = new Properties();

    int c = in.read();
    if( c == -1 )
      return null; // End of stream

    boolean complete = false;
    READ_BLOCK: {
      // Parse message type-name
      while( c != ' ' ) {
        if( c != '\r' && c != '\0' )
          type.append( (char) c );
        c = in.read();
        if( c == -1 )
          break READ_BLOCK; // End of stream, incomplete
      }

      // Parse name-value pairs in the message.
      c = in.read();
      if( c == -1 )
        break READ_BLOCK; // End of stream, incomplete
      while( c != '\n' ) {
        if( c == '{' ) {
          length = name.length();
          if( length > 0 )
            name.delete( 0, length );
          c = in.read();
          if( c == -1 )
            break READ_BLOCK; // End of stream, incomplete
          while( c != ' ' && c != '\n' ) {
            if( c != '\r' && c != '\0' )
              name.append( (char) c );
            c = in.read();
            if( c == -1 )
              break READ_BLOCK; // End of stream, incomplete
          }

          if( c != '\n' ) {
            c = in.read();
            if( c == -1 )
              break READ_BLOCK; // End of stream, incomplete
          }

          length = value.length();
          if( length > 0 )
            value.delete( 0, length );
          while( c != '}' && c != '\n' ) {
            if( c != '\r' && c != '\0' )
              value.append( (char) c );
            c = in.read();
            if( c == -1 )
              break READ_BLOCK; // End of stream, incomplete
          }

          props.setProperty( name.toString(), value.toString() );
        }

        c = in.read();
        if( c == -1 )
          break READ_BLOCK; // End of stream, incomplete
      }
      complete = true;
    } // end READ_BLOCK

    String typeStr = type.toString();
    Message message;
    if( typeStr.equals(BEG) ) {
      List messages = new LinkedList();
      message = parseMessage( in );
      MESSAGE_LOOP: {
        while( !message.getType().equals( END ) ) {
          messages.add( message );
          if( !message.isComplete() ) {
            complete = false;
            break MESSAGE_LOOP;
          }
          message = parseMessage( in );
          if( message == null ) {
            complete = false;
            break MESSAGE_LOOP;
          }
        }
        messages.add( message ); // END message
      }
      message = new MessageBlock( this, typeStr, props, messages, complete );
    } else {
      message = new Message( this, typeStr, props, complete );
    }

    return message;
  }

  protected void fireConnected() {
    final String methodName = "fireConected()";

    Iterator i = listeners.iterator();
    while( i.hasNext() ) {
      try {
        ((Listener) i.next()).connected();
      } catch( RuntimeException error ) {
        log.logError( "Error during "+methodName, error );
      }
    }
  }

  /**
   *  Passes given network error to all registered {@link Listener}s.
   */
  protected void fireReceivedSocketError( Throwable error ) {
    final String methodName = "fireReceivedSocketError(Throwable)";

    Iterator i = listeners.iterator();
    while( i.hasNext() ) {
      try {
        ((Listener) i.next()).receivedError( error );
      } catch( RuntimeException newError ) {
        log.logError( "Error during "+methodName, newError );
      }
    }
  }

  protected void fireDisconnected() {
    final String methodName = "fireDisconected()";

    Iterator i = listeners.iterator();
    while( i.hasNext() ) {
      try {
        ((Listener) i.next()).disconnected();
      } catch( RuntimeException error ) {
        log.logError( "Error during "+methodName, error );
      }
    }
  }

  //  Inner Classes
  ///////////////////////////////////////////////////////////////////////////

  /**
   *  Interface for listening to a {@link GamebotsClient}.
   */
  public static interface Listener extends EventListener {
    /**
     *  Notifies the Listener that the {@link GamebotsClient client} has been
     *  connected to the server.
     */
    public void connected();

    /**
     *  Notifies the Listener of a new asynchronous message.
     */
    public void receivedAsyncMessage( Message message );

    /**
     *  Notifies the Listener of a new synchronous message, also know as a
     *  vision block.
     */
    public void receivedSyncMessage( MessageBlock message );

    /**
     *  Notifies the Listener of any error that occured.
     */
    public void receivedError( Throwable error );

    /**
     *  Notifies the Listener that the {@link GamebotsClient client} has been
     *  disconnected from the server.
     */
    public void disconnected();
  }

  /**
   *  Simple implementation of a {@link GamebotsClient.Listener}.
   */
  public static class Adapter implements Listener {
    /**
     *  Notifies the Listener that the {@link GamebotsClient client} has been
     *  connected to the server.
     *
     *  @see GamebotsClient.Listener#connected
     */
    public void connected() {}

    /**
     *  Notifies the Listener of a new asynchronous message.
     *
     *  @see GamebotsClient.Listener#receivedAsyncMessage
     */
    public void receivedAsyncMessage( Message message ) {}

    /**
     *  Notifies the Listener of a new synchronous message, also know as a
     *  vision block.
     *
     *  @see GamebotsClient.Listener#receivedSyncMessage
     */
    public void receivedSyncMessage( MessageBlock message ) {}

    /**
     *  Notifies the Listener of any error that occured.
     *
     *  @see GamebotsClient.Listener#receivedError
     */
    public void receivedError( Throwable error ) {}

    /**
     *  Notifies the Listener that the {@link GamebotsClient client} has been
     *  disconnected from the server.
     *
     *  @see GamebotsClient.Listener#disconnected
     */
    public void disconnected() {}
  }

  protected class PingTask extends TimerTask {
    long sendTime = System.currentTimeMillis();

    public void run() {
      if( pingTask != this )
        return;  // ignore
      pingTask = null;
      fireReceivedSocketError( new IOException( "Server failed to respond to ping sent "+
                                                (((float) System.currentTimeMillis()-sendTime)/1000f )+
                                                " seconds ago" ) );
    }
  }
}
