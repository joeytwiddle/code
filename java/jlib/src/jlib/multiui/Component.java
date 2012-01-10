package jlib.multiui;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import jlib.*;


public class Component {

	public static final String Delim=".";
	public AppServer appServer;

	public String ref=/*this.getClass().getName()+"#"+*/JString.random(6); // this.toString();
  
  private boolean visible=true;

	// public Component() {
		// super();
		// // init();
	// }

	public void init() { }

  public void setVisible(boolean b) {
    visible=b;
  }

  public boolean isVisible() {
    return visible;
  }

  public String renderHTML() {
    return getClass()+".renderHTML(): not yet implemented<br>\n";
  }

}
