package dbmaker;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.db.spec.*;
import jlib.multiui.*;

public class Obj extends Type {

	public Spec parentSpec;

	public Vector extendsObjs=new Vector();
	public String name;
	public List properties;

	public Obj(Spec p,String n) {
		parentSpec=p;
		name=n;
		properties=new Vector();
	}

	public String getName() {
		return name; // parentSpec.getPackage()+"."+name;
	}

	public void addToDBSpec(DBSpec db) {
		// DBTable table=new DBTable(name);
		// db.tables.add(table);
		DBTable table=db.getTable(name);
		for (int i=0;i<properties.size();i++) {
			Property p=(Property)properties.get(i);
			p.addToDBTable(db,table);
		}
	}

	public void writeJavaObj(PrintWriter p) {
		p.println("public class "+name+" {");
		for (int i=0;i<properties.size();i++) {
			((Property)properties.get(i)).addToJavaObj(p);
		}
		p.println("}");
	}

	public String getJavaType() {
		return name;
	}

	public String getSQLType() {
		return Property.SQLIDtype;
	}

}
