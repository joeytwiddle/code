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

public class YesNoChoice extends CheckBox implements UserEditableComponent {
		
	String description;

  public YesNoChoice(String d,boolean c) {
		super(c);
		description=d;
  }

	public String renderHTML() {
		StringBuffer s=new StringBuffer();
		s.append(JString.toHTML(description+"? ")); // Should be done by adding a text component (though init()?)
		s.append(super.renderHTML());
		return s.toString();
	}

}
