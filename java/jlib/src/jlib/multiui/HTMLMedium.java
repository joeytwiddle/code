package jlib.multiui;

import java.lang.*;
import java.lang.reflect.*;
import java.net.*;
import java.util.*;
import java.io.*;
import javax.swing.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import jlib.*;
import nuju.*;

public class HTMLMedium {

  public void renderPage(Page p,PrintWriter out) {
		// out.println("<form action=\"\">");
    List comps=p.comps;
    for (int i=0;i<comps.size();i++) {
      Component c=(Component)comps.get(i);
      out.print(c.renderHTML());
    }
		// out.println("</form>");
  }

}
