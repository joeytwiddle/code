package jlib.db.spec;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.db.spec.*;
import jlib.multiui.*;

public class Obj extends Type {

	public String name;
	public List extendsObjs=new Vector();
	public List properties;

	public Obj(String n) {
		name=n;
		properties=new Vector();
	}

	public Property[] getFriends(DBSpec spec) {
		List fs=new Vector();
		Obj[] os=spec.getObjects();
		for (int i=0;i<os.length;i++) {
			Obj o=os[i];
			Property[] ps=o.getAllProperties();
			for (int j=0;j<ps.length;j++) {
				Property p=ps[j];
				if (this.isATypeOf(p.getType())) {
					fs.add(p);
				}
			}
		}
		return (Property[])(fs.toArray(new Property[0]));
	}

	public Property[] getProperties() {
		return (Property[])(properties.toArray(new Property[0]));
	}

	public Obj[] getExtensions() {
		return (Obj[])(extendsObjs.toArray(new Obj[0]));
	}

	public Property[] getAllProperties() {
		List ps=new Vector();
		ps.addAll(properties);
		// Obj[] exts=getAllSuperObjs();
		Obj[] exts=getExtensions();
		Log.report(this+" has extensions "+JList.toList(exts));
		for (int i=0;i<exts.length;i++) {
			Obj o=exts[i];
			System.out.println(""+(i+1)+"/"+exts.length+": "+o);
			ps.addAll(JList.toList(o.getAllProperties()));
			Log.report(this+" has properties "+JList.toList(o.getAllProperties())+" from "+o);
		}
		return (Property[])(ps.toArray(new Property[0]));
	}

	public Obj[] getAllSuperObjs() {
		List sups=new Vector();
		Obj[] exts=getExtensions();
		for (int i=0;i<exts.length;i++) {
			Obj o=exts[i];
			sups.add(o);
			sups.addAll(JList.toList(o.getAllSuperObjs()));
		}
		// Log.report("sups:");
		// for (int i=0;i<sups.size();i++)
			// Log.report("  "+sups.get(i));
		return (Obj[])sups.toArray(new Obj[0]);
	}

	public String getClassName() {
		return JString.afterlast(name,".");
	}

	public String getPackageName() {
		return JString.beforelast(name,".");
	}

	public void setPackage(String p) {
	  name=p+"."+JString.afterlast(name,".");
	}

	public String getName() {
		return name;
	}

	public String getJavaType() {
		// return packageName+"."+name;
		return name;
	}

	public String getSQLType() {
		return Property.SQLIDtype;
	}

	// public void addToDBSpec(DBSpec db) {
		// // DBTable table=new DBTable(name);
		// // db.tables.add(table);
		// DBTable table=db.getTable(name);
		// for (int i=0;i<properties.size();i++) {
			// Property p=(Property)properties.get(i);
			// p.addToDBTable(db,table);
		// }
	// }

	/* public void writeJavaObj(PrintWriter p) {
		p.println("public class "+name+" {");
		for (int i=0;i<properties.size();i++) {
			((Property)properties.get(i)).addToJavaObj(p);
		}
		p.println("}");
	} */

	// this is a type of o if it is an o, or any of its ancestors are an o
	public boolean isATypeOf(Type t) {
		if (t instanceof Obj) {
			Obj o=(Obj)t;
			return (this.equals(o) || JList.toList(getAllSuperObjs()).contains(o));
		}
		return false;
	}
	
	public boolean equals(Object o) {
		if (o instanceof Obj) {
			return ((Obj)o).getName().equals(getName());
		}
		return false;
	}

	public String toString() {
		return "Obj:"+getClassName();
	}
	
}
