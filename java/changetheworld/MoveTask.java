package changetheworld; import jlib.JString;

import java.util.*;

public class MoveTask extends Task {
  public String srcpath="";
  private String urltodeleteme(String text) {
    return urltodeleteme(text,"");
  }
  private String urltodeleteme(String text,String extraargs) {
    return "<a href=\""+CTW.Url+"?command=delete&deletepath="+path+"&deleteconfirm=yesplease"+extraargs+"\">"+text+"</a>";
  }
  public MoveTask() {
  }
  public MoveTask(String src) {
    srcpath=src;
  }
  public String render(HTMLRenderContext c) {
    if (c.inside!=null)
    if (c.inside.length()>0) {
      XML x=XML.openpath(srcpath);
//      log+="Got x="+x+"\n";
      if (x==null)
        return "Out of date task ("+urltodeleteme("Remove")+")"; //,"&endupcommand="+cgiencode("display page="+c.where))+")";
      String a="<a href=\""+urltodome()+"&movesrc="+srcpath+"&movedest="+c.inside+"\">";
      String s="";
      s+=a+"Move </a><i>"+x.renderlink()+"</i>"+a+" to here</a>";
      s+=" <small>("+urltodeleteme("Cancel")/*,"&endupcommand="+cgiencode("display page="+c.where))*/+")</small>\n";
      return s;
    }
    return "";
  }
  public boolean complete(Map args,CTW ctw) {
    try {
      String movedest=(String)args.get("movedest");
      ctw.out.println(ctw.docommand("move movesrc="+srcpath+" movedest="+movedest));
      return true;
    } catch (Exception e) {
      ctw.out.println("MoveTask.complete(): "+e+"<br>");
      XML.log+="MoveTask.complete(): "+e+"\n";
      return false;
    }
  }
}