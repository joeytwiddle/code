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

public class Button extends Component implements ActionComponent {

  public String text;
  //private Page destination;
	public Object carry;

	public Button() { }

  public Button(String t) {
    text=t;
  }

  public Button(String t,Object c) {
    text=t;
		carry=c;
  }

	public void action() { }

	public String renderHTML() {
		return "<input type=\"submit\" name=\"action"+Delim+ref+"\" value=\""+text+"\">";
	}

}
