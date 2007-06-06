/*
 * UnrealBot.java
 *
 * Created on February 19, 2002, 12:38 AM
 */

package edu.tamu.gamebots.humanbot;

import edu.isi.gamebots.client.*;
import edu.tamu.gamebots.ui.*;
import java.util.*;

/**
 * <p>
 * A user interface that allows users to view the messages sent to the bot and to
 * and send messages (actions) to the server.
 * </p>
 * @author rtr7684
 */
public class HumanBot extends edu.isi.gamebots.client.Bot{
  
  UnrealMessageTextArea textArea;
  UnrealMessageTextArea asyncMessageArea;
  CurrentBotStatePanel botState = null;
  /**
   * <p>
   * The table that will hold the parsed visual infromation
   * </p>
   */  
  protected javax.swing.JTable table = null;
  /**
   * <p>
   * Flag to update the table during the next synchronous block
   * </p>
   */  
  protected boolean updateTable = true;
  
  /**
   * <p>
   * Creates a new instance of HumanBot
   * </p>
   */
  public HumanBot() {
    super();
    textArea = new UnrealMessageTextArea();
    
  }
  
  /**
   * <p>
   * Creates a new HumanBot.
   * </p>
   * @param syncArea The TextPanel that will hold the synchronous messages
   * @param asyncArea The TextPanel that will hold the asynchronous messages
   */  
  public HumanBot(UnrealMessageTextArea syncArea, UnrealMessageTextArea asyncArea){
    super();
    textArea = syncArea;
    asyncMessageArea = asyncArea;
  }
  
  /**
   * <p>
   * Sets the CurrentBotStatePanel for this bot. This shows the current stats for the
   * bot
   * </p>
   * @param statePanel The CurrentBotStatePanel for this bot
   */  
  public void setCurrentBotStatePanel(CurrentBotStatePanel statePanel){
    botState = statePanel;
  }
  
  /**
   * <p>
   * Sets the UnrealMessageTextArea for the bot where the synchronous messages will
   * be displayed.
   * </p>
   * @param syncArea The UnrealMessageTextArea for the bot where the synchronous messages will be displayed.
   */  
  public void setSyncMessagesTextArea(UnrealMessageTextArea syncArea){
    textArea = syncArea;
  }
  
  /**
   * <p>
   * Sets the UnrealMessageTextArea for the bot where the asynchronous messages will
   * be displayed.
   * </p>
   * @param asyncArea The UnrealMessageTextArea for the bot where the asynchronous messages will be displayed.
   */  
  public void setASyncMessagesTextArea(UnrealMessageTextArea asyncArea){
    asyncMessageArea = asyncArea;
  }
  
  /**
   * <p>
   * Set the table that will hold the parsed visual node information from sync messages.
   * </p>
   * @param msgTable <CODE>JTable</CODE> that holds the information.
   */  
  public void addJTableForMessages(javax.swing.JTable msgTable){
    table = msgTable;
  }
  
  /**
   * <p>
   * If it is an NFO message, initializes the bot with the world. Otherwise, it prints the message to the async message text panel.
   * </p>
   * @param message The ASynchronous message to handle.
   */   
  protected void receivedAsyncMessage(Message message) {
    // If you get an NFO message initialize
    if( message.getType().equals( INFO ) ) {
      // Init
      //  Should check to make sure it is only the first...
      Properties props = new Properties();
      props.setProperty( client.PLAYER_NAME, getName() );
      int team = getInitialTeam();
      if( team != TEAM_ANY )
        props.setProperty( client.PLAYER_TEAM, Integer.toString(team) );
      client.sendMessage( client.INIT, props );
    }
    
    // Add the message to the message area
    asyncMessageArea.addMessage(message);
  }
  
  /**
   * <p>
   * Flag the table to be updated on the next Synch. message block.
   * </p>
   */  
  protected void setUpdateTable(){
    updateTable = true;
  }
  
  /**
   * <p>
   * Returns a {@link #edu.tamu.gamebots.humanbot.HumanBotPanel BotPanel} for display. Called by {@link #edu.isi.gamebots.client.BotRunnerApp BotRunnerApp}
   * </p>
   * @return The <CODE>HumanBotPanel</CODE> of this bot
   */  
  public javax.swing.JComponent getView(){
    return new HumanBotPanel(this);
  }
  
  // Event Handlers
   /**
    * <p>
    * Sends a message to the interface saying that the bot was connected. This is an event handling method in Bot.
    * </p>
    */   
  protected void connected() {
    super.connected();    
    log.logNote( "Connected... ("+new Date()+")" );
  }
  
  /**
   * <p>
   * Sends a message to the interface saying that the bot was disconnected. This is an event handling method in Bot.
   * </p>
   */  
  protected void disconnected() {
    log.logNote( "Disconnected... ("+new Date()+")" );
  }
  
  /**
   * <p>
   * Updates the interface when a new sync. message is received.
   * </p>
   * @param message The <CODE>Message</CODE> object to handle
   */  
  protected void receivedSyncMessage(MessageBlock message) {
    // update JTable
    if(table != null && updateTable){
      table.setModel(new SyncMsgTableModel(message));
      updateTable = false;
    }
    
    // update text areal with all messages
    Iterator it = message.getMessages();
    while(it.hasNext()){
      Message next = (Message)it.next();
      textArea.addMessage(next);
      if(next.getType().equalsIgnoreCase(SELF)) botState.update(next);
    }
  }
  
  /**
   * <p>
   * Sends all messages sent to the server to the debug log. If using
   * BotRunnerApp, then this is sent to the lower half of the split pane
   * in the display
   * </p>
   */
  public void init() {
    client.setLog(log);
    client.setLogOutput(true);
  }
  
}
