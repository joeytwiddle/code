package changetheworld; import jlib.JString;

import java.lang.String;
import java.lang.reflect.*;

import changetheworld.XML;
import jlib.JString;
import changetheworld.HTMLRenderContext;
import changetheworld.CTW;
import changetheworld.Person;

public class RenderableXML extends XML {

  public String urltome() {
    return urltome(gettitle());
  }
  public String urltome(String red) {
//  if (FileType==Tree || FileType==Jar) {
    return "<a href=\""+CTW.Url+"?command=render&path="+path+"\">"+red+"</a>";
//  } else {
//    return "<a href=\""+CTWServlet.ServletURL+"Browse?browsepath="+path+"\">"+red+"</a>";
//  }
  }
  public String gettitle() {
    Field f=findField("title");
    if (f==null)
      f=findField("name");
    if (f==null)
      return type;
    else
      return (String)getpath("title");
  }

  public String render() {
    return renderunknown(new HTMLRenderContext());
  }
  public String renderlink() {
    return urltome(gettitle());
//    return "RenderableXML.renderlink: using base type or need to overwrite for "+type;
  }
  public String renderlist() {
//    return urltome(gettitle());
    return "RenderableXML.renderlist: using base type or need to overwrite for "+type;
  }
  public String render(HTMLRenderContext context) {
    return rendercontext(context);
  }
  public String rendercontext(HTMLRenderContext context) {
    if (context.link)
      return renderlink();
    if (context.list)
      return renderlist();
    if (context.form)
      return renderunknown(context);
    else
      return render();
  }
  public String renderunknown(HTMLRenderContext context) {
    String s=""; // type+":<br>";
    try {
    if (context.debug)
      s+="Rendering object "+path+"<br>\n";
    if (context.form) {
      s+="<form action=\""+context.action+"\" method=\"get\">\n"+context.formembed;
      s+="<input type=\"hidden\" name=\"destpath\" value=\""+context.destpath+"\">\n";
      s+="<input type=\"hidden\" name=\"javatype\" value=\""+this.realClass()+"\">\n";
      if (context.passid)
        s+="<input type=\"hidden\" name=\"srcid\" value=\""+id+"\">\n";
    }
    s+="<table align=\"center\">\n";
    Field[] fs=openFields();
    for (int i=0;i<fs.length;i++) {
      Field f=fs[i];
      String name=f.getName();
      String showname=JString.toupper(name.charAt(0))+name.substring(1);
      // Hidden within form: passed but not visible or editable to user
      boolean hide=JVector.contains(HiddenInForm,name);
      // Does it appear at all?
      boolean show=true;
      boolean pass=name.equals("pass") || name.equals("Pass");
      if (name.equals("id") && !context.passid)
        show=false;
      if (name.equals("rendered"))
        show=false; // usually too large!
// Would passing the ID ever be a problem?
//      boolean show=!(name.equals("id") && !context.passid) && !name.equals("creationdate") && !name.equals("visits");
      String type=f.getType().getName();
      Object o=f.get(this);
//      s+=""+f+": "+o+"<br>\n";
      try {
//        s+=name+" hide="+hide+" show="+show+"<br>";
        if (show) {
          if (JReflect.canbe(o,"changetheworld.XML")) {
            boolean hit=false;
            if (f.getName().equals("author")) { // Never render an author for editing
              // Instead, set up the author (deprecated by submitnew?)
              if (context.formnew && context.person!=null)
                f.set(this,new Link(context.person.path));
              hit=true;
            }
            if (!hit) {
              XML x=(XML)f.get(this);
              // when render shows type
              String re=( JReflect.canbe(x,"changetheworld.RenderableXML") ? ((RenderableXML)x).renderunknown(context.pluspath(name+".")) : x.render(context.pluspath(name+".")) );
              s+="<tr><td bgcolor=\"#ff0000\"><font size=\"+2\" color=\"#ffffff\"><center>"+x.type+"<br><br>"+showname+"</center></font></td><td bgcolor=\"#0000ff\">"+re+"</td></tr>\n";
              //s+="<tr><td colspan=\"2\">"+x.render()+"</td></tr>\n";
            }
          } else {

          if (!hide)
            s+="<tr><td align=\"right\" valign=\"center\"><font size=\"+1\">"+showname+":</font></td><td>";
          if (context.form) {
            if (JReflect.canbe(o,"boolean") || JReflect.canbe(o,"Boolean") || JReflect.canbe(o,"java.lang.Boolean")) {
              s+="<input type=\"checkbox\" name=\""+context.xmlpath+name+"\""+( ((Boolean)o).booleanValue() ? " checked" : "" )+">\n";
            } else {
              String tos=Stringify(o);
  //            s+=""+name+"="+tos;
              int size=tos.length();
              boolean area=( size>50 || tos.indexOf("\n")>0 || tos.indexOf("<br>")>0);
              String ft=( pass ? "password" : hide ? "hidden" : "text" );
              String head=( area && !hide ? "textarea cols=\"60\" rows=\"5\"" :
                            "input type=\""+ft+"\" size=\"30\"" );
              s+="<"+head+" name=\""+context.xmlpath+name+"\"";
  //            if (area || hide)
                s+=" value=\""+formencode(tos)+"\"";
              s+=" size="+size+">\n";
              if (area && !hide)
                s+=tos+"</textarea>\n";
            }
          } else if (!hide) {
            s+=Stringify(f.get(this));
          }

          }
        }

/*        if (type.equals("java.lang.String")) {
          if (!hide)
            s+="<tr><td align=\"right\" valign=\"center\"><font size=\"+1\">"+showname+":</font></td><td>";
          if (context.form) {
            int size=((String)f.get(this)).length();
            String ft=( hide ? "hidden" : "text" );
            String head=( size>30 && !hide ? "textarea cols=\"60\" rows=\"5\"" :
                          "input type=\""+ft+"\" size=\"30\"" );
            s+="<"+head+" name=\""+context.xmlpath+name+"\"";
            if (size<=30 || hide)
              s+=" value=\""+formencode((String)f.get(this))+"\"";
            s+=" size="+size+">\n";
            if (size>30 && !hide)
              s+=formencode((String)f.get(this))+"</textarea>\n";
          } else if (!hide) {
            s+=(String)f.get(this);
          }
          if (!hide)
            s+="</td></tr>\n";
        } else if (Class.forName("changetheworld.XML").isAssignableFrom(f.getType())) {
          //s+="<tr><td>"+f.getType().getName()+" "+showname+" "+f.get(this)+"</td></tr>";
          boolean hit=false;
          if (f.getName().equals("author")) { // Never render an author for editing
            if (context.formnew && context.person!=null)
              f.set(this,new Link(context.person.path));
            hit=true;
          }
          if (!hit) {
            XML x=(XML)f.get(this);
            // when render shows type
            String re=( JReflect.canbe(x,"changetheworld.RenderableXML") ? ((RenderableXML)x).renderunknown(context.pluspath(name+".")) : x.render(context.pluspath(name+".")) );
            s+="<tr><td bgcolor=\"#ff0000\"><font size=\"+2\" color=\"#ffffff\"><center>"+x.type+"<br><br>"+showname+"</center></font></td><td bgcolor=\"#0000ff\">"+re+"</td></tr>\n";
            //s+="<tr><td colspan=\"2\">"+x.render()+"</td></tr>\n";
          }
        } else {
          s+="<tr><td>"+showname+"</td><td>"+f.get(this)+"</td></tr>\n";
        }*/
      } catch (Exception e) {
        System.out.println("Some problem with forName in XML.render(int)");
      }
    }
    s+="</table>\n";
    if (inside.size()>0 && !context.list && !context.form) {
      context.list=true;
      for (int i=0;i<inside.size();i++) {
        Object o=inside.get(i);
        if (JReflect.canbe(o,"changetheworld.XML"))
          s+=((XML)o).renderXML(context)+"<br>\n";
        else
          s+="RednerableXML.renderunknown can't render "+o+"<br>\n";
      }
      context.list=false;
//      s+="There are "+inside.size()+" other objects but XML.renderxml() can onl render fields at this time<br>\n";
    }
    if (context.form && context.xmlpath.equals("")) {
      s+="<center><input type=\"submit\" name=\"submit\" value=\""+context.button+"\">\n";
      s+="</form>\n";
    }
    } catch (Exception e) {
      s+="RenderableXML.renderunknown:"+e;
    }
    return s;
  }
  public String renderXML(HTMLRenderContext c) {
    return renderunknown(c);
  }
}