/*
 * UnrealMessageTextArea.java
 *
 * Created on February 19, 2002, 12:44 AM
 */

package edu.tamu.gamebots.ui;

import edu.isi.gamebots.client.*;
import javax.swing.*;
import javax.swing.text.*;
import java.awt.*;

/**
 * <p>
 * Prints messages from the Gamebots server for the bot to the TextPanel. Time
 * messages are printed in red to make it easy to visually parse message blocks.
 * Messages from the server come very rapidly (10 full updates/second) so this has
 * the ability to be paused so as not to overwhelm system resources.
 * </p>
 * @author Ryan Rozich (rozich@tamu.edu)
 */
public class UnrealMessageTextArea extends javax.swing.JTextPane {
  
  private StyledDocument doc;
  private boolean paused = false;
  
  /**
   * <p>
   * Creates a new instance of UnrealMessageTextArea
   * </p>
   */
  public UnrealMessageTextArea() {
    super();
    doc = getStyledDocument();
  }
  
  /**
   * <p>
   * Add a message to this TextPane
   * </p>
   * @param msg The message to print
   */  
  public void addMessage(Message msg){
    if(!paused){
      SimpleAttributeSet attributes = new SimpleAttributeSet();
      
      // make beg and end statements red
      if(msg.getType().equalsIgnoreCase(GamebotsConstants.BEG) || msg.getType().equalsIgnoreCase(GamebotsConstants.END)){
        StyleConstants.setBold(attributes,true);
        attributes.addAttribute(StyleConstants.CharacterConstants.Foreground, Color.red);
      }
      
      //insert message
      try{
final int maxLength = 1024 * 4;
        doc.insertString(0,msg + "\n", attributes);
          if (doc.getLength()>maxLength) {
              doc.remove(maxLength,doc.getLength()-maxLength);
          }
//        doc.insertString(doc.getLength(),msg + "\n", attributes);
//          if (doc.getLength() > maxLength) {
//          doc.remove(0,doc.getLength() - maxLength);
//          }
      }
      catch(BadLocationException e){
        e.printStackTrace();
        System.exit(1);
      }
    }
  }
  
  /**
   * <p>
   * Pause the updates of this TextPane
   * </p>
   */  
  public void pause(){ paused = true; }
  /** 
   * <p>
   * If this is currently paused, it will start to print messages again
   * </p>
   */  
  public void unpause(){ paused = false; }
  /**
   * <p>
   * Toggles between paused and unpaused state. In other words, if currently
   * not-paused, it will pause and visa-versa.
   * </p>
   */  
  public void togglePause(){ paused = !paused; }
  
}
