package changetheworld; import jlib.JString;

import java.lang.String;
import java.util.Vector;
import java.io.PrintWriter;

import javax.servlet.*;
import javax.servlet.http.*;

import changetheworld.XML;
import changetheworld.CTWServlet;
import changetheworld.ScoredList;

public class Events extends CTWServlet {

  Vector ss;

  String title() {
    return "Events for search \""+req.getParameter("searchtext")+"\"";
  }

  double scorefor(Event event) {
    if (ss==null)
      return 0;
    double score=(double)0.0;
    String title=JString.tolower(event.title);
    String location=JString.tolower(event.location);
    String description=JString.tolower(event.description);
    for (int i=0;i<ss.size();i++) {
      if (title.indexOf(JString.tolower((String)ss.get(i)))>-1)
        score+=1.0;
      if (location.indexOf(JString.tolower((String)ss.get(i)))>-1)
        score+=1.0;
      if (description.indexOf(JString.tolower((String)ss.get(i)))>-1)
        score+=1.0;
    }
    return score;
  }

  void respond() {

    try {

      String s=req.getParameter("searchtext");

      out.println("<center><h1>Events found for search <i>"+s+"</i></h1></center>");

      XML events=XML.parsefile(DataPath+"events");

      String addressreport="";
      if (person!=null) {
        String add=" "+person.location;
        s=s+add;
        addressreport="<center>I have included your address \""+add+"\" in the search.<br>";
      }

      if (s.length()==0)
        ss=null;
      else
        ss=JString.splitv(s," ");

      ScoredList sl=new ScoredList();

      for (int i=0;i<events.inside.size();i++)
        sl.add(events.inside.get(i),scorefor((Event)events.inside.get(i)));

      out.println("<table width=\"100%\">");
      // out.println("<tr><th>Event<th>Date<th>Place<th>Score");
      out.println("<tr><td align=\"center\"><i>Event<td align=\"center\"><i>Date<td align=\"center\"><i>Place<td><i>Score");

      for (int i=0;(i<100) && (i<sl.size());i++) {
        Event event=(Event)sl.get(i);
        double score=sl.getscore(i);
        out.println(event.render((int)score));
      }

      out.println("</table>");

      Event e=(Event)sl.get(1);
      out.println(e.render(new HTMLRenderContext()));
      if (person!=null)
        out.println(person.render(new HTMLRenderContext()));
      if (session!=null)
        out.println(session.render(new HTMLRenderContext()));


      out.println("<p align=\"right\">Total events in database: "+events.insidesize+"</p>");
      out.println(addressreport);

    } catch (Exception e) {
      out.println("Events.respond: <b>"+e+"</b><br>");
    }

  }

}
