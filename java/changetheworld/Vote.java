package changetheworld; import jlib.JString;

import java.util.Vector;
import changetheworld.XML;
import changetheworld.Link;

// Vote on a real-world decision
// Vote to remove an object

public class Vote extends Voteable {
  public int votesFor=0;
  public int votesAgainst=0;
  public int votesAbstain=0;
  public Vote() {
  }
  public Vote(XML src) {
    issue=new Link(src.id);
  }
  private String castvote(String s,int c,boolean castlink,String col) {
    String pre="<a href=\""+CTW.ServletUrl+"CTW?command=castvote&votepath="+path+"&vote="+s+"\">";
    String post="</a>";
    if (!castlink) {
      pre=post="";
    }
    return pre+s+post+": <font color=\"#"+col+"\"><b>"+c+"</b></font>";
//    return "<td bgcolor=\"#"+col+"\">"+pre+s+post+":</td><td>"+c+"</td>";
  }
  public String rendervote(boolean allowcast) {
    return castvote("For",votesFor,allowcast,"008000")+" <font size=\"-2\">"+castvote("Abstain",votesAbstain,allowcast,"A0A020")+"</font> "+castvote("Against",votesAgainst,allowcast,"F02020");
  }
  public String render(boolean allowcast) {
    String s="";
    s+=rendervote(allowcast);
    return s;
  }
  public String render() {
    return render(true);
  }
  public String renderlist() {
    return rendervote(false);
//    return "<table align=\"center\"><tr>"+castvote("For",votesFor,c,"80FF80")+" "+castvote("Abstain",votesAbstain,c,"FFFF80")+" "+castvote("Against",votesAgainst,c,"FF8080")+"</tr></table>\n";
  }
  public String renderlist(boolean allowcast) {
  	return render(allowcast);
  }
  public void dovote(String vote) {
    if (vote.compareToIgnoreCase("for")==0)
      votesFor++;
    if (vote.compareToIgnoreCase("abstain")==0)
      votesAbstain++;
    if (vote.compareToIgnoreCase("against")==0)
      votesAgainst++;
  }
  public void undovote(String vote) {
    if (vote.compareToIgnoreCase("for")==0)
      votesFor--;
    if (vote.compareToIgnoreCase("abstain")==0)
      votesAbstain--;
    if (vote.compareToIgnoreCase("against")==0)
      votesAgainst--;
  }
}