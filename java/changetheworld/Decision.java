package changetheworld; import jlib.JString;

import java.util.Vector;
import changetheworld.XML;

public class Decision extends CTWInput {
  public String title="Yes or no question, eg: Shall we do this?";
  public String description="Explain more about this question\n";
  public Vote vote=new Vote();
  public Decision() {
  }
  public String render() {
    String s="";
    s+="<center><b>"+urltome(title)+"</b><br>\n";
    s+="<br><i>"+description+"</i><br><br>";
    s+=vote.render()+"<br>\n";
    return s;
  }
  public String renderlist() {
    return "<b>"+urltome(title)+"</b><br>\n"+vote.renderlist();
  }

}