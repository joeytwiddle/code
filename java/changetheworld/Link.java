package changetheworld; import jlib.JString;

import changetheworld.CTW;
import java.util.Vector;
import changetheworld.XML;

public class Link extends CTWInput {
  public String title="Describe where the link will take you (or leave)";
  public String seekpath="http://www.web.site/page/path or just the path of an XML object";
  public boolean transparent=true;
  public Link() {
  }
  Link(String sp) {
    seekpath=sp;
  }
  public boolean ownconsistencycheck() {
    if (title.equals((new Link()).title) || title.length()==0) {
      XML x=follow();
      if (!JReflect.canbe(x,"changetheworld.CTWInput")) {
        title="Link to \""+seekpath+"\" does not exist!";
        return true;
      }
      else {
        title=((CTWInput)x).gettitle();
        return true;
      }
    }
    return false;
  }
  public boolean isweblink() {
    return seekpath.startsWith("http://");
  }
  public XML follow() {
    XML x=null;
  //  try {
      x=openpath(seekpath);
  //  } catch (Exception e) {
  //    log+="Link.follow() sp="+seekpath+" : "+e;
  //  }
    return x;
/*    if (seekpath.endsWith("/")) {
      String tmp=seekpath.substring(0,seekpath.length()-1);
      int i=tmp.lastIndexOf("/");
      int j=tmp.lastIndexOf(".");
      String seekid=( i==j && j==-1 ? tmp :
                      i>j ? tmp.substring(i+1) : tmp.substring(j+1) );
      XML x=XML.findID(seekid);
      if (x==null)
        System.out.println("Link \n"+toString("  ")+"\nfailed to return object "+seekid);
      return x;
    }
    return null;*/
  }
  public String render() {
    String s="";
    try {
    if (isweblink()) {
      s+="<font size=\"+2\">Link to <a href=\""+seekpath+"\">"+title+"</a><font><br><br>\n";
    } else {
      s+=title+"<br><br>\n";
      XML f=follow();
      if (f==null)
        s+="Broken link "+seekpath;
      else
        s+="<font size=\"+2\">Link to "+f.render(new HTMLRenderContext("link"))+"</font><br>\n";
    }
    } catch (Exception e) {
      return "Link.render(): "+e;
    }
    return s;
//    return "<a href=\""+CTW.ServletUrl+"ExamineObject?objid="+seekid+"\">hello</a>";
  }
  public String renderlist() {
    String s="";
    try {
//    s+="<small><i>"+urltome("link")+":</i></small>&nbsp;";
    s+="<table><tr><td align=\"center\">";
    if (isweblink()) {
      s+="<a href=\""+seekpath+"\">"+title+"</a>\n";
    } else {
      XML f=follow();
      if (f==null)
        s+="Broken link "+seekpath;
      else {
        if (transparent)
          s+=f.render(new HTMLRenderContext("list"));
        else
          s+=((RenderableXML)f).urltome(title);
      }
/*      if (JReflect.canbe(f,"RenderableXML"))
        return ((RenderableXML)f).renderlist();
      else
        return "Link.renderlist(): Not renderable: "+f.type;
*/
//    return "<a href=\""+CTW.ServletUrl+"ExamineObject?objid="+seekid+"\">hello</a>";
    }
    s+="</td><td><small><i>&nbsp;("+urltome("link")+")</i></small></td></tr></table>";
    } catch (Exception e) {
      return "Link.renderlist(): "+e;
    }
    return s;
  }
/*  public String toString(String ind) {
    return ind+"link";
  }*/
}

