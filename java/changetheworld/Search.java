package changetheworld; import jlib.JString;

import java.lang.*;
import java.util.*;
import java.lang.reflect.*;
import java.text.SimpleDateFormat;

public class Search extends RenderableXML {
  public String keystring="";
  public String searchpath="default";
  public int max=100;
  public double favourchildren=0.3;
  public boolean searchfields=false;
  public String typesallowed="";
  public String byauthor="";
  public boolean showlots=false; // author, date, path
  public boolean preferrecent=false;

  public String[] keywords;
  private static String[] forbiddenpaths={ "sessions" , "tmp" };
  public static String log="Search Log:\n";

  public double keywordoccurrence(XML x) {
    int occs=0;
    try {
    Field[] fs=x.openFields();
    for (int i=0;i<fs.length;i++) {
      Field f=fs[i];
      if (f.getType().getName().equals("java.lang.String")) {
        Object o=f.get(x);
        if (o!=null) {
          String t=(String)o;
//           if (t.length()<10000) {
          String s=JString.tolower(t);
          for (int j=0;j<keywords.length;j++) {
            if (s.indexOf(keywords[j])>=0)
              occs++;
          }
//           }
        }
      }
    }
    } catch (Exception e) {
      log+="keywordoccurrence: "+e+"\n";
    }
    return (double)occs;
  }
  public static boolean forbidden(XML x) {
    for (int i=0;i<forbiddenpaths.length;i++) {
//      log+="Comparing "+x.path+" to "+forbiddenpaths[i]+"\n";
      if (x.path.startsWith(forbiddenpaths[i]) || x.path.startsWith("/"+forbiddenpaths[i]))
        return true;
    }
    return false;
  }
  public double scorefor(XML x) {
    double total=0.0;
    try {
    log+="A\n";
    if (typesallowed!=null)
    if (typesallowed.length()!=0) {
      String xt=x.realClass();
      if (xt!=null) {
        String[] ts=JString.split(typesallowed," ");
        boolean found=false;
        for (int i=0;i<ts.length;i++) {
          if (ts[i]==null)
            log+="Weird: ts["+i+" = null\n";
          if (xt.equals(ts[i]) || JReflect.canbe(x,"changetheworld."+ts[i]))
            found=true;
        }
        if (!found) {
          log+="B\n";
          return 0.0;
        }
      }
    }
    total+=10.0;
    log+="B2\n";
    } catch (Exception e) {
      log+="Search.scorefor(x) typesallowed "+e+"<br>\n";
      return -0.0;
    }
    try {
    if (byauthor!=null)
    if (byauthor.length()!=0) {
      if (!JReflect.canbe(x,"changetheworld.CTWInput"))
        return -1.0;
      CTWInput c=(CTWInput)x;
      if (c.author!=null) {
      XML a=c.author.follow();
      if (a==null)
        return -1.0;
      Person p=(Person)a;
      if (p.name!=null)
      total+=10.0*JString.similarity(p.name,byauthor);
      }
    }
    if (preferrecent && JReflect.canbe(x,"changetheworld.CTWInput")) {
      Date d=((CTWInput)x).creationdate;
      if (d!=null)
        total+=(d.getTime()-(new Date()).getTime())/1000/60/60/24/7+1.0;
    }
    total+=keywordoccurrence(x);
    } catch (Exception e) {
      log+="Search.scorefor(x) "+e+"<br>\n";
    }
    return total;
  }
  public double search(XML x,ScoredList sl) {
log+="a\n";
    if (forbidden(x))
      return -1;
log+="b\n";
    try {
    double childscore=0;
    int childcount=0;
log+="c\n";
    if (searchfields) {
      Field[] fs=x.openFields();
      for (int i=0;i<fs.length;i++) {
        Field f=fs[i];
        Object o=f.get(x);
        if (JReflect.canbe(o,"changetheworld.XML")) {
          XML y=(XML)o;
          childscore+=search(y,sl);
          childcount++;
  //        sl.add(y,scorefor(y));
        }
      }
    }
log+="d\n";
    for (int i=0;i<x.inside.size();i++) {
      Object o=x.inside.get(i);
      if (JReflect.canbe(o,"changetheworld.XML")) {
        XML y=(XML)o;
        childscore+=search(y,sl);
        childcount++;
//        sl.add(y,scorefor(y));
      }
    }
log+="e\n";
    double score=scorefor(x);
    score=( childcount==0 ? score :
      score*(1.0-favourchildren)+childscore/(double)childcount*favourchildren );
log+="f\n";
    sl.add(x,score);
log+="g\n";
    return score;
    } catch (Exception e) {
      log+="search(): "+e+"\n";
      return -1;
    }
  }
  public String render(/*HTMLRenderContext c*/) {
    String s="";
    try {
    keywords=JString.split(keystring," ");
    for (int i=0;i<keywords.length;i++)
      keywords[i]=JString.tolower(keywords[i]);
    XML sx=XML.openpath(searchpath);
    if (sx!=null) {
      ScoredList sl=new ScoredList();
      double tmp=search(sx,sl);
      s+="<center><font size=\"+2\">Search results for </font><i>\""+keystring+"\"</i><font size=\"+2\"> in</font><br>"+sx.renderlist()+"</font><br><br>\n";
      if (sl.size()>max)
        s+="Showing first "+max+" of "+sl.size()+"<br>\n";
      s+="<table><tr>";
      try {
      String ppath=( showlots ? "Path / " : "" );
      s+="<th bgcolor=\"#404070\"><font size=\"+1\" color=\"#ffffff\">"+ppath+"Type</font></th>";
      s+="<th bgcolor=\"#60A060\"><font size=\"+1\" color=\"#ffffff\">&nbsp;</font></th>";
      s+="<th bgcolor=\"#A04040\"><font size=\"+1\" color=\"#ffffff\">";
      if (showlots)
        s+="Author / Date / ";
      s+="Score</font></th>";
      s+="</tr>\n";
      for (int i=0;i<max && i<sl.size();i++) {
        XML x=(XML)sl.get(i);
        double score=sl.getscore(i);
        s+="<tr>";
        s+="<td bgcolor=\"#2060B0\" valign=\"center\" align=\"center\"><font color=\"#ffffff\"><b><i>";
        if (showlots) {
          XML y=x.parent;
          if (y!=null && JReflect.canbe(y,"changetheworld.CTWInput"))
            s+=((CTWInput)y).urltome(((CTWInput)y).gettitle())+": ";
        }
        s+=x.type+"</i></b></font></td>";
        s+="<td align=\"center\">"+x.renderlist()+"</td>";
        s+="<td bgcolor=\"#ff0000\" valign=\"center\" align=\"center\"><font color=\"#ffffff\">";
        if (showlots) {
          s+="<center>";
          if (JReflect.canbe(x,"changetheworld.CTWInput")) {
            CTWInput y=(CTWInput)x;
            Object a=( y.author==null ? null : y.author.follow() );
            if (JReflect.canbe(a,"changetheworld.Person")) {
              Person p=(Person)a;
              s+=p.name+"<br><br>";
            }
            Date d=y.creationdate;
            if (d!=null)
              s+=new SimpleDateFormat("dd/MM/yy").format(d)+"<br><br>";
          }
        }
        s+="<b>"+((int)(score*100.0))+"</b></font></td></tr>\n";
      }
      } catch (Exception e) {
        s+="Search.render(c) inside: "+e+"<br>\n";
      }
      s+="</table>\n";
    }
    } catch (Exception e) {
      s+="Search.render(c): "+e+"<br>\n";
    }
    return s;
  }
  public String renderlink() {
    String s="";
    s+=urltome("Search")+" for \"<i>"+keystring+"</i>\" in <i>"+searchpath+"</i>";
    if (typesallowed.length()>0)
      s+=" of types <i>"+typesallowed+"</i> only";
    if (byauthor.length()>0)
      s+=" by author <i>"+byauthor+"</i>";
    return s;
  }
  public String renderlist() {
    return renderlink();
  }
}