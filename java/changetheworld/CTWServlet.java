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

import changetheworld.Files;
import jlib.JString;
import changetheworld.XML;
import changetheworld.CTW;

import jlib.*;

public class CTWServlet extends HttpServlet {

//  public static String ServletPath="/home/httpd/servlets/"; // changetheworld/";
  public static String ServletURL=CTW.ServletUrl;
  ///*http://www.mycgiserver.com*/"/servlet/changetheworld.";
  public static String DataPath=CTW.DataPath;
  public static int SessionTimeout=60*30; // seconds

  public static String log="";

  HttpServletRequest req;
  HttpServletResponse res;
  PrintWriter out;

  // Data
  XML sessions=null;
  XML people=null;

  // This transaction
  String sessionid=null;
  Session session;
  Person person;

  public static boolean locked=false;

  public void lock(CTW ctw) {
    if (locked)
      ctw.out.println("Locked - waiting for unlock<br>");
    Date starttime=new Date();
    boolean timeout=false;
    while (locked && !timeout) {
      if ((new Date()).getTime()-starttime.getTime()>1000*20)
        timeout=true;
    }
    if (timeout)
      ctw.out.println("Timeout on lock - I've taken over!<br>");
    locked=true;
  }

  public void unlock() {
    locked=false;
  }

  public void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

    CTW ctw=new CTW();

    ctw.req=request;
    ctw.res=response;

    // Set the Content-Type header
//    out.println("Content-type: text/html\n");
//      res.setHeader("Content-type","text/html");
    ctw.res.setContentType("text/html");
    // Return early if this is a HEAD
    if (ctw.req.getMethod().equals("HEAD"))
      return;

    // Proceed otherwise
    ctw.out = ctw.res.getWriter();

//    trytoshow("/");
//    trytoshow("/members/");
//    trytoshow(ServletPath);
//    trytoshow(DataPath);

    /* Test to show that things get remembered between calls to the servlet
    if (sessionid!=null)
      out.println("Remembering old sessionid<BR>"); */

/*    if (sessions==null)
      out.println("New sessions every psge change!<br>");
    if (XML.log.length()==0) {
      out.println("New log never (actually on recompile, reboot, ... )!<br>");
    }*/

    log="";
    ctw.log="";
    XML.log="";//XML log:<br>\n";
    Files.log="";//Files log:<br>\n";
    Profiler.log="";
    XDB.log="";
    Log.clear();
    Profile.clear();

    ctw.sessions=null;
    ctw.people=null;
    ctw.sessionid=null;
    ctw.session=null;
    ctw.person=null;

    XML.read=new Vector();
//    XML.log="";

    Profile.start("CTWServlet inside");

    try {

    // Assumes recovery of the user always desired if possible
    // Also implies getsessionid
    ctw.respondall();
    log+="b4 lock\n";

    lock(ctw);

    log+="Past lock\n";

    ctw.getperson();
    ctw.respondall();
    ctw.writesessions();
    ctw.out.print("");

    unlock();

    } catch (Exception e) {
      ctw.out.println("CTWServlet.doGet: <b>"+e+"</b><br>");
    }

    /* Fun with header info */
    ctw.out.println("HELLO");

    Enumeration e=ctw.req.getHeaderNames();
    if (e!=null) {
    ctw.out.println("Headerszzzz:<br>");
    while (e.hasMoreElements()) {
      try {
      String name=(String)e.nextElement();
      String value=ctw.req.getHeader(name);
      ctw.out.println("&nbsp;&nbsp;"+name+": "+value+"<br>");
      } catch (Exception ex) {
        ctw.out.println("&nbsp;&nbsp;"+ex+"<br>");
      }
    }
    }
    /**/

    Profile.stop("CTWServlet inside");

//    out.println("XML.read has read "+XML.read.size()+" XML objects<br>");
    ctw.out.println("<br><center><a href=\""+CTW.HomeUrl+"\">changetheworld home</a><br>");

//    if ((XML.log.length()>0 || log.length()>0) && CTW.Debugging) {
    if (CTW.Debugging || Log.error) {
      ctw.out.println("<p align=\"left\"><i>Debugging information</i><br><br><tt>\n");
      ctw.out.println("<font size=\"+2\">Log log:</font><br>\n"+Log.logHtml+"<br><br>\n");
      ctw.out.println("<font size=\"+2\">Profiles:</font><br>\n"+Profile.report()+"<br><br>\n");
      ctw.out.println("</tt></p>\n");
/*      ctw.out.println("XML log:<br>"+XML.decode(XML.log)+"<br><br>\n");
      ctw.out.println("Files log:<br>"+XML.decode(Files.log)+"<br><br>\n");
      ctw.out.println("CTWServlet.log:<br>\n"+XML.decode(log)+"<br><br>\n");
      ctw.out.println("CTW.log:<br>\n"+XML.decode(ctw.log)+"<br><br>\n");
      ctw.out.println("XDB.log:<br>\n"+XML.decode(XDB.log)+"<br><br>\n");
      ctw.out.println("Log log:<br>\n"+XML.decode(Log.log)+"<br><br>\n");
      ctw.out.println(Search.log); */
    }
    ctw.out.println("</BODY></HTML>");

  }
  public void doPost(HttpServletRequest req, HttpServletResponse res) throws ServletException, IOException {
    doGet(req, res);
  }

  String getcookie(String name) {
    Object tmp=req.getCookies();
    if (tmp==null)
      return null;
    Cookie[] cs=(Cookie[])tmp;
    for (int i=0;i<cs.length;i++) {
      Cookie c=cs[i];
      if (c.getName().equals(name))
        return c.getValue();
    }
    return null;
  }
  void setcookie(String name,String val,int time) {
    Cookie c=new Cookie(name,val);
    c.setMaxAge(time);
    res.addCookie(c);
  }

  void getpeople() throws Exception {
    if (people==null)
      people=XML.openpath("people");
//      people=XML.parsefile("people");
  }
  void getsessions() throws Exception {
    if (sessions==null) {
      sessions=XML.openpath("sessions");
//      out.println(""+sessions.render());
//      sessions=XML.parsefile("sessions");
      // Remove old sessions
      for (int i=0;i<sessions.inside.size();i++) {
        Session session=(Session)sessions.inside.get(i);
        try {
          Date d=new SimpleDateFormat().parse(session.time);
          if (new Date().getTime()-d.getTime()>1000*SessionTimeout) { // milliseconds
            sessions.inside.remove(session);
            i--;
          }
        } catch (Exception e) { }
      }
    }
  }
  void writesessions() {
    try {
      if (sessions!=null) {
        if (!sessions.write()/*path("sessions")*/)
          out.println("Didn't write sessions.<br>");
//        sessions.writefile("sessions");
//        out.println("<h2>Sessions file successfully written!</h2><br>");
      }
    } catch (Exception e) {
      out.println("<br>CTWServlet.writesessions: <b>"+e+"</b><br><br>");
    }
  }
  void writepeople() {
    try {
    if (people!=null)
      out.println("Writing people"+XML.encode(people.toString())+"<br>\n");
      people.writepath("people");
//      people.writefile("people");
    } catch (Exception e) {
      out.println("<br>CTWServlet.writepeople: <b>"+e+"</b><br><br>");
    }
  }

  void getperson() throws Exception {
    // Try to find the person from the cookie
    person=null;
    getsessions();
//sessions=new XML();
    Object tmp=getcookie("ctwsessionid");
    if (tmp!=null) {
      String sid=(String)tmp;
//      getpeople();
      session=(Session)XML.findID(sid);
//      session=(Session)sessions.getpath(sid); // (Session)XML.findID(sid);
      if (session!=null)
        if (session.link!=null) {
          Object t=session.link.follow();
          if (t!=null)
            person=(Person)t;
        }
        // Remove session, because about to set up new one
        // If people keep losing cookies and going back to earlier ones, try taking this out
      sessions.inside.remove(session);
    }
    // If person doesn't exist, make a fake one to track?

    // Set up a new session for the person
    session=new Session();
    sessionid=newsessionid();
    session.setID(sessionid);
    if (person!=null) {
      Link link=new Link(person.path);
//      link.addpar("person",person.getID());
      session.link=link;
    }
    session.time=new SimpleDateFormat().format(new Date());
    sessions.inside.add(session);
    setcookie("ctwsessionid",sessionid,60*15);
//    setcookie("sessionid","",0);
  }

  String newsessionid() {
    return "Session:"+JString.random(20);
  }
  void getsessionid() throws Exception {
    if (sessionid==null) {
      out.println("Getting sessionid<br>");
      sessionid=getcookie("ctwsessionid");
      getsessions();
      session=(Session)XML.findID(sessionid);
      out.println("Session: "+session+"<br>");
      if (session==null) { // No session, generate a new one
        out.println("Generating new sessionid<br>");
        sessionid=newsessionid();
        while (XML.findID(sessionid)!=null)
          sessionid=newsessionid();
        session=new Session();
        sessions.inside.add(session);
        session.setID(sessionid);
      }
      session.time=new SimpleDateFormat().format(new Date());
    }
  }

  String title() {
    return "CTWServlet Base Class";
  }
  String backgroundcolour() {
    return "FFFFFF";
  }
  void respondall() {
    out.println("<HTML>");
    out.println("<HEAD><TITLE>"+title()+"</TITLE></HEAD>");
    out.println("<BODY bgcolor=\"#"+backgroundcolour()+"\">");
    respond();
  }
  void respond() {
    out.println("CTWServlet.respond: This is the base class.  Extend it!");
  }

/*  void trytoshow(String dir) {
    try {
      out.println("<b>Listing directory <tt>"+dir+"</tt></b><br>");
      File f=new File(dir);
      String[] ss=f.list();
      for (int i=0;i<ss.length;i++)
        out.println(""+ss[i]+"<br>");
    } catch (Exception e) {
      out.println("CTWServlet.trytoshow: "+e+"<br>");
    }
  }*/

  public static Map mapofparams(HttpServletRequest req) {
    Map map=new Hashtable();
    Enumeration ps=req.getParameterNames();
    while (ps.hasMoreElements()) {
      String p=(String)ps.nextElement();
      String r=req.getParameter(p);
      if (r!=null)
        map.put(p,r); // XML.paramdecode equivalent should be done by Java
    }
    return map;
  }

  public XML getXMLfromform() {
    return getXMLfromform(mapofparams(req));
    /*
    String s=req.getParameter("javatype");
    if (s!=null)
      if (s.length()>0)
        return getXMLfromform(s);
    XML.log+="CTWServlet.getXMLfromform: no javatype\n";
    return null;*/
  }
  public XML getXMLfromform(String type) {
    return getXMLfromform(type,mapofparams(req));
    /*
    try {
      XML x=XML.oftype(type);
      updateXMLfromform(x);
        Field f=x.findField("author");
        if (f!=null && person!=null) {
          Link l=new Link(person.path);
          log+="Trying to drop author in.\n";
          x.setField("author",l);
          log+="Author done.\n";
        }
        f=x.findField("creationdate");
        if (f!=null)
          x.setField("creationdate",new SimpleDateFormat().format(new Date()));
      return x;
    } catch (Exception e) {
      out.println("CTWServlet.getXMLfromform: "+e);
      XML.log+="CTWServlet.getXMLfromform: "+e+"\n";
    }
    return null;*/
  }
  public void updateXMLfromform(XML x) {
    updateXMLfromform(x,"",mapofparams(req));
//    updateXMLfromform(x,"");
  }
  public void updateXMLfromform(XML x,String xmlpath) {
    updateXMLfromform(x,xmlpath,mapofparams(req));
    /*
    try {
//      out.println("x="+x.type+"<br>");
      if (x!=null) {
      x.newID(); // will allow ID in form to override if it exists
      Field[] fs=x.openFields();
      for (int i=0;i<fs.length;i++) {
//        log+="Field: "+fs[i].getType().getName()+"\n";
        String f=fs[i].getName();
        String s=req.getParameter(xmlpath+f);
        boolean hit=false;
        if (Class.forName("changetheworld.XML").isAssignableFrom(fs[i].getType())) {
          log+="Trying to update sub-object "+f+"\n";
          try {
            // Inherit fields from father
//            updateXMLfromform((XML)fs[i].get(x),"");
            // Parse sub-form for sub-object
            updateXMLfromform((XML)fs[i].get(x),f+".");
            hit=true;
          } catch (Exception e) {
            log+="Error updating sub-object "+f+"<br>\n";
          }
        }
        if (!hit && (fs[i].getType().getName().equals("java.lang.Integer") || fs[i].getType().getName().equals("int"))) {
          hit=x.setField(f,new Integer(s));
        }
        if (!hit && (fs[i].getType().getName().equals("java.lang.Float") || fs[i].getType().getName().equals("float"))) {
          hit=x.setField(f,new Float(s));
        }
        if (!hit && (fs[i].getType().getName().equals("java.lang.Double") || fs[i].getType().getName().equals("double"))) {
          hit=x.setField(f,new Double(s));
        }
        if (!hit && s!=null)
          hit=x.setField(f,s);
        if (hit)
          log+="=) managed to drop "+s+" into "+f+"\n";
        if (!hit)
          log+="Failed to drop "+s+" into "+f+"\n";
//        else
//          out.println("Managed to drop "+s+" into "+f+"<br>");
      }
      }
      } catch (Exception e) {
        out.println("CTWServlet.updateXMLfromform(2): "+e);
        XML.log+="CTWServlet.updateXMLfromform(2): no javatype\n";
      }
    */
  }

  public XML getXMLfromform(Map m) {
    String s=(String)m.get("javatype");
    if (s!=null)
      if (s.length()>0)
        return getXMLfromform(s,m);
    return null;
  }
  public XML getXMLfromform(String type,Map map) {
    try {
      XML x=XML.oftype(type);
      updateXMLfromform(x,map);
        Field f=x.findField("author");
        if (f!=null && person!=null) {
          Link l=new Link(person.path);
          log+="Trying to drop author in.\n";
          x.setField("author",l);
          log+="Author done.\n";
        }
        f=x.findField("creationdate");
        if (f!=null)
          x.setField("creationdate",new SimpleDateFormat().format(new Date()));
      return x;
    } catch (Exception e) {
      out.println("CTWServlet.getXMLfromform: "+e);
      log+="CTWServlet.getXMLfromform: "+e+"\n";
    }
    return null;
  }
  public void updateXMLfromform(XML x,Map map) {
    updateXMLfromform(x,"",map);
  }
  public void updateXMLfromform(XML x,String xmlpath,Map map) {
    xmlpath="";
    try {
//      out.println("x="+x.type+"<br>");
      if (x!=null) {
      x.setID(x.newID()); // will allow ID in form to override if it exists
      Field[] fs=x.openFields();
      //Log.reportHtml("<b>Updating fields "+fs+" in "+x.id+"</b><br>\n");
      x.fieldsRead=true; // a bit premature!
      for (int i=0;i<fs.length;i++) {
//        log+="Field: "+fs[i].getType().getName()+"\n";
        String f=fs[i].getName();
        String s=(String)map.get(xmlpath+f);
        boolean hit=false;
        if (!hit && (fs[i].getType().getName().equals("java.lang.Double") || fs[i].getType().getName().equals("boolean"))) {
          if (s!=null)
            if (s.equals("on"))
              hit=x.setField(f,new Boolean(true));
          if (!hit)
            x.setField(f,new Boolean(false));
        }
        if (Class.forName("changetheworld.XML").isAssignableFrom(fs[i].getType())) {
          log+="Trying to update sub-object "+f+"<br>\n";
          try {
            // Inherit fields from father
//            updateXMLfromform((XML)fs[i].get(x),"");
            // Parse sub-form for sub-object
            //Log.reportHtml("<b>Updating "+f+"</b><br>\n");
            updateXMLfromform((XML)fs[i].get(x),f+".",map);
            //Log.reportHtml("<b>Got "+((XML)fs[i].get(x)).id+"</b><br>\n");
            hit=true;
          } catch (Exception e) {
            log+="Error updating sub-object "+f+": "+e+"<br>\n";
          }
        }
        if (s!=null && !s.equals("null")) {
        try {
        if (!hit && (fs[i].getType().getName().equals("java.lang.Integer") || fs[i].getType().getName().equals("int"))) {
          hit=x.setField(f,new Integer(s));
        }
        if (!hit && (fs[i].getType().getName().equals("java.lang.Float") || fs[i].getType().getName().equals("float"))) {
          hit=x.setField(f,new Float(s));
        }
        if (!hit && (fs[i].getType().getName().equals("java.lang.Double") || fs[i].getType().getName().equals("double"))) {
          hit=x.setField(f,new Double(s));
        }
        } catch (Exception e) {
          out.println("CTWServlet.updateXMLfromform(3): "+e);
          log+="CTWServlet.updateXMLfromform(3): "+e+"\n";
        }
        if (!hit && s!=null) // s should never = null since would cause class cast exception
          hit=x.setField(f,XML.paramdecode(s));
/*        if (!hit && person!=null && f.equals("author") && fs[i].getType().getName().equals("changetheworld.Link")) {
          Link l=new Link(person.path);
          out.println("Trying to drop author in.<br>");
          x.setField(f,l);
        }*/
        }
        if (hit)
          log+="Managed to drop "+s+" into "+f+"\n";
        if (!hit)
          log+="Failed to drop "+s+" into "+f+"\n";
//        else
//          out.println("Managed to drop "+s+" into "+f+"<br>");
      }
      }
      } catch (Exception e) {
        out.println("CTWServlet.updateXMLfromform(3): "+e);
        log+="CTWServlet.updateXMLfromform(3): "+e+"\n";
      }
  }

  public static String dbaseforjavatype(String type) {
      return    ( type.equals("Debate") ? "debates" :
                  type.equals("Event") ? "events" :
                  type.equals("Person") ? "people" :
                  type.equals("Decision") ? "decisions" :
                  type.equals("Issue") ? "issues" : null );
  }

  public static final String[] simpletypes() {
    String[] s={ "Question","Category","Debate","Event","Decision","Issue",
      "SolidarityIssue","SolidarityCount",
      "Link","Fact","Place","FastLink","Service" };
    return s;
  }

  public static final String[] alltypes() {
    String[] s={ "Question","Category","Debate","Event","Decision","Issue",
      "SolidarityIssue","SolidarityCount",
      "Link","Fact","Place","FastLink","Service",
      "Search", "Task", "VoteRecord", "Database",
      "Person", "Vote" };
    return s;
  }

}
