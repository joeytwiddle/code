package changetheworld; import jlib.JString;

import java.lang.String;
import java.util.Vector;

public class Voteable extends CTWInput {
  public Vector voterecord=new Vector();
  public Link issue=null;
  public void dovote(String vote) {
  }
  public void undovote(String vote) {
  }
  public String renderwithissue(boolean allowcast) {
    XML x=( issue==null ? null : issue.follow() );
    if (x==null)
      return "(could not find issue!)";
    else
      return x.renderlink();
  }
  public String render(boolean allowcast) {
    return "Voteable:render(bool) should be overwritten";
  }
  public String renderlist(boolean allowcast) {
    return "Voteable:renderlist(bool) should be overwritten";
  }
}