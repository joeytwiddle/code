package jlib.db;

import java.lang.*;
import java.sql.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;

public class DB {

  java.util.Map statements=new java.util.HashMap();

  public DB() {
  }

  public ResultSet getResultSet(DBLocation loc,String query) {
    Statement s=getStatement(loc);
		JLib.report("Got "+s+" from "+loc);
    try {
      ResultSet rs=s.executeQuery(query);
      return rs;
    } catch (Exception e) {
      Log.error(this,"DB.getResultSet("+query+"): "+e);
      return null;
    }
  }
  
  public ResultSet getResultSet(DBKey key) {
    String query="SELECT * FROM "+key.table()+" WHERE id='"+key.id()+"';";
    return getResultSet(key.location(),query);
  }
  
  public DBObj get(DBKey key) {
    ResultSet rs=getResultSet(key);
		System.out.println("Got resultset "+rs);
    return objectFromResultSet(key,rs);
  }
  
  public boolean writeBack(DBObj obj) {
		try {
			JLib.report("writeBack started");
		  DBKey key=obj.getKey();  // exclude id
		  List fields=getFields(obj,true);
		  StringBuffer sb=new StringBuffer();
		  sb.append("UPDATE "+key.table()+" SET ");
		  for (int i=0;i<fields.size();i++) {
				String[] ss=(String[])fields.get(i);
			  sb.append(ss[0]+" = '"+ss[1]+"' "+( i<fields.size()-1 ? ", " : "" ));
			}
		  sb.append("WHERE id = '"+key.id()+"';");
		  String command=sb.toString();
			JLib.report("0");
			JLib.report(command);
			JLib.report("1 "+key.location());
		  Statement s=getStatement(key.location());
			JLib.report("A");
		  int i=s.executeUpdate(command);
			JLib.report("B");
      return true;
		} catch (Exception e) {
			JLib.report(e);
		}
		return false;
  }
	
  public boolean writeNew(DBObj obj) {
		try {
		  DBKey key=obj.getKey();  // include id
		  List fields=getFields(obj,false);
		  StringBuffer sb=new StringBuffer();
		  sb.append("INSERT INTO "+key.table()+" ( ");
		  final String[] ss={"","'"};
		  for (int j=0;j<=1;j++) {
			  for (int i=0;i<fields.size();i++) {
					String[] fs=(String[])fields.get(i);
				  sb.append(ss[j]+fs[j]+ss[j]+( i<fields.size()-1 ? ", " : "" ));
				}
			  if (j==0)
				  sb.append(") VALUES ( ");
		  }
		  sb.append(");");
		  String command=sb.toString();
		  Statement s=getStatement(key.location());
		  int i=s.executeUpdate(command);
      return true;
		} catch (Exception e) {
			JLib.report(e);
		}
		return false;
  }

  Statement getStatement(DBLocation l) {
    try {
      // Check if already have working connection
      Object o=statements.get(l);
      if (o!=null) {
				Log.report("Already got statement");
        Statement s=(Statement)o;
        // if (!s.getConnection().isClosed())
				// This causes Internal Server Error - presumably not supported.
          return s;
      }
      
			Log.report("Need new statement");
      // If not, create new connection
      Class c=Class.forName(l.driver);
      Driver d=(Driver)c.newInstance();
      DriverManager.registerDriver(d);
      Connection connection=DriverManager.getConnection(l.url,l.user,l.pass);
      Statement s=connection.createStatement();
      statements.put(l,s);
      return s;
    } catch (Exception e) {
      Log.error("DB.getStatement: "+e);
      return null;
    }
  }

	// Stupid!  You are getting strings out of DB and converting
	// to types!
	// Support SQLtype->Javatype without String middleman please!
	// Worse than this, after reading into Java object we
	// invariably render into String for user to interact with!

  DBObj objectFromResultSet(DBKey key,ResultSet rs) {
    // System.out.println(""+key.objPackage);
    Class c=key.type();
    // System.out.print("Reading into "+c);
    try {
      Object o=c.newInstance();
      // System.out.println("Got "+o);
//      rs.first(); this little bastard causes Internal Server Error in servlets!
			rs.next();
      Field[] fs=o.getClass().getFields();
      for (int i=0;i<fs.length;i++) {
        Field f=fs[i];
        String name=f.getName();
        try {
          String val=rs.getString(name);
          String type=f.getType().getName();
					System.out.println(name+" >"+val+"< -> "+type);
          Object niceval=JTypes.fromString(val,type);
          f.set(o,niceval);
        } catch (Exception e) {
          JLib.report(e);
        }
      }
      return (DBObj)o;
    } catch (Exception e) {
      JLib.report(e);
    }
    return null;
  }

	List getFields(DBObj obj,boolean skipid) {
		try {
		  List l=new Vector();
		  Field[] fs=obj.getClass().getFields();
		  for (int i=0;i<fs.length;i++) {
			  Field f=fs[i];
			  String name=f.getName();
			  if (!(skipid && name.equals("id")))
			    try {
				    String val=""+f.get(obj); // this should be JTypes.toString I to correspond with above
				    String[] res={f.getName(),val};
				    l.add(res);
			    } catch (Exception e) {
				    JLib.report(e);
			    }
		  }
		  return l;
		} catch (Exception e) {
			JLib.report(e);
		}
		return null;
	}

}

