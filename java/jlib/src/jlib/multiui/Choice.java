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

public class Choice extends Component implements UserEditableComponent {

  public String[] choices;
	int choice=0;

  public Choice(String[] cs) {
    choices=cs;
  }

  public Choice(String[] cs,int i) {
    choices=cs;
		choice=i;
  }

  public Choice(String[] cs,String s) {
    choices=cs;
		choice=JVector.toList(cs).indexOf(s);
  }

	public void onChange() { // Overwrite this one
	}

	public String renderHTML() {
		StringBuffer s=new StringBuffer();
		s.append("<select name=\"value"+Delim+ref+"\">\n");
		for (int i=0;i<choices.length;i++)
			s.append("  <option value=\""+i+"\""+ (i==choice ? " selected" : "") +">"+JString.toHTML(choices[i])+"</option>\n");
		s.append("</select>");
		return s.toString();
		// return "<input type=\"text\" name=\"value"+Delim+ref+"\" value=\""+JString.toHTML(content)+"\">\n";
	}

	public void setValue(String s) {
		// choice=-1;
		// for (int i=0;i<choices.length;i++)
			// if (choices[i].equals(s))
				// choice=i;
		// if (choice==-1)
		// int i=new Integer(s).intValue();
		int i=Integer.parseInt(s);
		if (i<0 || i>choices.length)
			Log.error("Setting choice \""+s+"\" on "+this+": out of range.");
		onChange();
	}

	public String getValue() {
		return choices[choice];
	}

	public int getIndex() {
		return choice;
	}

}
