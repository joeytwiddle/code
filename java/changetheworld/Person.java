package changetheworld; import jlib.JString;

import java.util.Vector;
import changetheworld.XML;

public class Person extends CTWInput {

  public String name="your user name";
  public String pass="";
  public String email="";
  public boolean emailpublic=false;
  public String location="";
  public String phone="";
  public boolean locationpublic=false;
  public String skills="How can you help us change the world?\n";
  public boolean skillspublic=false;
  public String page="http://www.your.site/your/page";

  public Vector votescast=new Vector();
  public XML tasks=new XML(); // used an XML so tasks have a path

  public Person() {
  }

  public boolean maybeeditedby(Person p) {
    if (p==null)
      return false;
//    XML.log+="Path="+path+"<br>\np.Path="+p.path+"<br>\n";
    return (p.path.equals(path));
  }

  boolean hasurl() {
    return (page.length()>0 && !page.equals((new Person()).page));
  }

  public String urlornot(String txt) {
    String pre="";
    String post="";
    if (hasurl()) {
      pre="<a href=\""+page+"\">";
      post="</a>";
    }
    return pre+txt+post;
  }

  public String renderlist() {
    return urltome(name)+" "+( skillspublic ? skills : "" )+" "
           +( hasurl() ? "("+urlornot("<i>homepage</i>")+")" : "" );
  }

  public String render() {
    String s="";
    s+="<font size=\"+2\">"+name+"</font>";
    if (hasurl())
      s+=" ("+urlornot("Homepage")+")";
    s+="<br><br>\n";
    if (emailpublic)
      s+="Email: <a href=\"mailto:"+email+"\">"+email+"</a><br><br>\n";
    if (locationpublic) {
      s+="Location: "+location+"<br><br>\n";
      s+="Phone: "+phone+"<br><br>\n";
    }
    if (skillspublic)
      s+="Skills:<br><blockquote>"+skills+"</blockquote>\n";
    return s;
  }

  public String rendertasks(HTMLRenderContext context) {
    String s="";
    for (int i=0;i<tasks.inside.size();i++) {
      Task t=(Task)tasks.inside.get(i);
      String r=t.render(context);
      if (r.length()>0)
        s+="&nbsp;"+r+"<br>"; // &nbsp;&nbsp;&nbsp;";
    }
    return s;
  }

  public String gettitle() {
    return name;
  }

}
