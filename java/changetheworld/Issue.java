package changetheworld; import jlib.JString;

import java.util.Vector;
import changetheworld.XML;

public class Issue extends CTWInput {
  public String title="The issue you wish to raise";
  public String details="Introduce the issue.  Describe the problems you wish to resolve.";
  public Issue() {
  }
  public String render() {
    String s="";
    s+="<h2>"+title+"</h2><font size=\"-1\"><i>"+details+"</i></font><br>";
//    s+=renderinside();
    return s;
  }
  public String renderlist() {
    return "<table width=\"100%\"><tr><td><b>"+urltome(title)+"</b></td><td><font size=\"-1\"><i>"+clip(details)+"</i></font></td></tr></table>";
  }
}


