package jlib;

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

import java.net.*;

// import changetheworld.XDB;
import jlib.Profile;
import jlib.JNet;

public class JTypes {

	public static final String[] supported={
			"java.lang.Boolean",
			"java.lang.Integer",
			"java.lang.Long",
			"java.lang.Float",
			"java.lang.String",
			"java.util.Date",
			"java.sql.Date",
			"java.net.URL"
		};

  public static String niceJavaType(Class c) {
    String type=c.getName();
    try {
      if (type.equals("java.lang.Integer"))
        return "int";
      if (type.equals("java.lang.Double"))
        return "double";
      if (type.equals("java.lang.Float"))
        return "float";
      /* if (type.equals("java.sql.Date"))
        return "java.util.Date"; */
    } catch (Exception e) {
      System.out.println("JTypes.niceJavaType("+c+"): "+e);
      e.printStackTrace();
    }
    System.out.println("JTypes:niceJavaType("+c+"): remained the same");
    return c.getName();
  }
      /* if (type.startsWith("java.lang.")) {
        String shortType=JString.afterlast(type,".");
        Field f=null;
        f=c.getField("get"+shortType); // ,new Object[0]);
        if (f==null)
          System.out.println("Could not fing get"+shortType+"() in "+c);
        else
          return c;
      } */

  public static boolean isNumber(String classtype) {
    return ( classtype.equals("java.lang.Integer")
          || classtype.equals("java.lang.Long")
          || classtype.equals("java.lang.Double")
          || classtype.equals("java.lang.Float")
          || classtype.equals("int")
          || classtype.equals("long")
          || classtype.equals("float")
          || classtype.equals("double") );
  }

  public static boolean isBool(String classtype) {
    return ( classtype.equals("java.lang.Boolean")
          || classtype.equals("boolean") );
  }

	/** Forgetting changetheworld.*. **/

  /**
   * Tries to turn the String s into an Object of type t.
   * <p>Currently implemented types t are:
   * <blockquote>java.lang.String,
	 *				 <br>Boolean,
	 *				 <br>Integer,
	 *				 <br>Long,
	 *			 	 <br>Float,
	 *				 <br>Double,
   *         <br>java.util.Date,
	 *         <br>java.sql.Date,
	 *         <br>java.net.URL,
   *         <br>changetheworld.*
   * </blockquote>
   * <p>In the changetheworld case, the String s is assumed to be the path/id
   * of the object in the changetheworld database, and XDB.readnode(s) is called.
   * <p>see changetheworld.XDB#readnode(String)<p>
   * <!-- @see changetheworld.XDB#readnode(String) -->
   * @param t the full class path of the desired Object, for example "java.util.Date"
   * @param s a String from which the desired object is generated
   * @return an Object of class t generated from s, or <i>null</i> if the operation failed
   **/

  public static Object fromString(String s,String t) {

    if (s==null) {
      Log.report("JString.fromString() got given s=null!");
      return null;
    }

    try {

      if (t.equals("java.lang.String"))
        return s;

      if (t.equals("java.lang.Boolean")
       || t.equals("boolean"))
        return new Boolean(s);

      if (t.equals("java.util.Date"))
        return (new SimpleDateFormat()).parse(s);

      if (t.equals("java.sql.Date")) {
				// Date d=(new SimpleDateFormat()).parse(s);
				// System.out.println("Converted date "+s+" to "+d);
				// return new java.sql.Date(d.getDate());
				return java.sql.Date.valueOf(s);
			}

      if (t.equals("java.lang.Integer")
       || t.equals("int"))
        return new Integer(s);

      if (t.equals("java.lang.Long")
       || t.equals("long"))
        return new Long(s);

      if (t.equals("java.lang.Float")
       || t.equals("float"))
        return new Float(s);

      if (t.equals("java.lang.Double")
       || t.equals("double"))
        return new Double(s);

      if (t.equals("java.net.URL"))
        return JNet.url(s);

      // Get the class of the destination type
      Class c=null;
      try {
        c=Class.forName(t);
      } catch (Exception e) {
        Log.report("JTypes.fromString(\""+s+"\",\""+t+"\"): can not find desired class \""+t+"\" : "+e);
      }

      // If requested type is an XML object, then
      // s is assumed to be its path
      try {
        Class x=Class.forName("changetheworld.XML");
        if (x.isAssignableFrom(c)) {
          Log.report("JTypes.fromString(): Trying to get "+t+" from path "+s);
          Profile.start("jtype reading inner node");
          Object o=null;
          // try {
            // o=XDB.readnode(s);
          // } catch (Exception e) {
            // Log.error("JTypes.fromString(\""+s+"\",\""+t+"\"): Failed to XDB.readnode: "+e);
          // }
          Profile.stop();
          return o;
        }
      } catch (Exception e) {
        Log.report("JTypes.fromString(\""+s+"\",\""+t+"\"): In ctw bit: "+e);
      }

      // If requested type has String constructor, use that
      try {
        System.out.println("JTypes: looking for constructor");
        Class[] pts=new Class[1];
        pts[0]=Class.forName("java.lang.String");
        Constructor con=c.getConstructor(pts);
        // no checking
        Object[] args=new Object[1];
        args[0]=s;
        Object o=con.newInstance(args);
        Log.report("JTypes: Successfully created new "+t+"(\""+s+"\").");
        System.out.println("Success");
        return o;
      } catch (Exception e) {
        Log.report("JTypes.fromString(\""+s+"\",\""+t+"\"): In constructor: "+e);
      }


    } catch (Exception e) {
      Log.error("JTypes.fromString(\""+s+"\",\""+t+"\"): Problem converting: "+e);
    }

    Log.error("JTypes.fromString(\""+s+"\",\""+t+"\"): Do not know how to convert type "+t);
    return null;

  }

/* // Use JNet.url(s) instead.
  public static URL urlFromString(String s) {
    if ( ! (s.startsWith("http://") || s.startsWith("file:///")) ) {
      s="http://"+s;
    }
    try {
      URL u=new URL(s);
      return u;
    } catch (Exception e) {
      Log.error("JTypes.urlFromString(\""+s+"\"): "+e);
      return null;
    }
  } */

}
