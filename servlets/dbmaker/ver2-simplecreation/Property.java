package dbmaker;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.db.spec.*;
import jlib.multiui.*;

class One2One extends Property {

	public One2One(String n,Type t) {
		super(n,t);
	}

	public void addToDBTable(DBSpec db,DBTable tab) {
		if (type instanceof BaseType) {
			tab.add(name,type.getSQLType());
		} else {
			tab.add(name+"_"+type.getName()+"_id",SQLIDtype);
			DBTable reltab=db.getTable(parentObj.name+"_"+name+"_"+type.getName());
			reltab.add(parentObj.name+"_id",SQLIDtype);
			reltab.add(parentObj.name+"_"+type.getName()+"_"+name+"_id",SQLIDtype);
		}
	}

}

class One2Many extends Property {

	public One2Many(String n,Type t) {
		super(n,t);
	}

	public void addToDBTable(DBSpec db,DBTable tab) {
			DBTable reltab=db.getTable(parentObj.name+"_"+name+"_"+type.getName());
			reltab.add(parentObj.name+"_id",SQLIDtype);
			reltab.add(type.getName(),type.getSQLType());
	}

}

class Many2Many extends Property {

	public Many2Many(String n,Type t) {
		super(n,t);
	}

	public void addToDBTable(DBSpec db,DBTable tab) {
		if (type instanceof BaseType) {
			Log.error("Should not have BaseType "+type+" as n->n property "+this.name+" in "+this.parentObj.getName());
		} else {
			tab.add(name+"_"+type.getName()+"_id",SQLIDtype);
			DBTable reltab=db.getTable(parentObj.name+"_"+name+"_"+type.getName());
			reltab.add(parentObj.name+"_id",SQLIDtype);
			reltab.add(type.getName()+"_id",SQLIDtype);
		}
	}

}

public class Property implements Serializable {

	public static final String[] choices={"1 -> 1","1 -> N","N -> N"};
	public static final String SQLIDtype="text";

	public Spec parentSpec; // linkback
	public Obj parentObj; // linkback
	
	public String name;
	public Type type;
  public boolean separateTable=false;
  public boolean autoLinkBack=false; 
  public boolean separateID=false; 

	public final static String explainLinkBack="The Java object(s) contains a link back to the parent";
	public final boolean canLinkBack() {
		// if (type instanceof BaseType)
			return false;
		// ...
		// return true;
	}

	public final static String explainSeparate="Store this field in a separate table";
	public final boolean canBeSeparate() {
		return false; // implied for Objs
	}

	public final static String explainSeparateID="Keep the ID relationship in a separate table";
	public final boolean canHaveSeparateID() {
		// if (type instanceof BaseType)
			return false;
		// else
			// return true;
	}

	public Property(String n,Type t) {
		name=n;
		type=t;
	}

	public final static Property getNew(Spec s,Obj o,String n,Type t,int lt) {
		Property p=getNew2(n,t,lt);
		p.parentSpec=s;
		p.parentObj=o;
		return p;
	}

	public final static Property getNew2(String n,Type t,int lt) {
		if (lt==0)
			return new One2One(n,t);
		if (lt==1)
			return new One2Many(n,t);
		if (lt==2)
			return new Many2Many(n,t);
		return null;
	}

	public String typeString() {
		if (this instanceof One2One)
			return "1 -> 1";
		if (this instanceof One2Many)
			return "1 -> N";
		if (this instanceof Many2Many)
			return "N -> N";
		return null;
	}

	public void addToDBTable(DBSpec db,DBTable tab) {
	}

	public void addToJavaObj(PrintWriter p) {
		p.println("    "+type.getJavaType()+" "+name+";");
	}
	
}
