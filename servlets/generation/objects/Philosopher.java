package generation.objects;

import java.sql.ResultSet;
import jlib.db.*;
import jlib.*;

public class Philosopher implements DBObj {
  public java.lang.String id;
  public int visits;
  public double rating;
  public java.sql.Date creationdate;
  public java.lang.String authorid;
  public java.lang.String dummy;
  public java.lang.String name;
  public java.sql.Date birthdate;
  public java.sql.Date deathdate;

  public Philosopher() { }

  Philosopher(ResultSet res) {
    try {
      id = (java.lang.String) res.getObject("id");
      visits = res.getInt("visits");
      rating = res.getDouble("rating");
      creationdate = (java.sql.Date) res.getObject("creationdate");
      authorid = (java.lang.String) res.getObject("authorid");
      dummy = (java.lang.String) res.getObject("dummy");
      name = (java.lang.String) res.getObject("name");
      birthdate = (java.sql.Date) res.getObject("birthdate");
      deathdate = (java.sql.Date) res.getObject("deathdate");
    } catch (Exception e) {
      JLib.report("Problem creating "+this+": "+e);
    }
  }

  public DBKey getKey() {
    return new SimpleDBKey("generation","Philosopher",id);
  }

}
