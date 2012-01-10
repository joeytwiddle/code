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


public class TextField extends Component implements UserEditableComponent {

	public String name=null;
  public String content;

  public TextField(String t) {
    content=t;
  }

  public TextField(String n,String t) {
		name=n;
    content=t;
  }

	public String renderHTML() {
		return "<input type=\"text\" name=\"value"+Delim+ref+"\" value=\""+JString.toHTML(content)+"\">\n";
	}

	public void setValue(String s) {
		content=s;
	}

	public String getValue() {
		return content;
	}

}
