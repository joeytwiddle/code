package changetheworld; import jlib.JString;

import java.io.File;
import changetheworld.CTW;
import java.util.Vector;
import changetheworld.XML;
import java.util.Date;
import java.text.SimpleDateFormat;
import java.net.*;

public class FastLink extends Link {
  public Date lastrendered=null;
  public String rendered=null;
  public int rendertime=0;
  public FastLink() {
  }
  FastLink(String sp) {
    super(sp);
  }
  public void rerender() {
    lastrendered=new Date();
    XML x=follow();
    if (x==null)
      rendered="Broken link";
    else {
      Date st=new Date();
      rendered=URLEncoder.encode(x.render());
      Date en=new Date();
      rendertime=(int)(en.getTime()-st.getTime());
    }
    write();
  }
  public String render(HTMLRenderContext context) {
    // Do not overwrite
    // This render method will render the object
    // and wrap it with editing options and information
      String s="";
  try {
    if (context.list)
      return renderlist();
    if (context.link)
      return renderlink();
    else if (context.form)
      return renderunknown(context);
    else {
      if (context.full) {

        s+="<table width=\"100%\">";

        s+=renderpath(context);

        s+="<tr><td><br>";
        s+=render();
        s+="<br></td></tr>\n";

//        s+="<tr><td width=\"100%\" bgcolor=\"#ffa000\"><center>";
//        s+=CTW.submitdatastart(path);
//          s+="<table width=\"100%\" bgcolor=\"#ffffff\" cellpadding=\"10\"><tr><td><center>";
//          s+=renderinside();
//          s+="</td></tr></table>\n";
//          s+=menuaddtasksearch(context);
//        s+=CTW.submitdatamiddle();
/*    if (context.person!=null) {
      context.inside=path;
//      s+="Got person, path="+c.inside;
      s+=context.person.rendertasks(context);
      context.inside="";
    }*/
//        s+="<a href=\""+CTWServlet.ServletURL+"RequestNew?javatype=Debate&destpath="+path+"\">Add new data</a><br>\n";
//        s+="</td></tr>\n";
        s+=JString.replace(rendereditoptions(context),"<br>","&nbsp;");
        s+="</table>\n";
//        s+=CTW.submitdataend();
      } else
        s+=render();
      return s;
    }
  } catch (Exception e) {
    return s+"CTWInput.render(c): "+e;
  }
  }
  public boolean outofdate() {
    if ( rendered==null || lastrendered==null)
      return true;
    try {
      if (lastrendered.getTime()<new Date().getTime()-1*24*60*60*1000)
        return true;
    } catch (Exception e) {
      log+="FastLink.render() day "+e;
    }
    try {
      if (lastrendered.getTime()<(new File(CTW.TreePath+seekpath,XML.XmlNodeFileName)).lastModified() )
        return true;
    } catch (Exception e) {
      log+="FastLink.render() filemod "+e;
    }
    return false;
  }
  public String howlong() {
    return JString.time(rendertime);
  }
  public String render() {
    try {
    String report=""; // FastLink("+lastrendered+"): ";
    if (isweblink()) {
      return report+"<a href=\""+path+"\">"+path+"</a>";
    }
//    rerender();
    if (outofdate()) {
      report+="Out of date, so re-rendering...<br>\n";
//      if (lastrendered!=null)
//        report+="Last rendered: "+(new SimpleDateFormat()).format(lastrendered)+"<br>\n";
//      report+="Now: "+(new SimpleDateFormat()).format(new Date())+"<br>\n";
//      report+="Object's last modification date: "+(new SimpleDateFormat()).format(new Date(new File(CTW.TreePath+seekpath,XML.XmlNodeFileName).lastModified()))+"<br>\n";
      rerender();
      XDB.writenode(this);
    } else {
      report+="Showing you fast version generated "+(new SimpleDateFormat()).format(lastrendered);
      report+="&nbsp;&nbsp;&nbsp;<a href=\""+CTW.Url+"?command=render&path="+path+"&rerender=true\">Generate up to date version</a> ("+howlong()+")";
      report+="<br>\n";
    }
    return "<small><i>"+report+"</i></small>"+"<br>\n"+URLDecoder.decode(rendered);
    } catch (Exception e) {
      log+="FastLink.render() "+e;
      return "";
    }
  }
  public String renderlist() {
    return urltome(gettitle())+( outofdate() ? " <i>(will take "+howlong()+")</i>" : "" );
  }
  public String renderlink() {
    return renderlist();
  }
}
