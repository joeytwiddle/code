package changetheworld; import jlib.JString;

import java.lang.String;
import java.util.Vector;
import java.io.PrintWriter;

import javax.servlet.*;
import javax.servlet.http.*;

import changetheworld.XML;
import changetheworld.CTWServlet;
import changetheworld.HTMLRenderContext;

public class RequestEdit extends CTWServlet {

  Vector ss;

  String title() {
    return "Editing data";
  }
  String backgroundcolour() { return "E0C040"; }

  void respond() {

    try {

      String editpath=req.getParameter("editpath");
      int i=editpath.indexOf(":");
      String dbfile=editpath.substring(0,i);
      String xmlpath=editpath.substring(i+1);
      xmlpath=JString.remove(xmlpath,"Database.");
      XML f=XML.parsefile(dbfile);
      XML x=(XML)f.getpath(xmlpath);
      out.println("<h1>Editing data</h1>\n");
      HTMLRenderContext c=new HTMLRenderContext("editform");
      c.destpath=editpath;
      out.println(x.render(c));

    } catch (Exception e) {
      out.println("RequestEdit.respond: <b>"+e+"</b><br>");
    }

  }

}
