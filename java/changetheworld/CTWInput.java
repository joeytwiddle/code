package changetheworld; import jlib.JString;

import java.util.Vector;
import java.util.Date;
import java.lang.reflect.*;
import java.text.SimpleDateFormat;

import changetheworld.XML;

import jlib.Log;

public class CTWInput extends RenderableXML {
//  public String title="";
  public Date creationdate=null;
  public Link author=null;
  public int visits=0;

  public boolean maybeeditedby(Person p) {
    if (author==null)
      return true;
    XML x=author.follow();
    if (x==null)
      return true;
    if (!JReflect.canbe(x,"changetheworld.Person")) {
      log+="CTWInput:alloweditingby(): My author is not a person!";
      return false;
    }
    if (p==null)
      return false;
    return (x.path.equals(p.path));
  }

  public boolean maybedeletedby(Person p) {
  	return maybeeditedby(p) && insidemaybedeletedby(p);
  }

  public String renderXML(HTMLRenderContext c) {
  	return render(c);
  }
  public String renderlink() {
    return urltome(gettitle());
  }
  public String renderlist() {
    return urltome(gettitle());
  }

  public String renderpath(HTMLRenderContext context) {
    String s="";
        s+="<tr><td bgcolor=\"#60A060\" valign=\"center\">";
//      s+="<tr><td bgcolor=\"#2060B0\" valign=\"center\">";

        String tmptype=JString.toupper(type.charAt(0))+type.substring(1);
        s+="<p align=\"right\"><i>";
        if (parent==null) {
          String pp=parentpath(path);
//          parent=openpath(pp);
          parent=XDB.readtodepth(pp,0);
        }
        if (parent!=null) {
          if (JReflect.canbe(parent,"changetheworld.RenderableXML")) {
            RenderableXML c=(RenderableXML)parent;
//            s+="<font color=\"#FFFFFF\">"+c.urltome(c.gettitle()+":")+"</font>";
            s+="<font color=\"#FFFFFF\">"+c.renderlink()+":"+"</font>";
          } else if (JReflect.canbe(parent,"changetheworld.XML")) {
            s+="<font color=\"#FFFFFF\">"+((XML)parent).type+":</font>";
          }
        }
        s+="<font size=\"+1\" color=\"#FFFFFF\">&nbsp;&nbsp;"+tmptype+"&nbsp;&nbsp;</font>";
        s+="</i></p></td>\n";
    return s;
  }

  public String rendereditoptions(HTMLRenderContext context) {
    String s="";
        s+="<td rowspan=\"3\" width=\"2%\" align=\"center\" valign=\"top\" bgcolor=\"#ff0000\"><font color=\"#FFFFFF\">\n";
        s+="<font size=\"-1\">";
        boolean editpriv=maybeeditedby(context.person); // (author==null);
        String name="unknown";
        if (author!=null) {
          Object tmp=author.follow();
          if (tmp==null) {
            name="broken link";
          } else {
            Person p=(Person)tmp;
            name=p.urltome(p.name);
            if (context.person!=null)
              if (p.path.equals(context.person.path)) {
                editpriv=true;
                name=context.person.urltome("You!");
              }
          }
        }
        s+="<br>&nbsp;Author:&nbsp;<br>&nbsp;"+name+"&nbsp;<br><br>\n";
        if (creationdate!=null) {
          String day=(new SimpleDateFormat("d/M/yy")).format(creationdate);
          String today=(new SimpleDateFormat("d/M/yy")).format(new Date());
          if (day==today) {
            String time=(new SimpleDateFormat("h:mma")).format(creationdate).toLowerCase();
            s+="&nbsp;Created&nbsp; today at "+time;
          } else {
            String main=(new SimpleDateFormat("E d/M/yy")).format(creationdate);
            String time=(new SimpleDateFormat("h:mma")).format(creationdate).toLowerCase();
            s+="&nbsp;Created&nbsp; on "+main+" at "+time;
          }
          s+="<br><br>\n";
        }
        s+="&nbsp;Visits:&nbsp; "+visits+"<br><br>";
        if (editpriv) {
          s+="<a href=\""+CTW.Url+"?command=requestedit&editpath="+path+"\">Edit</a><br>\n";
          s+="<a href=\""+CTW.Url+"?command=delete&deletepath="+path+"\">Delete</a><br>\n";
          s+="<a href=\""+CTW.Url+"?command=move&movesrc="+path+"\">Move</a><br>\n";
          s+="<a href=\""+CTW.Url+"?command=link&linksrc="+path+"\">Link</a><br>\n";
          s+="<br>\n";
        }
        s+="</td></tr>\n";
    return s;
  }

  public String menuaddtasksearch(HTMLRenderContext c) {
    String s="";
    s+="<table width=\"70%\" align=\"center\" bgcolor=\"#ffa000\"><tr><td align=\"center\">";
    s+=CTW.submitdatastart(path);
    Object tmp=getpath("defaulttype");
    String defaulttype=( tmp==null ? "Debate" : (String)tmp );
    s+=CTW.submitdatamiddle(defaulttype);
    s+="</td>\n";
    if (c.person!=null) {
      s+="<td align=\"center\">";
      c.inside=path;
//      s+="Got person, path="+c.inside;
      s+=c.person.rendertasks(c);
      c.inside="";
      s+="</td>\n";
    }
    s+="<td bgcolor=\"#ffa000\" align=\"center\">";
    s+="<a href=\""+CTW.Url+"?command=realtimeedit&javatype=Search&destpath=tmp&searchpath="+path+(c.person!=null?"&keystring="+cgiencode(((Person)c.person).location)+"&destpath="+((Person)c.person).path:"")+"\">Search</a></td>\n";
    s+="</tr></table>\n";
    s+=CTW.submitdatastart(path);
    return s;
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

        try {
          s+=renderpath(context);
        } catch (Exception e) {
          s+="Problem rendering path<br>";
        }
        try {
          s+=rendereditoptions(context);
        } catch (Exception e) {
          s+="Problem rendering edit options<br>";
        }

        try {
        s+="<tr><td><br>";
        s+=render();
        s+="<br></td></tr>\n";
        } catch (Exception e) {
          s+="Problem rendering object itself<br>";
          Log.error("Problem rendering object "+id);
          Log.report(""+this);
        }

        s+="<tr><td width=\"100%\" bgcolor=\"#ffa000\">";
//        s+=CTW.submitdatastart(path);
          s+="<table width=\"100%\" bgcolor=\"#ffffff\" cellpadding=\"10\"><tr><td>";
        try {
          s+=renderinside();
        } catch (Exception e) {
          s+="Problem rendering inside<br>";
        }
          s+="</td></tr></table>\n";
        try {
          s+=menuaddtasksearch(context);
        } catch (Exception e) {
          s+="Problem rendering menuaddsearch<br>";
        }
//        s+=CTW.submitdatamiddle();
/*    if (context.person!=null) {
      context.inside=path;
//      s+="Got person, path="+c.inside;
      s+=context.person.rendertasks(context);
      context.inside="";
    }*/
//        s+="<a href=\""+CTWServlet.ServletURL+"RequestNew?javatype=Debate&destpath="+path+"\">Add new data</a><br>\n";
        s+="</td></tr></table>\n";
//        s+=CTW.submitdataend();
      } else
        s+=render();
      return s;
    }
  } catch (Exception e) {
    return s+"CTWInput.render(c): "+e;
  }
  }

  public String clip(String s) {
    // Should ensure closure of tags
    return JString.clip(s,100);
  }

}
