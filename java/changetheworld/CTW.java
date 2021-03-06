package changetheworld; import jlib.JString;

import javax.servlet.http.HttpSession;

import java.lang.String;
import java.lang.reflect.*;
import java.util.Vector;
import java.util.Map;
import java.util.Hashtable;
import java.util.Enumeration;
import java.util.jar.*;
import java.net.*;
import java.io.*;
import java.util.Date;

import jlib.Log;
import jlib.JHtml;
import jlib.Profile;

import changetheworld.Voteable;

// As well as being the main class for static functions for CTW,
// CTW is also the main servlet dealing with users requests



class FakePrintWriter extends PrintWriter {
  public String store="";
/*  public FakePrintWriter() {
  }*/
  public FakePrintWriter(Writer out) {
    super(out);
  }
  public void print(String s) {
//    XML.log+="Hello!\n";
    store+=s;
  }
  public void println(String s) {
    print(s+"\n");
  }
}

interface StringConverter {
  public String convert(String c);
}

class SimpleStarStringConverter implements StringConverter {
  String before;
  String after;
  SimpleStarStringConverter(String s) {
    int i=s.indexOf("*");
    if (i==-1) {
      System.out.println("Pattern "+s+" must contain a *");
      XML.log+="Pattern "+XML.encode(s)+" must contain a *";
//      System.exit(0);
    }
    before=s.substring(0,i);
    after=s.substring(i+1);
  }
  public String convert(String s) {
    return before+s+after;
  }
}

class URLUpdaterStringConverter implements StringConverter {
  URL src;
  String obefore,oafter,before,after;
  URLUpdaterStringConverter(URL sr,String pat,String s) {
    int k=sr.toString().lastIndexOf("/");
    try {
    src=( k==-1 ? sr : new URL(sr.toString().substring(0,k+1)) );
    } catch (Exception e) { }
    int i=s.indexOf("*");
    if (i==-1) {
      System.out.println("Pattern "+s+" must contain a *");
      XML.log+="Pattern "+XML.encode(s)+" must contain a *";
//      System.exit(0);
    }
    before=s.substring(0,i);
    after=s.substring(i+1);
    int j=pat.indexOf("*");
    if (j==-1) {
      System.out.println("Pattern "+pat+" must contain a *");
      XML.log+="Pattern "+XML.encode(s)+" must contain a *";
//      System.exit(0);
    }
    obefore=pat.substring(0,j);
    oafter=pat.substring(j+1);
//    XML.log+="StringConverter will convert "+pat+" to "+s+" (source="+sr+")\n";
  }
  public String convert(String s) {
    try {
      String news=(new URL(src,s)).toString();
      if ((s.endsWith(".html") || s.endsWith(".htm"))
       && s.indexOf("psybertech")==-1
       && s.indexOf(CTW.Host)==-1
       && s.indexOf("banners")==-1 ) {
//        XML.log+="Replacing "+XML.encode(obefore+">"+s+"<"+oafter)+" with "+XML.encode(before+news+after)+"<br>\n";
        return before+news+after;
      } else {
//        XML.log+="Not replacing "+XML.encode(obefore+s+oafter)+"<br>\n";
        return obefore+s+oafter;
      }
    } catch (Exception e) {
      return "failure "+e+" "+before+s+after;
    }
    //return before+s+after;
  }
}


  class PatternResult {
    String star;
    int start,end;
    PatternResult(String s,int i,int j) {
      star=s;
      start=i;
      end=j;
    }
  }

class PatternSearch {

  private static PatternResult findpattern(String s,String p,int from) {
    Vector v=JString.splitv(p,"*");
    if (v.size()!=2) {
      System.out.println("Pattern "+p+" must contain a *");
      XML.log+="Pattern "+s+" must contain a *";
//      System.exit(0);
    }
    String before=(String)v.get(0);
    String after=(String)v.get(1);
    int i=s.indexOf(before,from);
    int j=s.indexOf(after,i);
    if (i>-1 && j>-1) {
      PatternResult pr=new PatternResult(s.substring(i+before.length(),j),i,j+after.length());
      return pr;
    }
    return null;
  }

  public static String convertonestar(String s,String pattern,StringConverter c) {
    StringBuffer sb=new StringBuffer(s);
    int here=0;
    PatternResult res=findpattern(sb.toString(),pattern,here);
    while (res!=null) {
      String cnv=c.convert(res.star);
      sb=sb.delete(res.start,res.end);
      sb=sb.insert(res.start,cnv);
      here=res.start+cnv.length();
      res=findpattern(sb.toString(),pattern,here);
//      res=null;
    }
    return sb.toString();
  }

}



/******************************/
/*   CTW starts               */
/******************************/

public class CTW extends CTWServlet {

//  public static final boolean Debugging=true;
  public static final boolean Debugging=false;
  public static final String Host="hwi"; // "psybertech.yi.org";//"loki";//"hwi";//"www.mycgiserver.com"
  public static final String ServletUrl="/servlet/changetheworld."; // "http://"+Host+"/servlet/changetheworld.";
  public static final String Url=ServletUrl+"CTW";
  public static final String HomeUrl=Url+"?command=display&page=/changetheworld/theindex.html";
//  public static final String ServletPath="/members/1ZMaV5H9w1f56Cl4UEa6p7joCvatNGH3/"; // changetheworld/";
  public static final String ServletPath="/home/httpd/servlets/";
  public static final String DataPath=ServletPath+"data/";
  public static final String PlainName="ctwdata.xml";
  public static final String PlainPath=DataPath+PlainName;
  public static final String TreePath=DataPath+"tree/";
  public static final String JarPath=DataPath+"CtwData.jar";
  public static final String PagesUrl="/changetheworld/"; // "http://"+Host+"/changetheworld/";
  public static final String AbsPagesUrl="http://"+Host+PagesUrl;
//  public static final String RelPagesUrl="/changetheworld/";
  // public static final String PagesDir="/home/httpd/html/";
  public static final String PagesDir="/var/www/";
  public static final String LoginUrl=PagesUrl+"login.html";
  public static final String SignUpUrl=PagesUrl+"signup.html";
  public static final String LoginTag="<a href=\""+LoginUrl+"\">Log in</a>";
  public static final String SignUpTag="<a href=\""+SignUpUrl+"\">Sign up</a>";

  public final static void main(String[] argv) {
    String s="";
    for (int i=0;i<argv.length;i++)
      s+=argv[0]+( i==argv.length-1 ? "" : " " );
    CTW c=new CTW();
    System.out.println(JHtml.removeTags(c.docommand(s)));
  }

  public String title() {
    return "changetheworld.org.uk";
  }
  public String backgroundcolour() {
    return "FFFFFF";
  }

  public void respond() {
    try {

// Trying to get referring page
//    out.println("-> "+req.getAttribute("HTTP-REFERER")+"<br>");
/*    HttpSession ses=req.getSession(true);
    if (ses==null)
      out.println("No session recovered<br>");
    else {
      String[] l=ses.getValueNames();
      out.println("Session names: ("+l.length+")<br>");
      for (int i=0;i<l.length;i++) {
        out.println("&nbsp;&nbsp;"+l[i]+"<br>");
      }
    }*/


    respond(CTWServlet.mapofparams(req));
    } catch (Exception e) {
      out.println("CTW.respond: "+e);
    }
  }

  void respond(Map map) {
    docommand(map);
    loginoption();
  }

  void docommand(Map map) {
    String command=(String)map.get("command");//req.getParameter("command");
    Method m=null;
    try {
      Class[] cs=new Class[1];
      cs[0]=Class.forName("java.util.Map");
      m=this.getClass().getMethod(command,cs);
      // out.println("Found method "+m+"<br>");
    } catch (Exception e) {
    }
    if (m==null) {
      out.println("CTW.respond: Could not find method "+command);
    } else {
      Profile.start("CTW method "+m.getName());
      try {
        Object[] os=new Object[1];
        os[0]=map;
        Object tmp=m.invoke(this,os);
        if (JReflect.canbe(tmp,"java.lang.String"))
          out.print(tmp);
      } catch (Exception e) {
        out.println("CTW.respond: error invoking "+m+": "+e+"<br>");
      }
      Profile.stop("CTW method "+m.getName());
    }
    Object tmp=map.get("endupcommand");
    if (tmp!=null) {
      String endupat=(String)tmp;
      endupat=XML.paramdecode(endupat);
      out.println("<i><b>Trying to finally take you to result of command "+endupat+"</b></i><br>\n");
      out.println(docommand(endupat));
    }
    Object tmp2=map.get("endupat");
    if (tmp2!=null) {
      String endupat=(String)tmp2;
			endupat=XML.paramdecode(endupat);
			out.println("<script>setTimeout(\"location.href='"+endupat+"';\",1000)</script>\n");
		}
  }

  private XML readdbase(String path) {
    String dbfile=XML.getdbfile(path);
    return (XML)XML.parsefile(dbfile);
  }
  private Object readpath(XML f,String path) {
    String xmlpath=XML.getxmlpath(path);
    xmlpath=JString.remove(xmlpath,f.type+".");
    return f.getpath(xmlpath);
  }

/*******************************************************/
/*      CGI methods start here                         */
/*******************************************************/

  public void login(Map args) {
   try {
    people=XML.openpath("people"); // getpeople();
    String name=(String)args.get("name");
    String pass=(String)args.get("pass");
    if (name==null || pass==null) {
      out.println("Some problem with CGI parameters<br>");
    }
    SatisfiesAll sa=new SatisfiesAll();
    sa.add(new ObjectContainsField("name",name));
    sa.add(new ObjectContainsField("pass",pass));
//    out.println(people.render());
//    out.println(sessions.render());
    Object tmp=JVector.getfirst(sa,people.inside);
    person=( tmp==null ? null : (Person)tmp );
    if (person==null) {
      out.println("Login failed - couldn't find person called "+name+" or password wrong.<br>");
    } else {
      Link link=new Link(person.path);
      session.link=link;
      // writesessions(); Automatically done by CTWServlet
      out.println("Login suceeded.<br>");
      out.println(person.render(new HTMLRenderContext()));
      out.println("<a href=\"/servlet/changetheworld.CTW?command=requestedit&editpath="+person.path+"\">Click here</a> to edit your data<br>");
    }
   } catch (Exception e) {
    out.println("CTW.login(): <b>"+e+"</b><br>");
   }
  }

  public void signup(Map args) {
  	try {
      XML n=getXMLfromform("Person",args);
      Person p=(Person)n;
  //    out.println(XML.encode(n.toString()));
      people=XML.openpath("people"); // getpeople();
      String name=p.name;
      if (name==null) {
        out.println("Some problem with CGI parameters<br>");
      }
      Satisfier s=new ObjectContainsField("name",name);
      Object tmp=JVector.getfirst(s,people.inside);
      Person found=( tmp==null ? null : (Person)tmp );
      if (found!=null) {
        out.println("Sorry there is already a person called \""+name+"\".  Please go back and choose a new name.<br>");
      } else {
	      out.println(p.render());
        people.inside.add(p);
        people.write();
      }
    } catch (Exception e) {
    	out.println("CTW.signup(): "+e+"<br>\n");
    	XML.log+="CTW.signup(): "+e+"\n";
    }
  }

  public void dotask(Map args) {
    try {
      String tp=(String)args.get("taskpath");
      Task t=(Task)XML.openpath(tp);
      if (t.complete(args,this)) {
        out.println(docommand("delete deletepath="+t.path+" deleteconfirm=yesplease"));
      }
    } catch (Exception e) {
      out.println("CTW.completetask(): "+e+"<br>\n");
      XML.log+="CTW.completetask(): "+e+"\n";
    }
  }

  public void move(Map args) {
    try {
    String movesrc=(String)args.get("movesrc");
    if (person==null)
      out.println("Sorry, you have to be logged on to move stuff.");
    else {
      Object tmp=args.get("movedest");
      if (tmp==null) {
        out.println("Person has "+person.insidesize+" inside<br>\n");
//        out.println("First is "+((XML)person.inside.get(0)).render()+"<br>\n");
        person.tasks.inside.add(new MoveTask(movesrc));
        person.write();
        out.println("Go find somewhere to move it to!");
        out.println(docommand("render path="+XML.parentpath(movesrc)));
      } else {
        String movedest=(String)tmp;
        XML xpar=XML.openpath(XML.parentpath(movesrc));
        XML xsrc=(XML)xpar.getpath(XML.childpath(movesrc)); //XML.openpath(movesrc);
        // Need to delete and rewrite before reading destination
        // in case it's inside the destination
        // The case when the destination is inside it is not allowed!
        xpar.deleteID(xsrc.id);
        Files.log+="\nWriting source parent:\n";
        if (!xpar.write()) {
          out.println("CTW.move: couldn't write the source parent<br>\n");
          return;
        }
        out.println("Object<br>"+xsrc.render(new HTMLRenderContext("list"))+"<br>has been deleted, leaving parent<br>"+xpar.render(new HTMLRenderContext("list"))+".<br>\n");
        XML xdest=XML.openpath(movedest);
        boolean nodest=false;
        if (xdest==null)
          nodest=true;
        if (!nodest) {
          xdest.inside.add(xsrc);
          Files.log+="\nWriting destination:\n";
          nodest=!xdest.write();
        }
        if (nodest) {
          out.println("Couldn't write the destination.  Attempting rewrite of source parent<br>\n");
          xpar.inside.add(xsrc);
          if (xpar.write())
            out.println("Succeeded!<br>\n");
          else
            out.println("Failed!<br>\n");
        } else {
          out.println("Object<br>"+xsrc.render(new HTMLRenderContext("list"))+"<br>has been copied to<br>"+xdest.render(new HTMLRenderContext("list"))+".<br>\n");
          out.println(docommand("render path="+movedest));
          String taskpath=(String)args.get("taskpath");
        }
      }
    }
    } catch (Exception e) {
      out.println("CTW.move(): "+e+"<br>\n");
    }
  }

  public void link(Map args) {
    try {
    String linksrc=(String)args.get("linksrc");
    if (person==null)
      out.println("Sorry, you have to be logged on to link stuff.");
    else {
      Object tmp=args.get("linkdest");
      if (tmp==null) {
        out.println("Person has "+person.insidesize+" inside<br>\n");
//        out.println("First is "+((XML)person.inside.get(0)).render()+"<br>\n");
        person.tasks.inside.add(new LinkTask(linksrc));
        person.write();
        out.println("Go find somewhere to link from!");
        out.println(docommand("render path="+XML.parentpath(linksrc)));
      } else {
        String linkdest=(String)tmp;
        XML x=XDB.readtodepth(linkdest,0);
        if (x==null) {
          out.println("Destination "+linkdest+" does not exist!<br>");
        } else {
          Link l=new Link(linksrc);
          l.setID(l.newID());
          l.path=new File(linksrc,l.id).toString();
          out.println("New link path="+l.path+"<br>");
          //l.write();

          out.println("New link to "+l.seekpath+" created inside "+linkdest+"<br>");
        }
      }
    }
    } catch (Exception e) {
      out.println("CTW.link(): "+e+"<br>\n");
    }
  }

  public String dohashtwiddle(String page) {
    StringBuffer sb=new StringBuffer(page);
    dohashtwiddle(sb);
    return sb.toString();
  }

  public void dohashtwiddle(StringBuffer page) {
      int i=page.toString().indexOf("#~");
      while (i>-1) {
        int j=page.toString().indexOf("~#");
        String com=page.toString().substring(i+2,j);
        //ins+="Found command "+com;
        String result=docommand(com);
        String pre="\n\n<!--%~"+com+"~%-->\n";
        String post="<!--%~end "+com+"~%-->\n";
        page=page.delete(i,j+2);
        page=page.insert(i,pre+result+post);
        i=page.toString().indexOf("#~");
      }
  }

  public void display(Map args) {
    String pagename=(String)args.get("page");
    String ins="";
    try {
    StringBuffer page=null;
    URL pageurl=null;
    try {
      try {
        pageurl=new URL(AbsPagesUrl);
        pageurl=new URL(pagename);
      } catch (Exception e) {
        String slsh=( pagename.charAt(0)=='/' ? "" : "/" );
        pageurl=new URL(AbsPagesUrl+slsh+pagename);
      }
//      Object pagecontent=pageurl.getContent();
    } catch (Exception e) {
      out.println(""+e);
    }
      // sun.net.www.MeteredStream - might work on Internet
//    if (pagename.startsWith(PagesUrl)) {
      String fname=JString.after(pagename,PagesUrl);
      String pagestr=Files.readStringfromfile(PagesDir+fname);
      if (pagestr==null)
        out.println(Files.log);
      page=new StringBuffer(pagestr);

    if (page!=null) {

//    out.println(page.toString());

      // Render all #~ commands
      dohashtwiddle(page);

      // Make all html links run this changetheworld Display parser
      String pattern="<a href=\"*\">";
//      String pattern="<a href=\"*.html\">";
      StringConverter s=new URLUpdaterStringConverter(pageurl,pattern,"<a href=\""+Url+"?command=display&page=*\">");
//      StringConverter s=new URLUpdaterStringConverter(pageurl,pattern,"<a href=\""+Url+"?command=display&page=*.html\">");
      String respage=PatternSearch.convertonestar(page.toString(),pattern,s);
      page=new StringBuffer(respage);

      // Insert message
      int i=page.toString().indexOf("<body>");
      int j=page.toString().indexOf("<BODY>");
      if (i==-1)
        i=j;
      i+=6;
      page.insert(i,ins);
      page.insert(0,"<!--Please do not edit, this is a rendered file.  Please edit source file "+pageurl+"-->\n"+ins);
      out.println(page.toString());
      out.println("<a href=\""+pageurl+"\"><center><p>View original<a><br>");
    } else {
      out.println("Could not recover page "+pagename);
    }
    } catch (Exception e) {
      out.println("CTW.display("+pagename+"):"+e);
    }
  }

  public void linkto(Map args) {
    try {
    String path=(String)args.get("path");
    Object tmp=args.get("text");
    XML x=null;
    XDB.log="";
    x=XDB.readtodepth(path,0);
    if (x==null)
      out.println("XDB.readtodepth failed<br>"+XDB.log);
//    XML x=XML.openpath(path);
    if (x!=null) {
      String text=( tmp==null ? "" : (String)tmp );
      if (text.trim().length()>0 || !JReflect.canbe(x,"changetheworld.RenderableXML"))
        out.print(((RenderableXML)x).urltome(text));
      else
        out.print(x.renderlink());
//    out.print("<a href=\""+Url+"?command=render&path="+path+"\">"+text+"</a>");
    }
    } catch (Exception e) {
      out.println("CTW.linkto: "+e+"<br>\n");
      log+="CTW.linkto(): "+e+"<br>\n";  
    }
  }

  public void loginoption() {
    if (person!=null) {
      out.println("<p align=\"center\"><small>You are logged on as "+person.urltome(person.name)+"</small><br>");
    } else {
      out.println("<p align=\"center\">"+LoginTag+"&nbsp;&nbsp;&nbsp;"+SignUpTag+"<br>");
//      out.println("<p align=\"right\"><small>You have session id "+sessionid+" of "+sessions.insidesize);
    }
  }
  public void loginoption(Map args) {
    loginoption();
  }

  public String docommand(String com) {
    //out.println("Executing #~ command "+com);
/*    if (com.equals("Log in"))
      return LoginTag;
    else if (com.equals("Sign up"))
      return SignUpTag;*/
//    else {
      String s="";
      PrintWriter tmp=out;
      out=new FakePrintWriter(out);
      try {
        // Split command
        if (!com.startsWith("command="))
          com="command="+com;
        Map m=new Hashtable();
        Vector as=JString.splitv(com," ");
        for (int i=0;i<as.size();i++) {
          Vector p=JString.splitv((String)as.get(i),"=");
          m.put((String)p.get(0),XML.paramdecode((String)p.get(1)));
        }
        docommand(m);
        s+=((FakePrintWriter)out).store;
      } catch (Exception e) {
        s+=e;
      }
      out=tmp;
      return s;
//    }
  }

  public void render(Map args) {
    try {
//    Profile.clear();
//    Profiles.clear();
//    Profile.start("rendermethod");
    String renderpath=(String)args.get("path");
/*    out.println("Getting jar "+JarPath+"<br>");
    JarFile jf=new JarFile(JarPath);
    Enumeration e=jf.entries();
    while (e.hasMoreElements())
      out.println(e.nextElement()+"<br>");*/
//    XML x=XML.openpath(renderpath);
//    Profiler profa=new Profiler("reading "+renderpath);
    Profile.start("Reading");
    XML x=XDB.readtodepth(renderpath,1);
    Profile.stop();
//    profa.end();
    if (JReflect.canbe(x,"changetheworld.FastLink")) {
      Object tmp=args.get("rerender");
      if (tmp!=null) {
        String s=(String)tmp;
        if (s.equals("true"))
          ((FastLink)x).rendered=null;
      }
    }
    // reducing to 1 means when inside is rendered,
    // the size of the inside of those objects is zero
//    out.println(XDB.log);
    if (x==null)
      out.println("Found "+x+" at "+renderpath+"<br>XML log:<br>"+XML.log);
    else {
//      out.println(x.path+" "+x.id+"<br>"+XML.encode(x.toString()));
      HTMLRenderContext hrc=new HTMLRenderContext("browse",person);
      hrc.where=CTW.Url+"?command=render&path="+renderpath;
      Profiler profb=new Profiler("Rendering");
      Profile.start("Rendering");
      String objr=x.render(hrc);
      objr=dohashtwiddle(objr);
      out.println(objr);
      Profile.stop();
      profb.end();
      if (JReflect.canbe(x,"changetheworld.CTWInput")) {
        CTWInput y=(CTWInput)x;
        y.visits++;
        y.write();
      }
    }
//    Profile.stop();
//    Profiles.end();
//    out.println(Profiler.log);
//    out.println(Profile.report());
    } catch (Exception e) {
      out.println("CTW.render: "+e+"<br>"+XML.log+"<br>");
    }
  }

  public void realtimeedit(Map args) {
    String type=(String)args.get("javatype");
      if (type.startsWith("a "))
        type=type.substring(2);
      if (type.startsWith("an "))
        type=type.substring(3);
    String destpath=(String)args.get("destpath");

    out.println("<center><table align=\"center\" bgcolor=\"#80b080\"><tr><td><center><br><h2>Displaying a "+type+/*" to "+destpath+*/"</h2>");
//      XML x=XML.oftype(type);
    XML x=getXMLfromform(args); // which will mostly be empty ;)
      HTMLRenderContext c=new HTMLRenderContext("realtimeeditform");
      c.destpath=destpath;
      out.println(""+x.renderXML(c)+"</td></td></table>");
      out.println("<br><br>\n");
      out.println(""+x.render());
      x.path="tmp/tmp";
      x.write();
  }

  public void requestnew(Map args) {
    String type=(String)args.get("javatype");
      if (type.startsWith("a "))
        type=type.substring(2);
      if (type.startsWith("an "))
        type=type.substring(3);
    String destpath=(String)args.get("destpath");

    out.println("<center><table align=\"center\" bgcolor=\"#80b080\"><tr><td><center><br><h2>You are creating a new "+type+/*" to "+destpath+*/"</h2>");
//      XML x=XML.oftype(type);
    XML x=getXMLfromform(args); // which will mostly be empty ;)
      HTMLRenderContext c=new HTMLRenderContext("newform");
      c.destpath=destpath;
      out.println(""+x.renderXML(c)+"</td></td></table>");
      out.println("<br>Alternatively you can "+CTW.submitdatabutton(destpath));
  }

  public void submitnew(Map args) {
    // Optimised for tree case:
    String destpath=(String)args.get("destpath");
    XML x=null;
    if (XDB.readMode==XDB.DBstorage)
      x=XDB.readtodepth(destpath,2);
    else
      x=( XML.FileType==XML.Tree ? new XML() : XML.openpath(destpath) );
    if (x==null)
      out.println("Could not find destination object "+destpath);
    else {
      XML y=getXMLfromform(args);
      if (y.findField("creationdate")!=null)
        y.setpath("creationdate",new Date());
      Log.report("Got object "+y);
      x.inside.add(y);
      if (XDB.writeMode==XDB.DBstorage) {
//        x.createpath(destpath);
//        x.id=x.path;
        x.write();
        y.newID();
        y.write();
      } else
      if (XML.FileType==XML.Tree) {
        x.createpath(destpath);
        y.write();
        if (Debugging) {
          Log.report("CTW.submitnew(): Written "+x.id+" with inside "+x.inside);
        }
      } else {
        x.write();
        if (Debugging) {
          Log.report("CTW.submitnew(): Written "+x.id+" with inside "+x.inside);
        }
      }
      out.println("New object successfully created at "+y.path+"!<br>\n");
      out.println(y.render());
    }
  }

  public void requestedit(Map args) {
    String editpath=(String)args.get("editpath");
    XML x=XML.openpath(editpath);
    if (!x.maybeeditedby(person)) {
      out.println("You do not have access to edit "+x.path);
    } else {
      out.println("<center><table align=\"center\" bgcolor=\"#e0c040\"><tr><td><center><br><h2>You are editing "+x.type+/*" to "+destpath+*/"</h2>");
      HTMLRenderContext c=new HTMLRenderContext("editform");
      c.destpath=editpath;
      out.println(""+x.renderXML(c)+"</td></td></table>");
      // out.println("<br>Alternatively you can "+CTW.submitdatabutton(destpath));
    }
  }

  public void submitedit(Map args) {
   try {
    String editpath=(String)args.get("destpath");
    String parentpath=XML.parentpath(editpath);
    String childpath=XML.childpath(editpath);
//    XML par=XML.openpath(parentpath);
    XML x=XML.openpath(editpath);
    if (!x.maybeeditedby(person)) {
      out.println("You do not have access to edit "+x.render());
    } else {
      boolean canwrite=true;
      if (JReflect.isatypeof(x,"changetheworld.CTWInput")) {
        CTWInput c=(CTWInput)x;
        if (c.author!=null) {
          Person author=(Person)c.author.follow();
          if (person==null)
            canwrite=false;
          else {
            out.println("<br>You are "+person.id+"<br>Author is "+author.id+"<br>\n");
            if (author.id.compareTo(person.id)!=0)
              canwrite=false;
          }
          if (!canwrite) {
            out.println("Sorry, "+person+" you cannot edit this object, it was written by "+author.name+"<br>");
          }
        }
      }
      if (canwrite) {
        updateXMLfromform(x,args);
        if (x.write()) {//setpath(childpath,y);
          out.println("Object successfully updated!<br>\n");
//          out.println(((RenderableXML)x.getparent()).render(new HTMLRenderContext()));
          out.println(((RenderableXML)x).render(new HTMLRenderContext()));
        } else
          out.println("Problem writing object<br>\n");
      }
    }
   } catch (Exception e) {
    out.println("CTW.submitedit(): "+e+"<br>\n");
    log+="CTW.submitedit(): "+e+"\n";
   }
  }

  public void castvote(Map args) {
    try {
    if (person==null) {
      out.println("You must "+LoginTag+" or "+SignUpTag+" before you can cast a vote.<br>");
    } else {
      String votepath=(String)args.get("votepath");
      String vote=(String)args.get("vote");
      boolean mustundo=(vote.equals("undomine"));

      XML y=XML.openpath(votepath);
/*      if (y==null)
        out.println("Didn't read "+votepath);
      else
        out.println(y.render());*/

      Voteable x=null;

//      if (JReflect.canbe(y,"changetheworld.Voteable")) {
//        out.println("A\n");
        x=(Voteable)y;
//        out.println("B\n");
//      }

/*      String dbfile=XML.getdbfile(votepath);
      String xmlpath=XML.getxmlpath(votepath);
      out.println(votepath+" "+dbfile+" "+xmlpath+"<br>");
      XML f=XML.parsefile(dbfile);
      xmlpath=JString.remove(xmlpath,f.type+".");
      Voteable x=(Voteable)f.getpath(xmlpath);*/

      // Has this person already made the vote?
      VoteRecord found=VoteRecord.getrecord(x,person.votescast);
      VoteRecord found2=VoteRecord.getrecord(person,x.voterecord);
      if (found!=null && found2!=null) {
        if (!found.equals(found2)) {
          out.println("Serious vote error!  The following do not match:<br>"+person.path+".votescast has "+found.render());
          out.println("br"+x.path+".voterecord has "+found2.render());
        }
      }
      if (found==null)
        found=found2;
      if (found==null) {
        out.println("Welcome: new vote!<br>");
      } else {
//        out.println("You previous vote "+((RenderableXML)found).render()+", has been removed.<br>");
//        out.println(""+found.getClass());
        out.println("Your previous vote "+/*((RenderableXML)found)*/found.render()+", has been removed.<br>");
        x.undovote(found.vote);
      }
      VoteRecord vmade=null;
      if (mustundo) {
//        out.println("Your vote has been removed.<br>\n");
      } else {
//      out.println("Changing "+((RenderableXML)x).render());
      x.dovote(vote);
//      out.println("Changed to "+((RenderableXML)x).render());
      vmade=new VoteRecord(((XML)x).path,person.path,((XML)x).type,vote);
      person.votescast.add(vmade);
      x.voterecord.add(vmade);
      }

//      out.println("Writing persn to "+person.path);
      person.writepath(person.path);
      // writepeople();
      x.write();
      if (vmade!=null)
        out.println("You have now made vote "+vmade.render());
/*
      f.writefile(dbfile);*/

//      out.println(""+((XML)x).type+" "+((XML)x).render());
    }
    } catch (Exception e) {
      out.println("CTW.castvote: hello"+e+" L "+e.getMessage()+" K "+e.getLocalizedMessage()+"<br>");
    }
  }

  public void delete(Map args) {
    try {
      String deletepath=(String)args.get("deletepath");
      String parentpath=XML.parentpath(deletepath);
      XML p=XML.openpath(parentpath);
      XML x=XML.openpath(deletepath);
    if (!x.maybedeletedby(person)) {
      out.println("You do not have access to delete "+x.render());
    } else {

/*    XML f=readdbase(deletepath);
    XML x=(XML)readpath(f,deletepath);*/
    if (x==null) {
      out.println("Object "+deletepath+" not found.<br>");
    } else if (p==null) {
      out.println("Parent "+parentpath+" of object not found.<br>");
    } else {
    Object a=x.getpath("author");
    boolean dont=false;
    if (a!=null) {
      Person au=(Person)((Link)a).follow();
      if (au!=null)
        if (person==null)
          dont=true;
        else if (!au.id.equals(person.id))
          dont=true;
    }
    if (dont) {
      out.println("You do not have permission to delete this object<br>");
    } else {
      boolean confirmed=false;
      Object tmp=args.get("deleteconfirm");
      String confirm=( tmp==null ? "" : (String)tmp );
      if (confirm!=null) {
        if (confirm.equals("yesplease")) {
          confirmed=true;
//          XML p=x.parent;
          if (p==null) {
            out.println("Could not find parent<br>");
          } else {
//            out.println("Removing from "+p.render()+"<br>");
            p.deleteID(x.id);
            p.write();
//            out.println("Object "+((XML)o).render()+" successfully removed.<br>\n");
            out.println("Object successfully removed.<br>\n");
            out.println(p.render(new HTMLRenderContext("browse",person)));
            //f.writefile(XML.getdbfile(deletepath));
          }
        }
      }
      if (!confirmed) {
        out.println("You have requested the deletion of object<br>"+x.render()+"<br>");
        out.println("Please confirm you wish to delete this object by clicking here: <a href=\""+CTW.Url+"?command=delete&deletepath="+deletepath+"&deleteconfirm=yesplease\">Delete</a>");
      }
    }
    }
    }
    } catch (Exception e) {
      out.println("CTW.delete: "+e+"<br>");
    }
  }

/*******************************************************/
/*      Static methods                                 */
/*******************************************************/

  // Note: I have split up the submitdata calls so that the start/end form functions may be put
  // some distance away from the actual button.  This is useful when your form is causing unwanted gaps to appear

  public static String submitdatabutton(String destpath) {
    return submitdatastart(destpath)+submitdatamiddle()+submitdataend();
  }

  public static String submitdatabutton(String destpath,String defaulttype) {
    String s="";
    s+=submitdatastart(destpath);
    s+=submitdatamiddle(defaulttype);
    s+=submitdataend();
    return s;
  }

  public static String submitdatastart(String destpath) {
    // get loses command!
    // String s="<form action=\""+CTW.Url+"?command=requestnew\" method=\"get\">\n";
    String s="<form action=\""+CTW.Url+"?command=requestnew\" method=\"post\">\n";
    s+="<input type=\"hidden\" name=\"destpath\" value=\""+destpath+"\">\n";
    return s;
  }

  public static String submitdatamiddle() {
    return submitdatamiddle("Debate");
  }
  public static String submitdatamiddle(String defaulttype) {
    String s="";
    s+="&nbsp;<input type=\"submit\" name=\"submit\" value=\"Add\">&nbsp;a new&nbsp;<select name=\"javatype\">\n";
    String[] ss=CTWServlet.simpletypes();
      for (int i=0;i<ss.length;i++) {
        String t=ss[i];
        String sel=( t.equals(defaulttype) ? " SELECTED" : "" );
//        t=( JString.isvowel(t.charAt(0)) ? "an" : "a" )+" "+t;
//        out.println("<option"+sel+">"+t+"</option>\n");
        s+="<option"+sel+">"+t+"</option>\n";
      }
    s+="</select>\n";
    return s;
  }

  public static String submitdataend() {
    String s="";
    s+="</form>\n";
    return s;
  }

}
