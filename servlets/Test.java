// package generation;

import jlib.JString;

import java.lang.String;
import javax.servlet.*;
import javax.servlet.http.*;
import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Random;
import java.io.*;
import java.util.Vector;
import java.util.*;
import java.text.SimpleDateFormat;
import java.lang.reflect.*;
import java.util.Hashtable;

// import changetheworld.Files;
// import jlib.JString;
// import changetheworld.XML;
// import changetheworld.CTW;

public class Test extends HttpServlet {

  HttpServletRequest req;
  HttpServletResponse res;
  PrintWriter out;

  public void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
    req=request;
    res=response;

    // Set the Content-Type header
    res.setContentType("text/html");
    // Return early if this is a HEAD
    if (req.getMethod().equals("HEAD"))
      return;

    // Proceed otherwise
    out = res.getWriter();

    try {

    out.println("Hello world =)\n");
    
    out.println("<p>\n");

    Enumeration e=req.getHeaderNames();
    if (e==null) {
      out.println("null header Enumeration!<br>\n");
    } else {
      out.println("Headers:<br>\n");
      out.println("<blockquote>\n");
      while (e.hasMoreElements()) {
        try {
          String name=(String)e.nextElement();
          String value=req.getHeader(name);
          out.println(""+name+": "+value+"<br>\n");
        } catch (Exception ex) {
          out.println(""+ex+"<br>\n");
        }
      }
      out.println("</blockquote>\n");
    }

    out.println("<p>\n");

    out.println("Query string parameters:<br>\n");
    out.println("<blockquote>\n");
    e=req.getParameterNames();
    int i=1;
    while (e.hasMoreElements()) {
      String s=(String)e.nextElement();
      String val=req.getParameter(s);
      out.println(i+": "+s+"="+val+"<br>\n");
      i++;
    }
    out.println("</blockquote>\n");

    // Object o=req.getParameter("filename");
    // out.println(""+o);

    } catch (Exception ex) {
      out.println(""+ex);
    }

  }
  
  public void doPost(HttpServletRequest req, HttpServletResponse res) throws ServletException, IOException {
    doGet(req, res);
  }

}
