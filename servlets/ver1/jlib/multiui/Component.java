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
import nuju.*;

public class Component {
  
  private boolean visible=true;

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
