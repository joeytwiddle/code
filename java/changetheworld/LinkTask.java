package changetheworld; import jlib.JString;

import java.util.*;

public class LinkTask extends Task {
  public String srcpath="";
  private String urltodeleteme(String text) {
    return urltodeleteme(text,"");
  }
  private String urltodeleteme(String text,String extraargs) {
    return "<a href=\""+CTW.Url+"?command=delete&deletepath="+path+"&deleteconfirm=yesplease"+extraargs+"\">"+text+"</a>";
  }
  public LinkTask() {
  }
  public LinkTask(String src) {
    srcpath=src;
  }
  public String render(HTMLRenderContext c) {
    if (c.inside!=null)
    if (c.inside.length()>0) {
      XML x=XDB.readtodepth(srcpath,0);
//      log+="Got x="+x+"\n";
      if (x==null)
        return "Out of date task ("+urltodeleteme("Remove")+")"; //,"&endupcommand="+cgiencode("display page="+c.where))+")";
      String a="<a href=\""+urltodome()+"&linksrc="+srcpath+"&linkdest="+c.inside+"\">";
      String s="";
      s+=a+"Create link to </a><i>"+x.renderlink()+"</i>"+a+" here</a>";
      s+=" <small>("+urltodeleteme("Cancel")/*,"&endupcommand="+cgiencode("display page="+c.where))*/+")</small>\n";
      return s;
    }
    return "";
  }
  public boolean complete(Map args,CTW ctw) {
    try {
      String linkdest=(String)args.get("linkdest");
      XML x=XDB.readtodepth(srcpath,0);
      ctw.out.println(ctw.docommand("submitnew javatype=Link seekpath="+paramencode(srcpath)+" destpath="+paramencode(linkdest)+" title="+XML.paramencode(((CTWInput)x).gettitle())));
      return true;
    } catch (Exception e) {
      ctw.out.println("LinkTask.complete(): "+e+"<br>");
      XML.log+="LinkTask.complete(): "+e+"\n";
      return false;
    }
  }
}