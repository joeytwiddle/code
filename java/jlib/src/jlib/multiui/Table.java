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


public class Table extends Container {

	public int width,height;
	public Component[][] tab;

	public Table(int w,int h) {
		width=w;
		height=h;
		tab=new Component[width][height];
		// for (int i=0;i<width;i++)
		// for (int j=0;j<height;j++)
		  // tab[i][j]=null;
	}

	public void set(int i,int j,Component c) {
		tab[i][j]=c;
	}

	public Component get(int i,int j) {
		return tab[i][j];
	}

	public Component getComponent(String ref) {
		for (int i=0;i<width;i++)
		for (int j=0;j<height;j++) {
			Component c=get(i,j);
			if (c!=null)
				if (c.ref.equals(ref))
					return c;
		}
		return null;
	}

	public String renderHTML() {
		StringBuffer s=new StringBuffer();
		s.append("<table width=\"100%\">");
		for (int j=0;j<height;j++) {
			s.append("<tr>");
		  for (int i=0;i<width;i++) {
			  s.append("<td valign=\"middle\">");
				Component c=tab[i][j];
				if (c!=null)
					s.append(c.renderHTML());
				s.append("</td>");
			}
			s.append("</tr>");
		}
		s.append("</table>");
		return s.toString();
	}

  
}
