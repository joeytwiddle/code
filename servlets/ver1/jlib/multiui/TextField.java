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

public class TextField extends UserEditableComponent {

	public String name=null;
  public String content;

  public TextField(String t) {
    content=t;
  }

  public TextField(String t,String n) {
		name=n;
    content=t;
  }

	public String renderHTML() {
		return "<input type=\"text\" value=\""+JString.toHTML(content)+"\">\n";
	}

}
