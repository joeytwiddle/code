package jlib.db;

import java.lang.String;

public interface DBKey {

  public DBLocation location();

  public String table();

  public String id();

  public java.lang.Class type();

}
