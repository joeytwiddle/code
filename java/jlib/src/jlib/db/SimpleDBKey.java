package jlib.db;

import java.lang.String;

import jlib.*;

public class SimpleDBKey implements DBKey {
  
  static String objPackage="changetheworld"; 
  static final DBLocation loc=new DBLocation("org.postgresql.Driver","jdbc:postgresql:generation","ctwdb","ddbfppq");

  public String type,id;

  public SimpleDBKey(String o,String t,String i) {
    objPackage=o;
    type=t;
    id=i;
  }

  public DBLocation location() {
    return loc;
  }

  public String id() {
    return id;
  }

  public String table() {
    return type;
  }

  public java.lang.Class type() {
    try {
      System.out.println(""+objPackage+"."+type);
      return java.lang.Class.forName(objPackage+"."+type);
    } catch (Exception e) {
      JLib.error("Class "+type+": "+e);
      return null;
    }
  }

  public static DBKey fromString(String s) {
    String[] sa=JString.split(s,")");
    String[] sb=JString.split(sa[1],":");
    return new SimpleDBKey(sa[0].substring(1),sb[0],sb[1]);
  }

  public String toString() {
    return type+":"+id;
  }

}
