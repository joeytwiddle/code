package changetheworld; import jlib.JString;

import java.lang.String;
import java.util.Vector;
import java.io.PrintWriter;

import javax.servlet.*;
import javax.servlet.http.*;

import changetheworld.XML;
import changetheworld.CTWServlet;
import changetheworld.HTMLRenderContext;

public class RequestNew extends CTWServlet {

  Vector ss;

  String title() {
    return "Submitting new data to change the world";
  }
  String backgroundcolour() { return "80B080"; }

  void respond() {

    try {

      String type=req.getParameter("javatype");
      if (type.startsWith("a "))
        type=type.substring(2);
      if (type.startsWith("an "))
        type=type.substring(3);
      String destpath=req.getParameter("destpath");

      out.println("<center><h2>Submit a new "+type+/*" to "+destpath+*/"</h2>");

      XML x=XML.oftype(type);
      HTMLRenderContext c=new HTMLRenderContext("newform");
      c.destpath=destpath;
      out.println(""+x.renderXML(c));

      out.println("<form action=\"/servlet/changetheworld.RequestNew\" method=\"post\">\n");
      out.println("<input type=\"hidden\" name=\"destpath\" value=\""+destpath+"\">\n");
      out.println("Would you like to submit &nbsp;<select NAME=\"javatype\">\n");
      String[] ss=CTWServlet.simpletypes();
      for (int i=0;i<ss.length;i++) {
        String t=ss[i];
        String sel=( t.equals(type) ? " SELECTED" : "");
        t=( JString.isvowel(t.charAt(0)) ? "an" : "a" )+" "+t;
        out.println("<option"+sel+">"+t+"</option>\n");
      }
      out.println("</select>instead?&nbsp;<input TYPE=SUBMIT NAME=\"sub\" VALUE=\"Go!\">\n</form>\n");

    } catch (Exception e) {
      out.println("RequestNew.respond: <b>"+e+"</b><br>");
    }

  }

}
