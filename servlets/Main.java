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

import jlib.*;
import jlib.Log;
import jlib.db.*;
import jlib.multiui.*;

public class Main extends HttpServlet {

	static Session session=null;

  public static void main(String[] argv) {
    Map map=new HashMap();
    for (int i=0;i<argv.length;i++) {
      String s=argv[i];
      String[] ss=JString.split(s,"=");
      map.put(ss[0],ss[1]);
    }
    PrintWriter out=new PrintWriter(System.out);
    MainReal m=new MainReal(new Session(),map,out);
    out.println("1");
    m.go();
    out.flush(); // Important!
    System.out.println("2");
  }

  private Map getParameterMap(HttpServletRequest req) {
    Map m=new HashMap();
    Enumeration e=req.getParameterNames();
    while (e.hasMoreElements()) {
      String name=(String)e.nextElement();
      m.put(name,req.getParameter(name));
    }
    return m;
  }

  public void doGet(HttpServletRequest req, HttpServletResponse res) throws ServletException, IOException {

    // Set the Content-Type header
    res.setContentType("text/html");
    // Return early if this is a HEAD
    if (req.getMethod().equals("HEAD"))
      return;

    // Proceed otherwise
    // PrintWriter out = new HTMLPrintWriter(res.getWriter());
    PrintWriter out = new PrintWriter(res.getWriter());
		
    try {

      out.println("<html><body>\n");

      Map parameters=getParameterMap(req);

			// HttpSession s=req.getSession(true);
			// Object session=s.getValue("session");
			if (session==null) {
				session=new Session();
				// s.putValue("session",session);
				out.println("Welcome =)");
				JLib.report(this+": New session");
			} else {
				JLib.report(this+": Retrieved session "+session+" :");
				Session s=(Session)session;
				for (int i=0;i<s.pages.size();i++)
					JLib.report("  "+((Page)s.pages.get(i)).ref);
			}

      MainReal real=new MainReal((Session)session,parameters,out);
			real.go();

			JLib.report("--------------");

      /* String origURL=""+req.getParameter("origURL");
      String origURLdiff=origURL.substring(baseURL.length());
      String origURLstart=JString.before(origURLdiff,"/");
      String origURLrest=JString.after(origURLdiff,"/");
      
      if (origURLstart.equals("show")) {
        out.println("Showing: &gt;&gt;"+origURLrest+"&lt;&lt;<br>");
        DBKey k=new SimpleDBKey(JString.before(origURLrest,"/"),JString.after(origURLrest,"/"));
        DB db=new DB();
        DBObj o=db.get(k);
        out.println("Got object: "+o+"<br>");
      } */

    } catch (Exception ex) {
      out.println("Main(): "+ex);
			Log.error(this,ex);
    }
    
    // out.println(Log.logHtml);
    out.println(Log.HTMLlogiferror());

  }

  public void doPost(HttpServletRequest req, HttpServletResponse res) throws ServletException, IOException {
    doGet(req, res);
  }
  
}
