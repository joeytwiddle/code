package changetheworld; import jlib.JString;

import java.util.Vector;
import changetheworld.XML;
import changetheworld.CTWInput;

public class Debate extends CTWInput {
  public String title="Short description";
  public String content="Explain more what this debate should be about, or give an introduction.";
  public Debate() {
//    title="pose a question or opinion";
  }
  public String render() {
    String s="";
    s+="<h3>"+urltome(title)+"</h3>"+content+"<br>";
    return s;
  }
  public String renderlist() {
    return urltome(title)+" ("+insidesize+")<br><small>"+clip(content)+"</small>";
  }
}
