package changetheworld; import jlib.JString;

import java.util.Vector;
import changetheworld.XML;

public class SolidarityIssue extends Issue {
  public SolidarityCount count=new SolidarityCount();
  public SolidarityIssue() {
  }
  public String render() {
    String s="";
    s+="<h2>"+title+"</h2><font size=\"-1\"><i>"+details+"</i></font><br><br>"+count.render()+"\n";
//    s+=renderinside();
    return s;
  }
  public String renderlist() {
    return "<table width=\"100%\"><tr><td><b>"+urltome(title)+"</b></td><td>"+details+"</td><td>"+count.renderlist()+"</td></tr></table>";
  }
}


