package changetheworld;

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
import java.sql.*;

import changetheworld.XML;
import changetheworld.JMaths;
import changetheworld.JReflect;
import changetheworld.Parameters;
// import changetheworld.JString;
import changetheworld.Satisfier;

import changetheworld.HTMLRenderContext;

import jlib.*;

import changetheworld.CtwDB;

public class XDB {

  public static String log="";

  public static final int XMLstorage=1;
  public static final int DBstorage=2;
  public static int readMode=DBstorage;
  public static int writeMode=DBstorage;
  public static boolean killoriginal=true; // temporary

  public static XML readnode(String path) {
    //Log.reportHtml("<br><center><b><i>Reading node at \""+path+"\"</i></b></center>\n");
    if (readMode==DBstorage) {
      XML x=readfromDB(path);
      x.path=x.id;
      return x;
    } else /* if (readMode==XMLstorage) */ {
      XML x=XML.readfile(new File(CTW.TreePath+new File(path,XML.XmlNodeFileName).toString()).toString());
      if (x==null) {
        Log.error("XDB.readnode(\""+path+"\"): Failed to find file");
      } else {
        x.path=path;
        x.fieldsRead=false;
        x.insideRead=false;
      }
      return x;
    }
  }
  public static void readfields(XML x,int todepth) { // - expected already
    if (readMode==XMLstorage) {
    File f=new File(CTW.TreePath,x.path);
    File[] fs=f.listFiles();
    for (int i=0;i<fs.length;i++)
      if (fs[i].isDirectory()) {
        String name=fs[i].getName();
        Field f2=x.findField(name);
        if (f2!=null) {
          XML y=readtodepth(new File(x.path,name).toString(),todepth);
          x.setpath(name,y);
        }
      }
    x.fieldsRead=true;
    } else {
    x.fieldsRead=true; // Should happen in readfromDB
    }
  }
  public static void readinside(XML x,int todepth) { // - expected already
    if (readMode==XMLstorage) {
    File f=new File(CTW.TreePath,x.path);
    File[] fs=f.listFiles();
    log+="Got "+fs.length+" files from "+CTW.TreePath+x.path+": "+fs+"<br>";
    for (int i=0;i<fs.length;i++)
      if (fs[i].isDirectory()) {
        String name=fs[i].getName();
        Field f2=x.findField(name);
        if (f2==null) {
          XML y=readtodepth(new File(x.path,name).toString(),todepth);
          x.setpath(name,y);
        }
      }
    x.insideRead=true;
    x.consistencycheck();
    // x.insidesize=x.inside.size();
    } else {
      x.insideRead=true;
    }
  }
  public static XML readtodepth(String path,int todepth) {
    // -1 means infinite depth
    // 0 means don't read inside
    // >0 means don't read inside todepth down the line
    try {
    XML x=readnode(path);
    log+=">>"+x.path+"<<";
    int newdepth=( todepth>0 ? todepth-1 : todepth );
    readfields(x,newdepth);
    log+="Hello<br>";
    if (todepth>0 || todepth==-1) {
      log+="Trying to read inside "+x.path;
      readinside(x,newdepth);
    }
    return x;
    } catch (Exception e) {
      Log.error("readtodepth(\""+path+"\","+todepth+"): "+e+"<br>");
    }
    return null;
  }
/*  public static boolean write(XML x) {
    if (x. Oh sod it! */
  public static boolean writenode(XML x) {
    if (writeMode==XMLstorage)
      return x.reallywritenode();
    else if (writeMode==DBstorage)
      return writetoDB(x);
    else // writeMode wrong!
      return x.reallywritenode();
  }

  public static boolean writetoDB(XML x) {

    Log.report("Writing "+x.getClass()+" "+x.path);

    String sql="";
    String table=JString.afterlast(JReflect.classof(x),".").toLowerCase();
    String names="";
    String values="";
    String thiseqthat="";

    Field[] fs=x.openFields();
    for (int i=0;i<fs.length;i++) {
      String name=fs[i].getName();
      Object o=null;
      try { o=fs[i].get(x); } catch (Exception e) { }
      if (o!=null) {
        names+=name;
        String value="'";
        if (JReflect.canbe(o,"changetheworld.XML") && x.fieldsRead) {
          value+=((changetheworld.XML)o).id;
          //Log.reportHtml("<font color=\"#0000ff\">Writing field "+name+"="+value+"</font><br>\n");
          writetoDB((changetheworld.XML)o);
        } else {
          String ts=XML.Stringify(o); // .toString();
          value+=encodeForDB(ts);
        }
        value+="'";
        values+=value;
        thiseqthat+=name+" = "+value;
        if (i<fs.length-1) {
          names+=", ";
          values+=", ";
          thiseqthat+=", ";
        }
      }
    }

    if (x.insideRead) {
      String inside="'{ ";
      for (int i=0;i<x.inside.size();i++) {
        Object o=x.inside.get(i);
        try {
          XML y=(XML)o;
          inside+="\""+y.id+"\"";
          if (i<x.inside.size()-1)
            inside+=", ";
        } catch (Exception e) {
          Log.error("Object "+o+" found in inside of XML "+x);
        }
      }
      inside+=" }'";
      names+=", inside";
      values+=", "+inside;
      thiseqthat+=", inside = "+inside;
      Log.report("Writing inside: "+inside);
    } else
      Log.report("Inside not read, so not overwriting");

    // Temporary
    if (killoriginal || (x.insideRead && x.fieldsRead)) {
      sql+="DELETE FROM "+table+" WHERE id='"+x.id+"';\n";
      sql+="INSERT INTO "+table+" ("+names+") " +
               "VALUES ("+values+");\n";
    } else
    if (readfromDB(x.id)==null) {
      Log.report(x.id+" not found - first write!");
      sql+="INSERT INTO "+table+" ("+names+") " +
               "VALUES ("+values+");\n";
    } else {
      Log.report(x.id+" already exists - updating.");
      sql+="UPDATE "+table+" SET "+thiseqthat+" WHERE id='"+x.id+"';\n";
    }
//    String sql="DELETE FROM "+table+" WHERE id='"+x.id+"';";

    Log.report("<br>\nXDB.writetoDB(): Trying statement \""+sql+"\".\n<br>");

    CtwDB.docommand(sql);

//    Log.report("Got "+o+"<br>\n");

    return true;

  }

  public static String encodeForDB(String s) {
    s=JString.replace(s,"'","\\'");
    return s;
  }

  public static String decodeFromDB(String s) {
    s=JString.replace(s,"\\'","'");
    return s;
  }

  public static XML readfromDB(String path) {

    Profile.start("XDB.readfromDB(path)");

    if (path==null) {
      Log.report("readfromDB got given null - aborting.");
      Profile.stop("XDB.readfromDB(path)");
      return null;
    }

    if (path.trim().equals("")) {
      Log.report("readfromDB got given \"\" - aborting.");
      Profile.stop("XDB.readfromDB(path)");
      return null;
    }

    try {

//    Log.report("path="+path);
    path=JString.afterlast(path,"/");
//    Log.report("path="+path);
    String type=JString.before(path,":");
//    Log.report("type="+type);
    String table=type.toLowerCase();
//    Log.report("table="+table);

//    Profiler profa=new Profiler("Getting "+path+" from JDBC");

    String sql="SELECT * FROM "+table+" WHERE id='"+path+"';";
//    Profiles.dbread.start();
//    Profile.start("dbread");
    ResultSet rs=CtwDB.doquery(sql);
//    Profile.stop();
//    Profiles.dbread.stop();

//    profa.end();

    if (rs==null)
      Log.error("Query returned null!");

//    Profiler profb=new Profiler("Reading in "+path+" from ResultSet using reflection");

      Class c=Class.forName("changetheworld."+type);
      if (c==null)
        Log.error("c=null");
      Object x=c.newInstance();
      if (x==null)
        Log.error("x=null");
      Field[] fields=((XML)x).openFields();
      if (fields==null)
        Log.error("fields=null");

      Profile.start("XDB.readfromDB - Reading open fields from ResultSet");

      boolean hasanythingworked=false;
      rs.first();
      for (int i=0;i<fields.length;i++) {
        Field f=fields[i];
        if (f==null)
          Log.error("f=null");
        String name=f.getName();
        if (name==null)
          Log.error("name=null");
//        Profile.start("Resultset interaction");
        int j=rs.findColumn(name);
//        Profile.stop();
        Log.report("Trying to get field \""+name+"\" ("+j+") from ResultSet "+rs);
        try {
/*          Object oo=rs.getObject(j);
          if (oo==null)
            Log.error("Could not get "+name+" from ResultSet");
          String o=oo.toString();*/
//        Profile.start("Resultset interaction");
          String o=rs.getString(j);
//        Profile.stop();
          String t=f.getType().getName();
          Profile.start("XDB.readfromDB(): field->ResultSet - performing jtype");
          Object xo=JTypes.fromString(o,t);
          Profile.stop();
/*          Profiles.myreflect.start();
          Profile.start("myref");
          boolean done=FastReflect.set("changetheworld."+type,x,name,xo);
          Profile.stop();
          Profiles.myreflect.stop();
          if (!done) {
            Log.error("FastReflect "+type+"."+name+"="+xo+" failed");
  */
          Profile.start("XDB.readfromDB(): field->ResultSet - setting field with reflection");
          try {
            f.set(x,xo);
          } catch (Exception e) {
            Log.error("XDB.readfromDB(\""+path+"\"): Using reflection on field "+name+" : "+e);
          }
          Profile.stop();
    //      }
          hasanythingworked=true;
        } catch (Exception e) {
          Log.error("XDB.readfromDB(\""+path+"\"): On field "+name+" : "+e);
        }
      }

      Profile.stop();

      if (hasanythingworked==false) {
          Log.error("None of the fields read for "+path);
          Log.error("Failing read of "+path);
          Profile.stop();
          return null;
      }

      ((XML)x).fieldsRead=true;

      Profile.start("XDB.readfromDB(): Reading inside Vector");

      // Read the inside Vector
      try {
        // The following are crap:
//        Object o=rs.getObject("inside");
//        Object o=rs.getArray("inside");
        String s=rs.getString("inside");
        Log.report("Got inside field: \""+s+"\"");

//        Profiler profc=new Profiler("Reading inside "+s);

        s=JString.expect(s,"{");
        int i;
        boolean done=false;
        while ( !done ) {
          i=s.indexOf(",");
          if (i==-1) {
            i=s.indexOf("}");
            done=true;
          }
          String p=s.substring(0,i);
          s=s.substring(i+1);
          p=JString.expect(p,"\"");
          p=p.substring(0,p.length()-1);
//          Profile.start("inside logging");
          Log.report("("+i+"): "+p);
          Log.indent(2);
//          Profile.stop();
          Profile.start("inside - reading a node");
          try {
          XML y=readnode(p);
          ((XML)x).inside.add(y);
          } catch (Exception e) {
            Log.error(((XML)x).inside.size()+": Inside got \""+p+"\" but problem with readnode: "+e);
          }
          Profile.stop();
//          Profile.start("inside logging");
          Log.report("Seems like we got it!");
          Log.undent(2);
//          Profile.stop();

//      profc.end();

        }
        ((XML)x).insideRead=true;
      } catch (Exception e) {
        Log.error("Problem with the inside bit: "+e);
      }

      Profile.stop();

//      Log.report("Got "+((XML)x).id+"!");
      Profile.stop();
      return (XML)x;

    } catch (Exception e) {
      Log.error("XDB.readfromDB(\""+path+"\"): Error: "+e);
      Profile.stop();
      Log.error("Failing read of "+path);
      return null;
    }

  }

/*  public static boolean writefields(XML x) {
    boolean ok=true;
    String[] fs=x.openFields();
    for (int i=0;i<fs.length;i++)
      ok=ok && writeneeded(x.getpath(fs[i]));
    return ok;
  }
  public static boolean writeinside(XML x) {
    File f=new File(CTW.TreePath,x.path);
    File[] fs=f.listFiles();
    String[] fs=x.openfields();
    // Delete fields from fs
    // Delete node (non-directory / XmlNodeFileName from fs
    // All remaining in fs are inside files ,', deletable
    // Replace with current x.inside
    return false;
  }
  public static boolean writeneeded(XML x) {
    boolean done=true;
    lock(x.path);
    done=done && writenode(x);
    if (x.fieldsRead)
      done=done && writefields(x);
    if (x.insideRead)
      done=done && writeinside(x);
    unlock(x.path);
    return done;
  }*/
}
