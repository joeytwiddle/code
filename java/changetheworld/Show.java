package changetheworld; import jlib.JString;

import java.lang.String;
import java.util.Vector;
import java.io.PrintWriter;

import javax.servlet.*;
import javax.servlet.http.*;

import changetheworld.XML;
import changetheworld.CTWServlet;
import changetheworld.HTMLRenderContext;

public class Show extends CTWServlet {

  String title() {
    return "changetheworld - Just browsing";
  }

  void respond() {

    try {

      String showdbase=req.getParameter("showdbase");
      String dbfile=DataPath+showdbase;
      XML f=XML.parsefile(dbfile);

      out.println("<h1>Showing "+showdbase+"</h1>");

      ScoredList sl=new ScoredList();
      for (int i=0;i<f.inside.size();i++)
        sl.add(f.inside.get(i),((XML)f.inside.get(i)).score());

      for (int i=0;(i<100) && (i<sl.size());i++) {
        XML x=(XML)sl.get(i);
        double score=sl.getscore(i);
        out.println(x.render(new HTMLRenderContext("listing")));
        out.println("<br>");
      }

    } catch (Exception e) {
      out.println("RequestEdit.respond: <b>"+e+"</b><br>");
    }

  }

}
