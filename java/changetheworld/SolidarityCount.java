package changetheworld; import jlib.JString;

import java.util.Vector;
import changetheworld.XML;
import changetheworld.Link;

public class SolidarityCount extends Voteable {
  public int count=0;
  public Link issue=null;
  public String agreement="I agree to...";
  public SolidarityCount() {
  }
  public SolidarityCount(XML src) {
    issue=new Link(src.id);
  }
/*  public String render(HTMLRenderContext c) { // overwrite so this object will not be wrapped
    String s="";
//    s+="<a href=\""+CTW.ServletUrl+"CTW?command=castvote&votepath="+path+"&vote=solid\">"+agreement+"</a>";
    s+=urltome(agreement);
    s+=( c.list ? "&nbsp;" : "<br>" );
    s+="Solidarity count: "+count;
    s+="<br>\n";
    return s;
  }*/
  public String render() {
    return render(true);
  }
  public String render(boolean allowcast) {
    String s="";
    s+="<font size=\"+1\">"+agreement+"</font>";
    s+="<br><br>\n";
    s+="Solidarity count: "+count;
    s+="<br><br>\n";
    if (allowcast) {
    s+="<table><tr><td bgcolor=\"#80ff80\"><a href=\""+CTW.ServletUrl+"CTW?command=castvote&votepath="+path+"&vote=solid\">Join count</a></td>";
    s+="<td bgcolor=\"#ff6060\"><a href=\""+CTW.ServletUrl+"CTW?command=castvote&votepath="+path+"&vote=undomine\">Remove me from count</a></td></tr></table>";
    s+="\n";
    }
    return s;
  }
  public String renderlist() {
    return renderlist(true);
  }
  public String renderlist(boolean allowcast) {
    String s="";
//    s+="<a href=\""+CTW.ServletUrl+"CTW?command=castvote&votepath="+path+"&vote=solid\">"+agreement+"</a>";
    s+=urltome(agreement);
    s+="&nbsp;";
    s+="Solidarity count: "+count;
    return s;
  }
  public void dovote(String vote) {
    count++;
  }
  public void undovote(String vote) {
    count--;
  }
}