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

public class YesNoChoice extends Component implements UserEditableComponent {
		
	private static final String onVal="appears to be selected";
	public static final String offVal="*missing*";

	String description;
	boolean state;

  public YesNoChoice(String d,boolean c) {
		description=d;
		state=c;
  }

	public void setBoolean(boolean b) { // overwrite this chap - state is updated automaticall outside here
		
	}

	public String renderHTML() {
		StringBuffer s=new StringBuffer();
		s.append(JString.toHTML(description+"? ")); // Should be done by adding a text component (though init()?)
		s.append("<input type=\"hidden\" name=\"expectvalue"+Delim+ref+"\" value=\"\">\n");
		s.append("<input type=\"checkbox\" "+ (state ? "checked " : "") +"name=\"value"+Delim+ref+"\" value=\""+onVal+"\">\n");
		s.append("<br>\n");
		return s.toString();
	}

	public void setValue(String s) {
		Log.report("Setting "+this+" to \""+s+"\"");
		if (s.equals(onVal)) {
			state=true;
			setBoolean(true);
		} else {
			state=false;
			setBoolean(false);
		}
	}

	public String getValue() {
		return ""+state;
	}

}
