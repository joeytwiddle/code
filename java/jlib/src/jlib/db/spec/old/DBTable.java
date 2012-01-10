package jlib.db.spec.old;

import java.lang.*;
import java.sql.*;
import java.io.*;
import java.util.*;
import jlib.*;

class DBField {

	public String name,type;

	DBField(String n,String t) {
		name=n;
		type=t;
	}

}

public class DBTable {

	public List fields=new Vector();
	public String name;

	public DBTable(String n) {
		name=n;
	}

	public void add(String fieldname,String fieldtype) {
		fields.add(new DBField(fieldname,fieldtype));
	}

	public void createTable(PrintWriter s) {
		s.println("CREATE TABLE "+name+" (");
		for (int i=0;i<fields.size();i++) {
			DBField f=(DBField)fields.get(i);
			s.println("  "+f.name+" "+f.type
				+( i<fields.size()-1 ? "," : "" )+"");
		}
		s.println(");");
	}

}
