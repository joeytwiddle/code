package jlib.multiui;

import java.lang.*;
import java.lang.reflect.*;
import java.net.*;
import java.util.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import jlib.*;
import nuju.*;

public class Container extends Component {

  public List comps=new Vector();
	
	// Without Constructor way makes it easier to overwrite
  // public Container() {
    // comps=new Vector();
		// init();
  // }
	
	public void clear() {
		comps.clear();
	}

  public void add(Component c) {
    comps.add(c);
  }

  public void println(String s) {
    add(new Text(s+"\n"));
  }
	public void print(String s) {
		add(new Text(s));
	}
	public void nl() { /* Requires optimisation! */
		println("");
	}

	public Component getComponent(String ref) {
		for (int i=0;i<comps.size();i++) {
			Component c=(Component)comps.get(i);
			if (c.ref.equals(ref))
				return c;
			if (c instanceof Container) {
				Component inner=((Container)c).getComponent(ref);
				if (inner!=null)
					return inner;
			}
		}
		return null;
	}

	public String toString() {
		return getClass().getName()+" containing ["+comps+"]";
	}

	public String renderHTML() { // Standard containers have pretty border
		StringBuffer s=new StringBuffer();
		s.append("<table width=\"100%\" cellpadding=\"0\" bgcolor=\"#000000\">");
		s.append("<tr><td>");
		s.append("<table width=\"100%\" bgcolor=\"#ffffff\">");
		s.append("<tr><td>");
		for (int i=0;i<comps.size();i++) {
      Component c=(Component)comps.get(i);
      s.append(c.renderHTML());
      // s.append("<br>");
    }
		s.append("</td></tr>");
		s.append("</table>");
		s.append("</td></tr>");
		s.append("</table>");
		return s.toString();
	}

}
