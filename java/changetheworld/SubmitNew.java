package changetheworld; import jlib.JString;

import java.lang.String;
import java.util.Vector;
import java.io.PrintWriter;
import java.lang.reflect.*;
import java.text.SimpleDateFormat;
import java.util.Date;

import javax.servlet.*;
import javax.servlet.http.*;

import changetheworld.XML;
import changetheworld.CTWServlet;

public class SubmitNew extends CTWServlet {

  Vector ss;

  String title() {
    return "Submitting new data to change the world";
  }

  void respond() {

    try {

      out.println("Hello<br>\n");

      String type=req.getParameter("javatype");
//      out.println("Hello "+type+"<br>");
      String destpath=req.getParameter("destpath");

      String dbfile,xmlpath="";
      int i=destpath.indexOf(":");
      if (i>-1) {
        dbfile=destpath.substring(0,i);
        xmlpath=destpath.substring(i+1);
        if (xmlpath.startsWith("Database."))
          xmlpath=xmlpath.substring(9);
      } else {
        dbfile=destpath;
      }
//      out.println("DB: "+dbfile+"<br>");
      XML f=XML.parsefile(dbfile);
//      out.println("Got file "+f+"<br>");
      XML d=(XML)f.getpath(xmlpath);
      out.println("Destination "+d.render()+"<br>");
      XML x=getXMLfromform();
      if (x.findField("creationdate")!=null)
        x.setpath("creationdate",new Date());
      d.inside.add(x);
      out.println("Added object "+x.render()+"<br>");
//      out.println("Destination "+d.render()+"<br>");
      XML e=(XML)f.getpath(xmlpath);
//      out.println("Original destination "+e.render()+"<br>");
      boolean worked=f.writefile(dbfile);
      if (worked)
        out.println("Written file "+dbfile+" "+f.render()+XML.encode(f.toString()));
      else
        out.println("Failed to write file "+dbfile+" "+f.render()+XML.encode(f.toString()));

    } catch (Exception e) {
      out.println("SubmitNew.respond: <b>"+e+"</b><br>");
    }

  }

}
