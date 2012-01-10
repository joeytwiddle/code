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


public class HorizontalSplit extends Container {

	public String renderHTML() {
		StringBuffer s=new StringBuffer();
		s.append("<table width=\"100%\"");
		s.append("<tr>");
		for (int i=0;i<comps.size();i++) {
			Component c=(Component)comps.get(i);
			s.append("<td>");
			s.append(c.renderHTML());
			s.append("</td>");
		}
		s.append("</tr>");
		s.append("</table>");
		return s.toString();
	}

}
