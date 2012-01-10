
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


public class CheckBox extends Component implements UserEditableComponent {
		
	private static final String onVal="appears to be selected";
	public static final String offVal="*missing*";

	public boolean state;

  public CheckBox(boolean c) {
		state=c;
  }

	public void setBoolean(boolean b) { // overwrite this chap - state is updated automaticall outside here
		
	}

	public String renderHTML() {
		StringBuffer s=new StringBuffer();
		s.append("<input type=\"hidden\" name=\"expectvalue"+Delim+ref+"\" value=\"\">\n");
		s.append("<input type=\"checkbox\" "+ (state ? "checked " : "") +"name=\"value"+Delim+ref+"\" value=\""+onVal+"\">\n");
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

	// public boolean getBool() {
		// return state;
	// }

}
