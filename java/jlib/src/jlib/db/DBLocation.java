package jlib.db;

import java.lang.String;

public class DBLocation {

  public final String driver;
  public final String url;
  public final String user;
  public final String pass;

  public DBLocation(String d,String u,String us,String p) {
    driver=d;
    url=u;
    user=us;
    pass=p;
  }

	public String toString() {
		return "Location(\""+driver+"\",\""+url+"\",\""+user+"\",\"*******\")";
	}

}
