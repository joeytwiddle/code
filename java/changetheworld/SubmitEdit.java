package changetheworld; import jlib.JString;

import java.lang.String;
import java.util.Vector;
import java.io.PrintWriter;

import javax.servlet.*;
import javax.servlet.http.*;

import changetheworld.XML;
import changetheworld.CTWServlet;

public class SubmitEdit extends CTWServlet {

  Vector ss;

  String title() {
    return "Submitting new data to change the world";
  }

  void respond() {

    try {

      // *** Need to check authorised person!

      String editpath=req.getParameter("destpath");
      int i=editpath.indexOf(":");
      String dbfile=editpath.substring(0,i);
      String xmlpath=editpath.substring(i+1);
      xmlpath=JString.remove(xmlpath,"Database.");
      XML f=XML.parsefile(dbfile);
      XML x=(XML)f.getpath(xmlpath);
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
        updateXMLfromform(x);
        f.writefile(dbfile);
      }

    } catch (Exception e) {
      out.println("SubmitEdit.respond: <b>"+e+"</b><br>");
    }

  }

}
