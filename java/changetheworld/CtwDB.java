/* This is the class through which all operations
   on the PostGreSQL database are performed.

   Here follows a fruitless attempt at secutiry handling.
   The question is at which layer do we trust the operator?

   It will check security on reading and manipulation.

   At the moment, we are rejecting queries on person table since it contains passwords.
   But since we want people to search for Person objects, we should allow them through, provided they are rendered (and password is hidden).
   Alternatively, we could keep passwords in a seperate table, and have a special method to check them.*/

package changetheworld; import jlib.JString;

import java.lang.*;
import java.sql.*;
import java.lang.reflect.*;

import jlib.Log;
import jlib.Profile;

import changetheworld.*;

public class CtwDB {

  private static Connection connection=null;
  private static Statement statement=null;
  private static boolean security=false;
  private static boolean release=false;

  public static java.sql.ResultSet doquery(String q) {
    Profile.start("performing JDBC query");
    ResultSet e=execute(q,true);
    Profile.stop("performing JDBC query");
    return e;
  }
  public static void docommand(String q) {
    Profile.start("performing JDBC command");
    execute(q,false);
    Profile.stop("performing JDBC command");
  }
  public static java.sql.ResultSet execute(String q,boolean query) {
//    Log.report("CtwDB.execute(): Trying statement \""+q+"\"");
    if (security && q.indexOf("person")>=0) {
      Log.error("Cannot access table \"person\" since it contains passwords");
      return null;
    }
    connect();
    ResultSet r=null;
    try {
      if (query)
        r=statement.executeQuery(q);
      else
        statement.execute(q);
      Log.reportHtml("Succeeded in query: \""+q+"\"<br>\n");
    } catch (Exception e) {
      if (e instanceof java.sql.SQLException) {
        Log.error("CtwDB.execute(\""+q+"\"): statement.execute() failed: "+e);
//        Log.report("Error in your SQL \""+q+"\"!");
      } else {
        Log.error("CtwDB.execute(\""+q+"\"): statement.execute() failed "+e);
        Log.report("  Assuming connection failure: will try to reconnect next time.");
        // Assume connection is broken to attempt reconnect
        statement=null;
      }
    }
    if (release)
      try {
        statement=null;
        connection.close();
        Log.report("CtwDB.execute(): Closed JDBC connection");
      } catch (Exception e) {
        Log.error("CtwDB.execute(\""+q+"\"): trying to close connection, got "+e);
      }
    return r;
  }

  public static int connect() {
    if (statement!=null) {
      Log.report("CtwDB.connect(): JDBC connection requested but already exists.");
      return 2; // Already connected
    }
    try {
      Class c=Class.forName("org.postgresql.Driver");
      Driver d=(Driver)c.newInstance();
      DriverManager.registerDriver(d);
      connection=DriverManager.getConnection("jdbc:postgresql:ctw","ctwdb","ddbfppq");
      statement=connection.createStatement();
      statement.setQueryTimeout(120); // Times out if query not complete in 2 minutes
      Log.report("CtwDB.connect(): new JDBC connection made.");
      return 1; // Connected OK
    } catch (Exception e) {
      Log.error("CtwDB.connect(): failed "+e);
    }
    Log.report("CtwDB.connect(): JDBC connection attempt failed.");
    return 0; // Failed connection
  }

  public final static void main(String[] args) {

    String[] types=CTWServlet.alltypes();

    // Create empty tables for all the objects
    for (int i=0;i<types.length;i++) {
      String name=types[i];
      try {
        String table=name.toLowerCase();
        Class c=Class.forName("changetheworld."+name);
        Field[] fs=((XML)c.newInstance()).openFields();
        String sql="";
        sql+="CREATE TABLE "+table+" (\n";
        for (int j=0;j<fs.length;j++) {
          String fname=fs[j].getName();
          sql+="  "+fname+" text";
          if (j<fs.length-1)
            sql+=",\n";
        }
        sql+=",\n  inside text[]\n";
        sql+=");\n";
        System.out.println(sql);
        docommand(sql);
        sql="";
        sql+="CREATE INDEX "+table+"ids ON "+table+" (id);\n";
        System.out.println(sql);
        docommand(sql);
      } catch (Exception e) {
        System.out.println("Got exception "+e);
      }
    }

    boolean createfastreflectionclasses=false;

    if (createfastreflectionclasses) {
    // Create a fast reflection class for all the objects
    String ref="";
    ref+="package changetheworld;\n\n";
    ref+="import changetheworld.*;\n";
    ref+="import jlib.*;\n";
    ref+="import java.lang.*;\n";
    ref+="import java.lang.reflect.*;\n\n";
    ref+="public class FastReflect {\n\n";
    ref+="  public static boolean set(String type,Object o,String field,Object value) {\n\n";
    for (int i=0;i<types.length;i++) {
      String name=types[i];
      ref+="    if (type.equals(\"changetheworld."+name+"\")) {\n";
      try {
        Class c=Class.forName("changetheworld."+name);
        Field[] fs=((XML)c.newInstance()).openFields();
        for (int j=0;j<fs.length;j++) {
          String fname=fs[j].getName();
          Field f=c.getField(fname);
          String ftype=f.getType().getName();
          if (!fname.equals("keywords")) {

            ref+="      if (field.equals(\""+fname+"\")) {\n";

            if (ftype.equals("int"))
              ref+="        (("+name+")o)."+fname+"=new Integer(\"\"+value).intValue();\n";
            else if (ftype.equals("boolean"))
              ref+="        (("+name+")o)."+fname+"=new Boolean(\"\"+value).booleanValue();\n";
            else if (ftype.equals("double"))
              ref+="        (("+name+")o)."+fname+"=new Double(\"\"+value).doubleValue();\n";
            else if (ftype.equals("java.lang.String"))
              ref+="        (("+name+")o)."+fname+"=\"\"+value;\n";
            else
              ref+="        (("+name+")o)."+fname+"=("+ftype+")value;\n";

            ref+="        return true;\n";
            ref+="      }\n";

          }
        }
      } catch (Exception e) {
        System.out.println("Got exception "+e);
      }
      ref+="    }\n\n";
    }
    ref+="    return false;\n\n";
    ref+="  }\n\n";
    ref+="}\n";
    Files.writeStringtofile(ref,"/home/httpd/servlets/changetheworld/FastReflect.java");
    }

    System.out.println(""+Log.log);
  }

}