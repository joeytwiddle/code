package changetheworld; import jlib.JString;

import java.lang.String;
import java.util.Vector;
import java.io.PrintWriter;

import javax.servlet.*;
import javax.servlet.http.*;

import changetheworld.XML;
import changetheworld.CTWServlet;
import changetheworld.HTMLRenderContext;

public class Browse extends CTWServlet {

  String title() {
    return "changetheworld - Just browsing";
  }

  void respond() {

    try {

      String browsepath=req.getParameter("browsepath");
      int i=browsepath.indexOf(":");
      String dbpath=browsepath.substring(0,i);
      String xmlpath=browsepath.substring(i+1);
      xmlpath=JString.remove(xmlpath,"Database.");
      XML f=XML.parsefile(dbpath);
      XML x=(XML)f.getpath(xmlpath);
      HTMLRenderContext hrc=new HTMLRenderContext("browse",person);
      out.println(((RenderableXML)x).render(hrc));

    } catch (Exception e) {
      out.println("RequestEdit.respond: <b>"+e+"</b><br>");
    }

  }

}
