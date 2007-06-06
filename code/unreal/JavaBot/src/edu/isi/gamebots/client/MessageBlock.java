//  edu.isi.gamebots.clients.MessageBlock
//  Copyright 2000, University of Southern California,
//                  Information Science Institute
//
//  Personal and Educational use is hereby granted.
//  Permission required for commercial use and redistribution.


package edu.isi.gamebots.client;

import java.lang.*;
import java.util.*;

import edu.isi.gamebots.client.*;


/**
 *  @author <a href="mailto:amarshal#gamebots@isi.edu">Andrew n marshall</a>
 */
public class MessageBlock extends Message {
  protected List messages; // of Messages
  
  MessageBlock( GamebotsClient client,
  String type,
  Properties props,
  List messages,
  boolean complete ) {
    super( client, type, props, complete );
    
    this.messages = messages;
  }
  
  public Iterator getMessages() {
    return messages.iterator();
  }
  
  // added by Ryan Rozich
  // returns a new message block filtered to only contain the messages of
  // node-type object senses
  public MessageBlock onlyNodeMessages(){
    MessageBlock newBlock = new MessageBlock((GamebotsClient)source, type, props, messages, complete);
    // filter out all sync messages that are not visible nodes or objects
    for(int i=0; i<newBlock.messages.size();){
      Message msg = (Message)newBlock.messages.get(i);
      if(msg.getType().equalsIgnoreCase(GamebotsConstants.BEG) || msg.getType().equalsIgnoreCase(GamebotsConstants.END) || msg.getType().equalsIgnoreCase(GamebotsConstants.SELF) || msg.getType().equalsIgnoreCase(GamebotsConstants.GAMESTATE)) newBlock.messages.remove(i);
      else i++;
    }
    return newBlock;
  }
  
  public String getMessageString( StringBuffer sb, int indent ) {
    return super.toString( sb, indent ).toString();
  }
  
  // added by Ryan Rozich
  public Message getMessageAt(int index){
    try{
      return (Message)messages.get(index);
    }
    catch(IndexOutOfBoundsException e){
      return null;
    }
  }
  
  // added by Ryan Rozich
  public int getNumMessages(){
    return messages.size();
  }
  
  public StringBuffer toString( StringBuffer sb, int indent ) {
    super.toString( sb, indent );
    indent += 2;
    
    Iterator i = getMessages();
    while( i.hasNext() ) {
      sb.append( '\n' );
      ((Message) i.next()).toString( sb, indent );
    }
    
    return sb;
  }
}
