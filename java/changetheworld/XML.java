package changetheworld; import jlib.JString;

/* Parsing of XML:
   Tag is assumed to give type:
     If a Java field exists for the tag, the object gets cast to that type and dropped in
     If a Java XML class exists for the tag, a new object is instantiated
     Otherwise, default XML is assumed
*/

import java.lang.*;
import java.lang.reflect.*;
import java.util.Vector;
import java.util.*;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.jar.*;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

import changetheworld.JMaths;
import changetheworld.JReflect;
import changetheworld.Parameters;
import jlib.JString;
import changetheworld.Satisfier;

import jlib.Log;

//import nuju.Nuju;

import changetheworld.HTMLRenderContext;

public class XML {
  // XML information about this object
  public String path="";
  public String id="";
  public String type="no type given";
  String data="";
  Vector inside=new Vector();
  public int insidesize=-1;
  public XML parent=null;
  public static final String[] HiddenInForm={"id","author","creationdate","visits","insidesize","rendered","lastrendered"};
  public static final String[] ClosedFields={"type","path","data","inside","parent","XMLPath",
    "read","FileType","Plain","Jar","Tree","JdbcDb","XmlNodeFileName","log",
    "HiddenInForm","ClosedFields","fieldsRead","insideRead"};

  public static final int Plain=0;
  public static final int Jar=1;
  public static final int Tree=2;
  public static final int JdbcDb=3;
//  public static int FileType=( XDB.writeMode==XDB.XMLstorage || XDB.readMode==XDB.XMLstorage ? XML.Tree : XML.JdbcDb );
  public static int FileType=XML.Tree;
  public static final String XmlNodeFileName="node";
  public static final String XMLPath=CTW.DataPath;
  // Global list of objects read
  public static Vector read=new Vector();
  public static String log="";

  public boolean fieldsRead=false;
  public boolean insideRead=false;

  public static void main(String[] argv) {
    try {
//      Debate d=new Debate();
//      System.out.println(d.render(new HTMLRenderContext()));
//      XML x=XML.parsefile(argv[0]);
//      System.out.println("Here's the object:\n"+x);

      XML.log="Start\n";

      XML x=XDB.readtodepth("/action",2);

//      Nuju.present(x);

      String s="hello";
//      Nuju.present(s);

//      System.out.println(x);

/*      if (argv.length==0) {
      XML z=openpath("");
      if (z!=null) {
        System.out.println("Got object:\n"+z+"\n");
      }

      } else {

      if (argv[0].equals("show")) {
      XML z=openpath("");
      if (z!=null) {
        System.out.println("Got object:\n"+z+"\n");
      }
      } else if (argv[0].equals("rewrite")) {
      XML z=openpath("");
      if (z!=null) {
        System.out.println("Got object:\n"+z+"\n");
        z.writedir();
      }
      } else if (argv[0].equals("convert")) {
        XML.FileType=(new Integer(argv[1])).intValue();
        XML x=XML.openpath("./");
        XML.FileType=(new Integer(argv[2])).intValue();
        x.write();
      } else if (argv[0].equals("sess")) {
        XML x=XML.openpath("sessions");
        Session s=new Session();
        s.time="123";
        s.link=new Link();
        x.inside.add(s);
        x.writepath("sessions");
      } else {

      XML y=new XML();
      for (int i=0;i<argv.length;i++)
        y.inside.add(XML.parsefile(argv[i]));
      if (y.writeall())
        System.out.println("Written!");
      else
        System.out.println(log);

      }

      }
  */
//      else
      System.out.println("XMLLog:\n"+log+"\nFilesLog:\n"+Files.log);


//      XML y=XML.oftype("Debate");
//      System.out.println("Newly created:\n"+y);
//      XML people=XML.parsefile("people");
//      System.out.println("Here's the people:\n"+people);
//      System.out.println("Rendering debate:\n"+((Debate)debates.inside.get(0)).render());
    } catch (Exception e) {
      System.out.println("XML.main:"+e+"\n"+XML.log);
    }
  }

  public XML() {
    type=realClass();
//    id=JString.random(20);
  }

  public String realClass() {
    return JReflect.classof(this);
  }

  public String newID() {
    return type+":"+JString.random(20);
  }

  public void setID(String i) {
    id=i;
  }

  public Object findtag(String t) {
    try {
      Field f=findField(t);
      if (f!=null)
        try { return f.get(this); } catch (Exception e) { }
      for (int i=0;i<inside.size();i++) {
        Object o=inside.get(i);
        if (JReflect.classof(o).equals(t))
          return o;
        try {
          XML x=(XML)o;
          if (x.type.equals(t))
            return o;
        } catch (Exception e) { }
      }
    } catch (Exception e) { }
    return null;
  }

  public static XML openpath(String p) {
    XML x=null;
    if (FileType==Jar) {
      x=XML.getfromjar(p);
    } else if (FileType==Tree) {
      x=XML.getfromdir(p);
    } else if (FileType==Plain) {
      x=parsefile(CTW.PlainName);
//      log+="openpath: not defined for non-jar\n";
//      return null;
    }
    if (x!=null) {
      if (!p.endsWith("/"))
        p+="/";
      x.createpath(p);
    }
    return x;
  }


  public Object getpath(String p) {
    if (p.length()==0)
      return this;
    int i=p.indexOf("/"); // ID
    int j=p.indexOf("."); // tag
    boolean doi=false;
    boolean doj=false;
    if (i>-1 && j>-1)
    if (i<j)
      doi=true;
    else
      doj=true;
    else if (i>-1)
      doi=true;
    else if (j>-1)
      doj=true;
    if (doi) {
      String seekid=p.substring(0,i);
      String therest=p.substring(i+1);
      Object o=findID(seekid);
      if (o==null)
        return null;
      if (therest.length()>0)
        return ((XML)o).getpath(therest);
      return o;
    }
//    if (doj) {
      String seektag=( j==-1 ? p : p.substring(0,j) );
      String therest=( j==-1 ? "" : p.substring(j+1) );
      Object o=findtag(seektag);
      if (o==null)
        return null;
      if (therest.length()>0)
        return ((XML)o).getpath(therest);
      return o;
//    }
//    return null;
  }

  public boolean setpath(String spath,Object o) {
    int i=JMaths.minnominus(spath.indexOf("/"),spath.indexOf("."));
    if (i>0) {
      if (i!=spath.length()-1) // more to parse
        return setpath(spath.substring(i+1),o); // dodgy?!
    // We are at destination
      spath=spath.substring(0,i); // Clear /. marker
    }
    // Is it a field?
    Field f=findField(spath);
//    System.out.println("Field for "+spath+": "+f);
    if (f!=null) {
      try {
        f.set(this,o);
        return true;
      } catch (Exception e) {
        log+="setpath: "+e+"\n  failed to set "+f+" to "+o+"\n";
      }
    }
    // (not bothering trying to find it by type because duplicates will fuck up: must use ID)
    // Can an object in inside be found with that ID?
    for (int j=0;j<inside.size();j++) {
      Object oi=inside.get(j);
      if (JReflect.canbe(oi,"changetheworld.XML")) {
        XML x=(XML)oi;
        if (x.id.compareTo(spath)==0) {
          inside.setElementAt(x,j);
          return true;
        }
      }
    }
    if (JReflect.canbe(o,"changetheworld.XML")) {
      XML x=(XML)o;
      x.id=spath;
      inside.add(x);
    }
    else {
      XML x=new XML();
      x.type=spath;
      if (o.getClass().getName().equals("java.lang.String"))
        x.data="XML.setpath - dodgy! "+(String)o;
      else
        x.inside.add(o);
      inside.add(x);
    }
    return false;
  }

  static public XML findID(String seekid) {
    Vector v=XML.read;
    for (int i=0;i<v.size();i++) {
      XML x=(XML)read.get(i);
      if (x.id.equals(seekid))
        return x;
    }
    System.out.println("Search for object "+seekid+"failed");
    return null;
  }

  public void removeID(String seekid) {
    if (seekid!=null)
      if (seekid.length()>0)
        for (int i=0;i<inside.size();i++)
          if (((XML)inside.get(i)).id.equals(seekid)) {
            inside.remove(i);
            i--;
          }
  }

  public Field[] openFields() {
    try {
    Field[] fs=this.getClass().getFields();
    Vector v=new Vector();
    for (int i=0;i<fs.length;i++) {
      Field f=fs[i];
      String varname=f.getName();
      if (!JVector.contains(ClosedFields,varname))
        v.add(f);
    }
    Field[] ofs=new Field[v.size()];
    for (int i=0;i<v.size();i++)
      ofs[i]=(Field)v.get(i);
    return ofs;
    } catch (Exception e) {
//      System.exit(0);
    }
    return new Field[0];
//    return (Field[])v.toArray();
  }

  Field findField(String name) {
    try {
        Field[] fs=openFields();//this.getClass().getFields();
        int found=-1;
        for (int a=0;a<fs.length && found==-1;a++)
          if (fs[a].getName().equals(name))
            found=a;
        if (found>-1)
          return fs[found];
        else
          return null;
    } catch (Exception e) {
//      System.exit(0);
    }
    return null;
  }

  boolean setField(String fname,Object val) {
    Field f=findField(fname);
    if (f==null)
      return false;
    try {
      f.set(this,val);
      return true;
    } catch (Exception e) {
//      System.exit(0);
      System.out.println("XML.setField: "+e);
    }
    return false;
  }

  public static XML oftype(String type) {
    try {
      Class c=Class.forName("changetheworld."+type);
      if (XML.canbe(c)) {
      Constructor cn=c.getConstructor(new Class[0]);
      XML x=(XML)cn.newInstance(new Object[0]);
      return x;
      }
    } catch (Exception e) {
      System.out.println("XML.oftype("+type+"): "+e);
    }
    return null;
  }

  public static boolean canbe(Class c) {
    boolean cb=false;
    try {
      cb=Class.forName("changetheworld.XML").isAssignableFrom(c);
    } catch (Exception e) {
    }
    return cb;
  }

  private static class P2Res {
    public XML x;
    public int i;
    P2Res(XML xx,int ii) {
      x=xx;
      i=ii;
    }
  }

  public static XML parse(String s,XML context) {
    P2Res p=parse2(s,0,context);
    return p.x;
  }

  private static P2Res parse2(String s,int start,XML context) {
//    Profiler.log+="<b>"+encode(s.substring(start,start+10))+"</b><br>\n";
    // Profiler prof=new Profiler("parse2 on "+s.substring(start,start+10));
//System.out.println("Parsing "+s.substring(start,start+10));
    // Profiler profparm=new Profiler("  params");
//    log+="Start<br>\n";
    int i=s.indexOf("<",start); // start of tag
//    s.delete(0,i+1);
    int k=s.indexOf(">",i+1); // end of tag
    int j=s.substring(i+1,k).indexOf(" "); // crap param space in tag
    if (j>-1)
      j=i+1+j;
    boolean hasinside=!(k!=-1 && s.charAt(k-1)=='/'); // meaning <tag/>
    String type;
    Parameters ps=new Parameters();
    if (j!=-1 && j<k) {
      type=s.substring(i+1,j);
//log+="Type: "+type;
      int l=s.indexOf("=",i+1);
      while (l!=-1 && l<k) {
//log+="<br>l'='="+l+" j' '="+j+" k'>'="+k;
        String par=s.substring(j+1,l);
//log+="  Got param: "+par;
//        s.delete(0,l+1);
        String val;
        int e;
        if (s.charAt(l+1)=='\"') {
          e=s.indexOf("\"",l+2);
//log+="About to chop to "+e+" of "+s.substring(0,e+2);
          val=s.substring(l+2,e);
//   we don't want to!       e++; // to put it on the space
//          s.delete(0,e+1); // to chop next space as well
        } else {
          e=s.indexOf(" ",l+1);
          val=s.substring(l+1,e);
//          s.delete(0,g+1);
        }
//log+="    Val: "+val;
        ps.add(decode(par),decode(val));
        k=s.indexOf(">",e+1);
//        l=s.indexOf("=",e+1);
        String tmp=s.substring(e+1,k);
        l=tmp.indexOf("=");
        if (l>-1)
          l=e+1+l;
//        j=s.indexOf(" ",e+1);  // crap param space in tag
        j=tmp.indexOf(" ");
        if (j>-1)
          j=e+1+j;
//log+="  Next: "+s.substring(0,10);
//log+="  l'=' at "+l+" and k'>' at "+k;
      }
    } else {
      if (hasinside)
        type=s.substring(i+1,k);
      else
        type=s.substring(i+1,k-1);
    }
//    s.delete(0,k+1);

//    profparm.end();

//    log+="A<br>\n";
    // We now have tag representing field or Java type
    String path=type+".";

    try {

      // Profiler profclass=new Profiler("  getting type");

      Class c=null;

      // Is the type a variable in the current context?
      Field found=context.findField(type);
      if (found!=null) {
        c=found.getType();
        if (!Class.forName("changetheworld.XML").isAssignableFrom(c))
          c=null;
      }

      // Otherwise is the type a Java XML type?
      if (c==null) {

//        System.out.println(type+" is not a variable");

      try {
        c=Class.forName("changetheworld."+type);
        if (!Class.forName("changetheworld.XML").isAssignableFrom(c)) {
//          System.out.println(c+" is not an XML object");
          c=null;
        }
      } catch (Exception e) {
  //      System.out.println("Class not found? "+e);
      }

      }

      // We now have class

      System.out.println("Creating an object class "+c);

//    log+="B<br>\n";

      XML xml;
      if (c==null) {
        xml=new XML();
      } else {
        Constructor[] rcs=c.getConstructors();
  //      System.out.println("Constructors in "+c+": "+rcs.length);
        for (int l=0;l<rcs.length;l++) {
  //        System.out.println(""+rcs[l]);
        }
/*        Class[] cs=new Class[1];
        cs[0]="".getClass();
        System.out.println("Creating new object "+cs[0]);*/
        Constructor cr=null;
        try {
        cr=c.getConstructor(new Class[0]);
        } catch (NoSuchMethodException e) {
          System.out.println("Need (empty) constructor public "+c.getName()+"()");
          log+="Need (empty) constructor public "+c.getName()+"()\n<br>\n";
//          System.exit(0);
        }
        String[] ss=new String[1];
        ss[0]="changetheworld."+type;
        xml=(XML)cr.newInstance(new Object[0]);
      }
      xml.type=type;
      xml.path=context.path+type+".";

//    log+="C<br>\n";

//      profclass.end();

  System.out.println("Reading tag "+type);
      // setID(newID());

/*      Field[] fs=new Field[0];
      if (c!=null) {
  //      System.out.println("Real class "+c.getName());
        fs=c.getFields();
  //      for (int a=0;a<fs.length;a++)
  //        System.out.println(fs[a].getType().getName()+" "+fs[a].getName());
      }*/

      // Profiler profin=new Profiler("  inside");

      Vector subobjs=new Vector();
      String cont="";
      if (hasinside) {
        // Profiler profnew=new Profiler("new tag");
        int newtagpos=s.indexOf("<",k+1);
//        profnew.end();
        // Profiler profend=new Profiler("end tag");
        int endtagpos=s.indexOf("</"+type+">",( newtagpos==-1 ? k+1 : newtagpos ));
//        profend.end();
        int rnd=0;
        while (endtagpos>newtagpos) {
          rnd++;
          P2Res p=XML.parse2(s,k+1,xml);
          k=p.i;
          XML x=p.x;
          if (x!=null) {
          x.parent=xml;
          if (x.id!=null && x.id.length()>0)
            x.path=xml.path+x.id+"/";
          // If object is a field, cast it and drop it in
          Field ff=xml.findField(x.type);
  //        System.out.println("Working out what to do with "+x.type+" with field "+ff);
          try {
          if (ff!=null) {
            String ft=ff.getType().getName();
            boolean hit=false;
            if (!hit && ft.equals("java.lang.String")) {
  //            System.out.println("\n\nString "+x.type+": dropping in\n");
  //            System.out.println("Before:\n"+xml.toString("  "));
              try {
              ff.set(xml,x.data);
              hit=true;
              } catch (Exception e) { }
  //            System.out.println("After:\n"+xml.toString("  ")+"\n\n");
            }
            if (!hit && (ft.equals("java.lang.Boolean") || ft.equals("boolean"))) {
              try {
              ff.set(xml,new Boolean(x.data));
              hit=true;
              } catch (Exception e) { }
  //            System.out.println("After:\n"+xml.toString("  ")+"\n\n");
            }
            if (!hit && ft.equals("java.util.Vector")) {
  //            Vector v=(Vector)ff.get(xml);
  //            v.add(x);
              if (!x.getClass().getName().equals("changetheworld.XML"))
                System.out.println("Warning: read Java XML object "+x.getClass().getName()+"\n"+x+"\ninto Vector");
              try {
              ff.set(xml,x.inside);
              hit=true;
              } catch (Exception e) { }
            }
            if (!hit && ft.equals("java.util.Date")) {
              try {
                Date d=(new SimpleDateFormat()).parse(x.data);
                ff.set(xml,d);
                hit=true;
              } catch (Exception e) {
                System.out.println("XML.parse~date:"+e);
                ff.set(xml,new Date());
                hit=true;
              }
            }
            if (!hit && (ft.equals("java.lang.Integer") || ft.equals("int"))) {
//              System.out.println("gerbils\n");
              try {
                ff.set(xml,new Integer(x.data));
                hit=true;
              } catch (Exception e) { }
            }
            if (!hit) {
  //          System.out.println("Probably XML: dropping in");
              try {
              ff.set(xml,x);
              hit=true;
              } catch (Exception e) { }
            }
            if (!hit) {
              System.out.println("Failed to set "+ff+" to "+x);
              xml.inside.add("<readfailure>"+ff+" "+x+"</readfailure>");
            }
          } else {
            // Otherwise just drop it in
  //        System.out.println("Putting inside");
            xml.inside.add(x);
          }
          } catch (Exception e) {
            XML y=new XML();
            y.type=""+e;
            xml.inside.add(y);
          }
          }
          endtagpos=s.indexOf("</"+type+">",k+1);
          newtagpos=s.indexOf("<",k+1);
        }
//        Profiler.log+="Went round "+rnd+" times<br>\n";
        String seek="</"+type+">";
        int f=s.indexOf(seek,k+1);
        //System.out.println("Looking at -"+s.substring(0,10)+"- for "+seek);
        // Profiler profdec=new Profiler("decoding");
        String tmp=s.substring(k+1,f);
        if (!tmp.equals(" ")) // accept a single space, but nothing else
          tmp=tmp.trim();
        xml.data=decode(tmp); //.trim();
//        profdec.end();
//        System.out.println("Data for "+xml.type+" "+xml.data);
//        s.delete(0,f+seek.length());
        k=f+seek.length();
      }

//      profin.end();

//      System.out.println("Finished creation of object "+xml+"\n");
      read.add(xml);
//      prof.what=xml.type;
//      prof.end();
      return new P2Res(xml,k);

    } catch (Exception e) {
      String report="Something went wrong with reflection: "+e+"\n";
      report+=type+"\n";
      report+="reading "+s+"\n";
      log+="<B>"+report+"</B>";
      System.out.println(report);
      return null;
/*      XML x=new XML();
      x.type="Big "+e;
      return x;*/
    }

//    System.out.println("Failed creation of object "+type);
//    return null;

  }

  public static XML parsefile(String fname) { //throws Exception {
    XML x=readfile(XMLPath+fname);
    x.createpath(fname+":"+x.type+".");
    return x;
  }

  public static XML readfile(String fname) { //throws Exception {
//    log+="Reading file "+fname+"<br>\n";
    try {
    XML x=new XML();
    x.path=fname+":"; // "ARSE!";
    XML y=XML.parse(Files.readStringfromfile(fname),x);
    y.createpath(fname+":"+y.type+".");
    return y;
    } catch (Exception e) {
      XDB.log+="XML.readfile: "+e+"<br>";
    }
    return null;
  }

  public void createpath(String p) { // Also points all parents to correct places (except root)
//    log+=p+"<br>\n";
    if (!p.endsWith("/"))
      p+="/";
    path=p;
    Field[] fs=openFields();
    for (int i=0;i<fs.length;i++) {
      Object o=null;
//      log+=p+"<font color=\"#ff0000\">"+fs[i].getName()+":";
      try { o=fs[i].get(this); } catch (Exception e) { log+="createpath("+p+") failed: "+e+"\n"; }
      if (o!=null) {
//        log+="succeed\n";
      if (JReflect.canbe(o,"changetheworld.XML")) {
        XML x=(XML)o;
        x.createpath(p+fs[i].getName()+"/");
        x.parent=this;
      }
      }
//      log+="</font><br>";
    }
    for (int i=0;i<inside.size();i++) {
      Object o=inside.get(i);
      if (JReflect.canbe(o,"changetheworld.XML")) {
        XML x=(XML)o;
        if (x.id!=null)
          if (x.id.length()>0)
            x.createpath(p+x.id+"/");
        x.parent=this;
      }
    }
//    log+="Fields: "+fs.length+" Inside: "+inside.size()+"<br>\n";
  }

  public boolean writefile(String fname) {
    return Files.writeStringtofile(this.toString(),XMLPath+fname);
/*    System.out.println("Writing file "+fname);
    System.out.println(this.toString());*/
  }

  public static String encode(String s) {
    s=JString.replace(s,"<br>","\n");
//    s=JString.replace(s,"\n","<br>");
    s=JString.replace(s,"<","&lt;");
    s=JString.replace(s,">","&gt;");
//    s=JString.replace(s," ","%20");
//    s=JString.replace(s,"<br>","\n");
    return s;
  }

  public static String decode(String s) {
/*    String[][] lst={
      {"&lt;","<"},
      {"&gt;",">"},
      {"!qm!","\""},
      {"&quot;","\""},
      {"\n","<br>"},
      {"!eq","="},
      {"!quest!","?"}
    };
    s=JString.replace(s,lst);*/

    s=JString.replace(s,"&lt;","<");
    s=JString.replace(s,"&gt;",">");
    s=JString.replace(s,"!qm!","\""); // old
    s=JString.replace(s,"&quot;","\"");
//    s=JString.replace(s,"&amp;","&");
//    s=JString.replace(s,"\n","<br>");
    s=JString.replace(s,"\n","<br>");
    s=JString.replace(s,"!eq","=");
    s=JString.replace(s,"!quest!","?");
    return s;
  }

//    s=JString.replace(s,"=","#061;");
//    s=JString.replace(s,"?","&#063;");

  public static String paramdecode(String s) {
    s=JString.replace(s,"\n","<br>");
    s=JString.replace(s,"%20"," ");
    s=JString.replace(s,"%2F","/");
    return s;
  }

  public static String paramencode(String s) {
    s=JString.replace(s," ","%20");
    s=JString.replace(s,"/","%2F");
    s=JString.replace(s,"\n","<br>");
    return s;
  }

  String formencode(String s) {
    String e=s;
//    XML.log+="Changing "+e+"\n";
    e=JString.replace(e,"\n","");
    e=JString.replace(e,"<br>","\n");
//    XML.log+="==== via "+e+"\n";
    e=encode(e);
//    XML.log+="===== to "+e+"\n";
    return e;
/*    s=JString.replace(s,"\"","&quot;");
    return s;*/
  }

  String cgiencode(String s) {
    s=JString.replace(s," ","+");
    s=JString.replace(s,"\"","&quot;");
    s=JString.replace(s,"=","!eq!");
    s=JString.replace(s,"?","!quest!");
//    s=JString.replace(s,"&","&amp;"); useless!
    s=encode(s);
    return s;
  }

  public String toString(String ind) {
    String s=ind+"<"+encode(type);
/*    if (!type.equals("XML"))*/
      s+=" javaclass=\""+encode(this.getClass().getName())+"\"";
      s+=" insidenum=\""+inside.size()+"\"";
    s+=">";
    Field[] fs=openFields();
    boolean written=false;
    for (int i=0;i<fs.length;i++) {
      Field f=fs[i];
//System.out.println(""+f);
      String varname=f.getName();
        try {
        Object o=f.get(this);
        if (o==null) {
//          s+="\n"+ind+"<"+varname+"></"+varname+">";
          // s+="\n"+ind+"<error "+f+" is null></error>";
        }
        else {
        written=true;
        String type=o.getClass().getName();
        if (type.equals("java.lang.String")) {
          if (!(varname.equals("id") && ((String)o).length()==0))
            s+="\n"+ind+"  <"+varname+">"+encode((String)o)+"</"+varname+">";
        } else if (type.equals("java.util.Vector")) {
          Vector v=(Vector)o;
          s+="\n"+ind+"  <"+varname+">";
          for (int j=0;j<v.size();j++) {
            XML x=(XML)v.get(j);
            s+="\n"+x.toString("    "+ind);
          }
          s+="\n"+ind+"  </"+varname+">";
        } else if (type.equals("java.lang.Integer")) {
          s+="\n"+ind+"  <"+varname+">"+encode(((Integer)o).toString())+"</"+varname+">";
        } else if (type.equals("java.util.Date")) {
          s+="\n"+ind+"  <"+varname+">"+encode((new SimpleDateFormat()).format((Date)o))+"</"+varname+">";
        } else if (Class.forName("changetheworld.XML").isAssignableFrom(o.getClass())) {
          XML x=(XML)o;
          x.type=varname;
          s+="\n"+x.toString("  "+ind);
        } else {
          try {
            s+="\n"+ind+"  <"+varname+">"+encode(o.toString())+"</"+varname+">";
          } catch (Exception e) {
            s+="\n"+ind+"<!error don't know how to write type "+type+"!>";
            System.out.println("Don't know how to write "+type+" to XML");
          }
        }
        }
        } catch (Exception e) {
          System.out.println("XML.toString(): "+e);
        }
    }
//    s+="Inside me is "+inside.size();
    for (int j=0;j<inside.size();j++) {
      s+="\n"+((XML)inside.get(j)).toString("  "+ind);
      written=true;
    }
    s+=( written ? "\n"+ind : "" );
    s+=( written ? "" : data.trim() );
    s+="</"+encode(type)+">";
    return s;
  }





  public String toString() {
    return toString("");
  }

  public String render() { // deprecated -> RenderableXML
    return render(new HTMLRenderContext());
  }

  public String render(HTMLRenderContext context) { // deprecated -> RenderableXML
    return renderXML(context);
  }

  public String renderlist()  {// deprecated -> RenderableXML
    return render(new HTMLRenderContext("list"));
  }

  public String renderlink()  {// deprecated -> RenderableXML
    return render(new HTMLRenderContext("link"));
  }

  public String renderinside(HTMLRenderContext c) { // deprecated -> RenderableXML
    if (c.list)
      return "("+inside.size()+")";
//      return "("+inside.size()+")";
    String s="<br>";
    s+="<font size=\"-1\">";
    try {
    if (!insideRead)
      XDB.readinside(this,0);
    for (int i=0;i<inside.size();i++) {
      Log.report("Rendering "+i+"/"+inside.size());
      try {
      XML x=(XML)inside.get(i);
      Log.report("Got "+x.id);
      if (JReflect.canbe(x,"changetheworld.RenderableXML")) {
        Log.report("Rendering as Renderable");
	      s+=((RenderableXML)x).renderlist(); //(new HTMLRenderContext("list"));
        if (!s.endsWith("</table>") && !s.endsWith("</table>\n") && !s.endsWith("<br>") && !s.endsWith("<br>\n"))
          s+="<br>\n";
	    } else
	      log+="Refusing to render object type "+x.getClass()+"<br>\n";
      s+="<br>\n";
      //      s+="&nbsp;("+x.type+")<br>\n";
      } catch (Exception e) {
        s+="Failed to render "+i+"/"+inside.size();
        Log.error("Failed to render "+i+"/"+inside.size());
      }
    }
    } catch (Exception e) {
      s+="XML.renderinside(c): "+e+"<br>\n";
      Log.error("XML.renderinside(c): "+e);
    }
    s+="</font>\n";
    return s;
  }

  public String renderinside() { // deprecated -> RenderableXML
    return renderinside(new HTMLRenderContext());
  }

  public String renderXML(HTMLRenderContext context) { // deprecated -> RenderableXML
    String s=""; // type+":<br>";
    try {
    if (context.debug)
      s+="Rendering object "+path+"<br>\n";
    if (context.form) {
      s+="<form action=\""+context.action+"\" method=\"post\">\n"+context.formembed;
      s+="<input type=\"hidden\" name=\"destpath\" value=\""+context.destpath+"\">\n";
      s+="<input type=\"hidden\" name=\"javatype\" value=\""+this.realClass()+"\">\n";
      if (context.passid)
        s+="<input type=\"hidden\" name=\"srcid\" value=\""+id+"\">\n";
    }
    s+="<table align=\"center\">Rendering XML "+type+" (should be deprecated)\n";
    Field[] fs=openFields();
    for (int i=0;i<fs.length;i++) {
      Field f=fs[i];
      String name=f.getName();
      String showname=JString.toupper(name.charAt(0))+name.substring(1);
      boolean hide=name.equals("id");
      boolean show=!(name.equals("id") && !context.passid);
      String type=f.getType().getName();
      try {
        if (show)
        if (type.equals("java.lang.String")) {
          if (!hide)
            s+="<tr><td>"+showname+":</td><td>";
          if (context.form) {
            int size=((String)f.get(this)).length();
            if (size<25)
              size=25;
            s+="<input type=\""+( hide ? "hidden" : "text"  )+"\" name=\""+name+"\" value=\""+(String)f.get(this)+"\" size="+size+">\n";
          } else if (!hide) {
            s+=(String)f.get(this);
          }
          if (!hide)
            s+="</td></tr>\n";
        } else if (Class.forName("changetheworld.XML").isAssignableFrom(f.getType())) {
          //s+="<tr><td>"+f.getType().getName()+" "+showname+" "+f.get(this)+"</td></tr>";
          XML x=(XML)f.get(this);
          // when render shows type
          s+="<tr><td>"+showname+"</td><td>"+x.render()+"</td></tr>\n";
          //s+="<tr><td colspan=\"2\">"+x.render()+"</td></tr>\n";
        } else {
          s+="<tr><td>"+showname+"</td><td>"+f.get(this)+"</td></tr>\n";
        }
      } catch (Exception e) {
        System.out.println("Some problem with forName in XML.render(int)");
      }
    }
    s+="</table>\n";
    if (inside.size()>0 && !context.list) {
      context.list=true;
      for (int i=0;i<inside.size();i++) {
        Object o=inside.get(i);
        if (JReflect.canbe(o,"changetheworld.XML"))
          s+="ID="+((XML)o).id+": "+((XML)o).renderXML(context)+"<br>\n";
        else
          s+="XML.renderXML can't render "+o+"<br>\n";
      }
      context.list=false;
//      s+="There are "+inside.size()+" other objects but XML.renderxml() can onl render fields at this time<br>\n";
    }
    if (context.form) {
      s+="<center><input type=\"submit\" name=\"submit\" value=\""+context.button+"\">\n";
      s+="</form>\n";
    }
    } catch (Exception e) {
      s+=e;
    }
    return s;
  }

  float score() {
    Field f;
    try {
      f=findField("rating");
      if (f!=null)
        return ((Rating)f.get(this)).score();
    } catch (Exception e) {
    }
    return (float)0.0;
  }

  public static String getdbfile(String path) {
    int i=path.indexOf(":");
    if (i>=0)
      return path.substring(0,i);
    return null;
  }

  public static String getxmlpath(String path) {
    int i=path.indexOf(":");
    if (i>=0)
      return path.substring(i+1);
    return null;
  }

  public boolean writedir(String dir,String ind,JarOutputStream jos) {
    try {

//    log+="Writing dir "+type+" at "+dir+" ("+openFields().length+","+inside.size()+") jos="+jos+"\n";

    if (jos==null) {
//      System.out.println("Writing "+fname);
      File f=new File(dir);
      if (f.exists())
        Files.deltree(f);
      f=new File(f.toString());
      f.mkdirs();
    }

    String s="";
//    s+=ind+"<"+type+">\n"; // type can be tag or
    String tag=JString.afterlast(JReflect.classof(this),".");
    s+=ind+"<"+tag+">\n";
    Field[] fs=openFields();

    for (int i=0;i<fs.length;i++) {
      String name=fs[i].getName();
      Object o=null;
      try { o=fs[i].get(this); } catch (Exception e) { }
//      log+=i+" "+name+" : "+o+"\n";
      if (o!=null)
      if (JReflect.canbe(o,"changetheworld.XML")) {
        XML x=(XML)o;
        File f=new File(dir,fs[i].getName());
//        if (jos==null)  does it for itself, above
//          if (!f.mkdirs())
//            return false;
        x.writedir((new File(dir,name)).toString(),ind+"  ",jos);
//        log+="Adding XML object "+x.type+"\n";
      } else {
        String ts=Stringify(o); // .toString();
        if (ts.length()>0)
        s+=ind+"  <"+name+">"+ts+"</"+name+">\n";
//        log+="Adding unknown field "+name+"\n";
      }
    }

    // Now do inside ***
    for (int i=0;i<inside.size();i++) {
      Object o=inside.get(i);
//      log+="got "+o+"\n";
      if (JReflect.canbe(o,"changetheworld.XML")) {
        XML x=(XML)o;
        if (x.id==null)
          x.setID(newID());
        if (x.id.length()==0)
          x.setID(newID());
/*        String name=x.type; // No good - might get duplicate types!
        if (x.id!=null)
          if (x.id.length()>0)
            name=x.id;*/
        String name=x.id;
        File f=new File(dir,name);
        String newdir=f.toString();
//        log+="Adding inner XML object "+newdir+"\n";
//        if (jos==null)  will do it itself
//          if (!f.mkdirs())
//            return false;
        x.writedir(newdir,ind+"  ",jos);
//          return false;
      } else {
        log+="P\n";
        String name=o.getClass().getName();
        String ts=Stringify(o); // .toString();
        if (ts.length()>0 || !name.equals("id"))
          s+=ind+"  <"+name+">"+Stringify(o)+"</"+name+">\n";
//        log+="Adding unknown inner object "+name+"\n";
      }
    }

    s+=ind+"</"+tag+">\n";

//    log+="Got here in "+type+" with jos="+jos+"\n";

    String fname=(new File(dir,XmlNodeFileName)).toString();
    if (jos==null) {
      if (s.indexOf("<br>")>0)
        log+="\n<br><b>File "+fname+" contains &lt;br&gt;</b><br>\n";
      Files.writeStringtofile(s,fname);
    } else {
//      log+="adding xml object "+type+" to "+dir+"\n";
      JarEntry je=new JarEntry(fname);
      je.setSize(s.length());
      jos.putNextEntry(je);
      byte[] bs=new byte[s.length()];
      for (int i=0;i<s.length();i++)
        bs[i]=(byte)s.charAt(i);
      jos.write(bs); // ,0,bs.length);
      jos.closeEntry();
    }
    return true;
    } catch (Exception e) {
      log+="XML.writedir(...):"+e;
      System.out.println("***************************"+e+"*************************");
      return false;
    }
  }

  public boolean writenode() {

    return XDB.writenode(this);

  }

  public boolean reallywritenode() {

    try {

    String s="";
    String ind="";
//    s+=ind+"<"+type+">\n"; // type can be tag or
    String tag=JString.afterlast(JReflect.classof(this),".");
    s+=ind+"<"+tag+">\n";
    Field[] fs=openFields();

    for (int i=0;i<fs.length;i++) {
      String name=fs[i].getName();
      Object o=null;
      try { o=fs[i].get(this); } catch (Exception e) { }
//      log+=i+" "+name+" : "+o+"\n";
      if (o!=null)
      if (!JReflect.canbe(o,"changetheworld.XML")) {
        String ts=Stringify(o); // .toString();
        if (ts.length()>0)
        s+=ind+"  <"+name+">"+ts+"</"+name+">\n";
//        log+="Adding unknown field "+name+"\n";
      }
    }

    s+=ind+"</"+tag+">\n";

//    log+="Got here in "+type+" with jos="+jos+"\n";

    String fname=(new File(new File(CTW.TreePath,path),XmlNodeFileName)).toString();
      if (s.indexOf("<br>")>0)
        log+="\n<br><b>File "+fname+" contains &lt;br&gt;</b><br>\n";
      Files.writeStringtofile(s,fname);
    return true;

    } catch (Exception e) {
      log+="XML.writedir(...):"+e;
      System.out.println("***************************"+e+"*************************");
      return false;
    }

  }

  public boolean writeall() {
    if (FileType==Jar)
      return writejar();
    if (FileType==Plain)
      return writefile("ctwdata.xml");
    if (FileType==Tree)
      return writedir();
    return false;
  }

  public boolean writedir() {
    String destdir=CTW.TreePath;
    File f=new File(destdir);
/*    if (!Files.deltree(f)) {
      log+="XML.writedir(): Files.deltree("+f+") failed\n";
      return false;
    }*/
    return writedir(destdir,"",null);
  }

  public boolean write() {
    XDB.writenode(this);
    return true;
/*    Files.log+="A write has been requested on object "+path+"\n";
    if (FileType==Tree)
      return writepath(path);
    if (FileType==Plain) {
      System.out.println("Writing plain "+CTW.PlainPath);
      return Files.writeStringtofile(toString(),CTW.PlainPath);
    }
    return false; */
  }

  public boolean writepath(String p) {
    String destdir=(new File(CTW.TreePath,p)).toString();
//    File f=new File(destdir);
//    boolean cleared=Files.deltree(f);
/*    if (!cleared) {
      log+="XML.writepath("+p+"): Files.deltree("+f+") failed\n";
      return false;
    }
*/    return writedir(destdir,"",null);
  }

  public boolean writejar() {
    try {
    String jardir=CTW.JarPath;
    File f=new File(jardir);
    FileOutputStream fos=new FileOutputStream(f);
    JarOutputStream jos=new JarOutputStream(fos);
    log+="a\n";
    writedir(".","",jos);
    log+="b\n";
    jos.flush();
    jos.finish();
    jos.close();
    fos.close();
    return true;
    } catch (Exception e) {
      log+="XML.writejar: "+e;
      return false;
    }
  }

  private static Vector subobjectsof(JarFile jf,String path) {
    String unwanted=(new File(path,XmlNodeFileName)).toString();
    Vector os=new Vector();
    Enumeration enu=jf.entries();
    while (enu.hasMoreElements()) {
      JarEntry je=(JarEntry)enu.nextElement();
//      Attributes as=je.getAttributes();
      String name=je.getName();
//System.out.println(""+name+" "+je.isDirectory()/*+" ("+path+")"*/);
      if (name.startsWith(path) && name.endsWith(XmlNodeFileName) && !name.equals(unwanted)) {
        String fname=JString.after(name,path);
        fname=JString.beforelast(fname,XmlNodeFileName);
        fname=JString.stripendsof(fname,"/"+File.pathSeparatorChar);
//        System.out.println("File stripped "+fname);
        if (fname.indexOf("/")==-1)
          os.add(fname);
      }
    }
    return os;
  }

  public static XML getfromjar(String path) {
    try {
    if (!path.startsWith("./"))
      path="./"+path;
    JarFile jf=new JarFile(CTW.JarPath);
    String nodename=(new File(path,XmlNodeFileName)).toString();
//    if (nodename.startsWith("./"))
//      nodename=JString.after(nodename,"./");
    JarEntry je=jf.getJarEntry(nodename);
    if (je==null) {
      log+="getfromjar: "+nodename+" not found\n";
      return null;
    }
    InputStream is=null;
    try { is=jf.getInputStream(je); } catch (Exception e) { }
    if (is==null) {
      log+="getfromjar: could not get input stream\n";
      return null;
    }
    String s="";
    while (is.available()>0) {
      //log+=is.available();
      s+=(char)is.read();
    }
    is.close();
    XML x=new XML();
    x.path=path;
    XML y=XML.parse(s,x);
//    log+="Got "+y+"\n";
    Vector subs=subobjectsof(jf,path);
//    System.out.println(""+path);
//System.out.println("Inserting "+subs.size()+" elements\n  from "+path+"\n");
    for (int i=0;i<subs.size();i++) {
      String n=(String)subs.get(i);
      XML z=getfromjar((new File(path,n)).toString());
//System.out.println("\n  Setting "+n+" to "+z+"\n");
      y.setpath(n,z);
    }
    y.createpath("JARPATH="+path+":"+y.type+".");
    return y;
    } catch (Exception e) {
      log+="XML.getfromjar("+path+"):"+e;
      return null;
    }
  }

  public static XML getfromdir(String path) {
    try {
    String nodename=(new File(path,XmlNodeFileName)).toString();
    String realpath=(new File(CTW.TreePath,path)).toString();
    String realnodepath=(new File(CTW.TreePath,nodename)).toString();
//    log+="A "+realnodepath+"<br>\n";
    XML x=readfile(realnodepath);
//    log+="B<br>\n";
    if (x==null) {
      log+="getfromdir("+path+"): no file "+realnodepath;
      return null;
    }
    File f=new File(realpath);
    File[] fs=f.listFiles();
    for (int i=0;i<fs.length;i++)
      if (fs[i].isDirectory()) {
        String name=fs[i].getName();
        XML y=getfromdir((new File(path,name)).toString());
        if (y==null) {
          log+="sub object "+name+" did not read";
//          return null;
        } else
          x.setpath(name,y);
      }
    if (!path.endsWith("/"))
      path+="/";
    // x.createpath(path);
    return x;
    } catch (Exception e) {
      log+="XML.getfromdir("+path+"): "+e;
      return null;
    }
  }

  public static String Stringify(Object o) {
        String type=o.getClass().getName();
        if (JReflect.canbe(o,"changetheworld.XML")) {
          return ((XML)o).toString();
        } else if (type.equals("java.lang.String")) {
          return encode((String)o);
        } else if (type.equals("java.lang.Integer")) {
          return ((Integer)o).toString();
        } else if (type.equals("java.util.Date")) {
          return encode((new SimpleDateFormat()).format((Date)o));
        } else if (type.equals("java.util.Vector")) {
          Vector v=(Vector)o;
          String s=" a vector size "+v.size()+":\n";
          for (int i=0;i<v.size();i++) {
            Object t=v.get(i);
            s+=Stringify(t)+"\n";
          }
          return s;
        } else {
          return ""+o;
        }
  }

  public static Object unStringify(String type,String s) {
    try { type=Class.forName(type).getName(); } catch (Exception e) { System.out.println("unStringify: "+e); }
    if (type.equals("java.util.Date")) {
      try { return (new SimpleDateFormat()).parse(s); } catch (Exception e) { }
    }
    return null;
  }

  public static String parentpath(String p) {
    int i=p.lastIndexOf("/");
    if (i==p.length()-1)
      i=p.substring(0,p.length()-1).lastIndexOf("/");
    if (i>=0)
      return p.substring(0,i);
    return "./";
  }

  public static String childpath(String p) {
    int i=p.lastIndexOf("/");
    if (i==p.length()-1)
      i=p.substring(0,p.length()-1).lastIndexOf("/");
    if (i>=0)
      return p.substring(i+1);
    return "./";
  }

  public boolean maybeeditedby(Person p) {
    log+="XML.maybeeditedby(): Should have been overwritten for class "+realClass()+"!\n";
    return true;
  }

  public boolean insidemaybedeletedby(Person p) {
    boolean maybe=true;
    for (int i=0;i<inside.size();i++) {
    	Object o=inside.get(i);
    	if (JReflect.canbe(o,"changetheworld.XML"))
    		maybe=maybe && ((XML)o).maybedeletedby(p);
    }
    return maybe;
	}
	
  public boolean maybedeletedby(Person p) {
  	return insidemaybedeletedby(p);
  }

  public boolean deleteID(String seekid) {
    Satisfier s=new ObjectContainsField("id",seekid);
    Object o=JVector.getfirst(s,inside);
    if (o==null) {
      log+="Could not find ID "+seekid+"\n";
      return false;
    }
    inside.remove(o);
    return true;
  }

  public boolean ownconsistencycheck() {
    return true;
  }
  public void consistencycheck() {
    boolean nodewrite=false;
    log+="Running consistency check on "+id+"\n";
    if (ownconsistencycheck())
      nodewrite=true;
    if (insideRead && insidesize!=inside.size()) {
      insidesize=inside.size();
      nodewrite=true;
    }
    if (fieldsRead) {
      try {
        Class c=this.getClass();
        Constructor cn=c.getConstructor(new Class[0]);
        XML n=(XML)cn.newInstance(new Object[0]);
        Field[] fs=openFields();
        for (int i=0;i<fs.length;i++) {
          Field f=fs[i];
          String fn=f.getName();
          try {
/*            try {
            if (fn.equals("defaulttype") && f.get(this).equals(".")) {
              f.set(this,"Debate");
              nodewrite=true;
            }
            if (f.get(this).equals(".")) {
              f.set(this,"");
              nodewrite=true;
            }
            } catch (Exception e) {
              log+="XML.cc: Bound to fail on "+f+" with "+e+"\n";
            }*/
            if (!fn.equals("defaulttype") && !fn.equals("title") && !fn.equals("name")) {
              Object mineo=f.get(this);
              if (mineo==null) {
                if (JReflect.canbe("",f.getClass().toString())) {
                  log+="XMLcc: Setting "+f+"="+mineo+" to \".\"<br>\n";
                  f.set(this,".");
                  nodewrite=true;
                }
              } else {
                String mine=mineo.toString().trim();
                log+="Got element "+mine+"\n";
                if (mine!=null)
                if (!mine.equals("")) {
                  Object defo=f.get(n);
                  if (defo!=null) {
                    String def=defo.toString().trim();
                    log+="Comparing to "+def+"\n";
                    if (!def.equals(""))
                      if (mine.equals(def))
                        if (JReflect.canbe("",f.getType().getName())) {
  //                      if (mine.getClass().getName().equals("java.lang.String")) {
                          log+="XMLcc: Setting "+f+"="+mine+"=default to \".\"<br>\n";
                          f.set(this,".");
                          nodewrite=true;
                        }
                  }
                }
              }
            }
          } catch (Exception e) {
            log+="XML.consistencychecking: "+e+" Crashed on field "+fs[i]+"="+fs[i].get(this)+" of "+this+"<br>\n";
          }
        }
      } catch (Exception e) {
        log+="XML.consistencychecking:Crashed<br>\n";
      }
    }
    if (nodewrite)
      XDB.writenode(this);
  }

  public XML getparent() {
    if (parent==null) {
      String pp=parentpath(path);
      parent=XDB.readtodepth(pp,0);
    }
    return parent;
  }

}



/*class Email {
  String from;
  Vector to;
  String subject;
  String content;
  Vector attachment;
}*/

