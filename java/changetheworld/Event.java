package changetheworld; import jlib.JString;

import java.util.Vector;
import changetheworld.XML;

public class Event extends CTWInput {
  public String title="The name of the event";
  public String url="http://official.site/blah/blah.html";
  public String date="";
  public String location="";
  public String description="";
  public Event() {
  }
  boolean isurl(String u) {
    if (u==null)
      return false;
    return (u.length()>0 && !u.equals((new Event()).url) && u.startsWith("http://"));
  }
  boolean hasurl() {
    return isurl(url);
  }
  public String urlornot(String txt) {
    String pre="";
    String post="";
    if (hasurl()) {
      pre="<a href=\""+url+"\">";
      post="</a>";
    }
    return pre+txt+post;
  }
  public String render() {
    String s="<center>";
    s+="<font size=\"+3\">"+urlornot(title)+"</font><br>";
    s+="<small><i>time:</i></small> "+date+"&nbsp;&nbsp;&nbsp;<small><i>place:</i></small> "+location+"\n";
    s+="<br><br>"+description+"<br>\n";
    return s;
  }
  public String renderlist() {
    String s="<center><font size=\"+1\">"+urltome(title)+"</font>";
    if (hasurl())
      s+=" "+( hasurl() ? "("+urlornot("official site")+")" : "" );
    s+=": "+date+" "+location+" ("+insidesize+")";
    return s;
  }
  public String render(int score) {
    String s="<tr>";
    String pre="";
    String post="";
    if (url.length()>0) {
      pre="<a href=\""+url+"\">";
      post="</a>";
    }
    s+="<td align=\"center\"><font size=\"+1\">"+pre+title+post+"</font>";
    s+="<td align=\"center\">"+date;
    s+="<td align=\"center\">"+location;
    s+="<td align=\"center\">"+score;
    String de=description;
    if (de.length()>0) {
      s+="<tr><td align=\"center\" colspan=\"4\">"+de;
    }
    return s;
  }
}