package changetheworld; import jlib.JString;

import changetheworld.CTWInput;

public class Place extends CTWInput {
  public String place="name of location";
  public Place() {
  }
  public String render() {
    String s="";
    s+="<h3>"+place+"</h3>\n";
    s+="Search for action in this place (under construction)<br>\n";
    s+="Search for discussion on this place (under construction)<br>\n";
    return s;
  }
  public String renderlist() {
    return urltome(place);
  }
  public String renderlink() {
    return urltome(place);
  }
}