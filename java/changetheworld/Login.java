package changetheworld; import jlib.JString;

import java.lang.String;
import java.util.Vector;
import java.io.PrintWriter;

import javax.servlet.*;
import javax.servlet.http.*;

import changetheworld.JVector;
import changetheworld.SatisfiesAll;
import changetheworld.XML;
import changetheworld.CTWServlet;

public class Login extends CTWServlet {

  String title() {
    return "Login";
  }

  void respond() {
   try {
    people=XML.openpath("people"); // getpeople();
    String name=req.getParameter("name");
    String pass=req.getParameter("pass");
    if (name==null || pass==null) {
      out.println("Some problem with CGI parameters<br>");
    }
    SatisfiesAll sa=new SatisfiesAll();
    sa.add(new ObjectContainsField("name",name));
    sa.add(new ObjectContainsField("pass",pass));
    out.println(people.render());
    out.println(sessions.render());
    Object tmp=JVector.getfirst(sa,people.inside);
    person=( tmp==null ? null : (Person)tmp );
    if (person==null) {
      out.println("Login failed - couldn't find person called "+name+" or password wrong.<br>");
    } else {
      Link link=new Link(person.path);
      session.link=link;
      // writesessions(); Automatically done by CTWServlet
      out.println("Login suceeded.<br>");
      out.println("<a href=\"/servlet/changetheworld.RequestEdit?editpath="+person.path+"\">Click here</a> to edit your data<br>");
    }
   } catch (Exception e) {
    out.println("Login: <b>"+e+"</b><br>");
   }
  }

}
