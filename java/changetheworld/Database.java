package changetheworld; import jlib.JString;

import changetheworld.XML;

public class Database extends CTWInput {
  public String title="";
  public String description="Give a description of the category or leave blank";
  public String defaulttype="Debate";
//  public String extratypesallowed="";
  public Database() {
  }
  public String renderlist() {
    return "<font size=\"+2\">"+urltome(title)+"</font>&nbsp;"+clip(description)+"&nbsp;("+insidesize+")";
  }
  public String render(HTMLRenderContext c) {
    if (c.form || c.link || c.list) {
      return rendercontext(c);
    }
    String s="";

    s+="<center>";

//    s+="<table>";

//    s+=renderpath(c);

//    if (!description.equals(new Database().description) && !(description.length()==0)) {
    if (description.trim().length()>0) {
//      s+="<tr><td>";
//      s+="<center>";
  //    s+="<font size=\"+2\"><b>"+title+"</b></font>\n";
      s+=description+"<br>\n";
//      s+="</td></tr>\n";
    }

 //   s+="<tr><td><center>\n";

    s+="<table width=\"70%\" align=\"center\"><tr>";
    s+="<td bgcolor=\"#600060\">~</td>\n";
    s+="<td bgcolor=\"#60A060\" align=\"center\"><font size=\"+2\" color=\"#FFFFFF\"><b>"+title+"</b></font></td>\n";
    s+="<td bgcolor=\"#600060\">~</td></tr>\n</table>\n";

//    s+="<table>\n";
//    s+="<tr><td></td><td><center>\n";
    s+=renderinside(new HTMLRenderContext());

    s+="<table width=\"100%\">\n";

    s+="<tr><td></td><td>"+menuaddtasksearch(c)+"</tr></td>\n";

    s+="</td></tr></table>\n";

    s+="<table align=\"center\" width=\"70%\"><tr><td><table align=\"center\" width=\"100%\">"+JString.replace(rendereditoptions(c),"<br>","&nbsp;&nbsp;&nbsp;")+"</table></td></tr></table>\n";

    getparent();
/*    if (parent==null) {
      String pp=parentpath(path);
      parent=XDB.readtodepth(pp,0);
    }*/
    if (parent!=null) {
      s+="<table align=\"center\" width=\"70%\"><tr><td bgcolor=\"#60a060\" valign=\"center\"><center><font color=\"#ffffff\">Go up to "+((CTWInput)parent).urltome()+"</font></td></tr></table>\n";
//      s+="<table align=\"center\" width=\"70%\"><tr><td bgcolor=\"#2060B0\" valign=\"center\"><center><font color=\"#ffffff\">Go up to "+((CTWInput)parent).urltome()+"</font></td></tr></table>\n";
    }

//    s+="</td></tr>\n";

//    s+="</table>\n";
//    s+="</table>\n";

    /*
    s+=CTW.submitdatastart(path);
    s+="<table align=\"center\" bgcolor=\"#ffa000\"><tr><td>";
    s+=CTW.submitdatamiddle(defaulttype);
    if (c.person!=null) {
      c.inside=path;
//      s+="Got person, path="+c.inside;
      s+=c.person.rendertasks(c);
      c.inside="";
    }
    s+="</td></tr></table>\n";
    s+="<table width=\"70%\"><tr><td bgcolor=\"#ffa000\" width=\"50%\" align=\"center\">";
    s+="<a href=\""+CTWServlet.ServletURL+"RequestNew?javatype="+defaulttype+"&destpath="+path+"\">Submit new "+defaulttype+"</a>\n";
    s+="</td><td bgcolor=\"#ffa000\" width=\"50%\" align=\"center\">";
    s+="<a href=\""+CTW.Url+"?command=realtimeedit&javatype=Search&destpath=tmp&searchpath="+path+(c.person!=null?"&keystring="+cgiencode(((Person)c.person).location)+"&destpath="+((Person)c.person).path:"")+"\">Search</a></td></tr></table>";
//    s+="<a href=\""+CTW.ServletUrl+"Search?searchdbase=path\">Search</a></td></tr></table>";
    s+=CTW.submitdataend();*/
    return s;
  }
/*  public String renderinside(HTMLRenderContext c) {
    String s="";
    for (int i=0;i<inside.size();i++)
      s+=((XML)inside.get(i)).render(c)+"<br>\n";
//    s+="<a href=\""+CTWServlet.ServletURL+"RequestNew?javatype="+defaulttype+"&destpath="+path+"\">Add new data</a><br>\n";
    return s;
  }*/
}