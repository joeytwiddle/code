package generation;

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

import jlib.Log;
import jlib.db.*;

public class Servlet extends HttpServlet {

  private static final String baseURL="/generation/";

  public void doGet(HttpServletRequest req, HttpServletResponse res) throws ServletException, IOException {

    // Set the Content-Type header
    res.setContentType("text/html");
    // Return early if this is a HEAD
    if (req.getMethod().equals("HEAD"))
      return;

    // Proceed otherwise
    PrintWriter out = res.getWriter();

    try {

      String origURL=""+req.getParameter("origURL");
      String origURLdiff=origURL.substring(baseURL.length());
      String origURLstart=JString.before(origURLdiff,"/");
      String origURLrest=JString.after(origURLdiff,"/");
      
      if (origURLstart.equals("show")) {
        out.println("Showing: &gt;&gt;"+origURLrest+"&lt;&lt;<br>");
        DBKey k=new SimpleDBKey(JString.before(origURLrest,"/"),JString.after(origURLrest,"/"));
        DB db=new DB();
        DBObj o=db.get(k);
        out.println("Got object: "+o+"<br>");
      }

    } catch (Exception ex) {
      out.println(""+ex);
    }
    
    // out.println(Log.logHtml);
    out.println(Log.HTMLlogiferror());

  }
  
  public void doPost(HttpServletRequest req, HttpServletResponse res) throws ServletException, IOException {
    doGet(req, res);
  }
  
}
