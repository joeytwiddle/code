package changetheworld; import jlib.JString;

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

import changetheworld.Files;
import jlib.JString;
import changetheworld.XML;
import changetheworld.CTW;

public class Test extends HttpServlet {

  HttpServletRequest req;
  HttpServletResponse res;
  PrintWriter out;

  public void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
    req=request;
    res=response;

    // Set the Content-Type header
//    res.setContentType("text/html");
    // Return early if this is a HEAD
    if (req.getMethod().equals("HEAD"))
      return;

    // Proceed otherwise
    out = res.getWriter();

    try {

    out.println("Hello<br>\n");

    Enumeration e=req.getParameterNames();
    int i=1;
    while (e.hasMoreElements()) {
      out.println(i+": "+e.nextElement());
      i++;
    }
//    out.println(""+q.keys());

    Object s=req.getParameter("blahblah");
    out.println(""+s);

    Object o=req.getParameter("filename");
    out.println(""+o);

    o=req.getParameter("file");
    out.println(""+o);

    o=req.getParameter("data");
    out.println(""+o);

    } catch (Exception ex) {
      out.println(""+ex);
    }

  }
  public void doPost(HttpServletRequest req, HttpServletResponse res) throws ServletException, IOException {
    doGet(req, res);
  }

}
