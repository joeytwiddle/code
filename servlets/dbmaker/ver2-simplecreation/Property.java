package dbmaker;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.db.*;
import jlib.multiui.*;

class One2One extends Property {

	public One2One(String n,Type t) {
		super(n,t);
	}

}

class One2Many extends Property {

	public One2Many(String n,Type t) {
		super(n,t);
	}

}

class Many2Many extends Property {

	public Many2Many(String n,Type t) {
		super(n,t);
	}

}

public class Property {

	public static final String[] choices={"1 -> 1","1 -> N","N -> N"};

	public Spec parentSpec; // linkback
	
	public String name;
	public Type type;
  public boolean seperateTable=false;
  public boolean autoLinkBack=false; 

	public Property(String n,Type t) {
		name=n;
		type=t;
	}

	public final static Property getNew(Spec s,String n,Type t,int lt) {
		Property p=getNew2(n,t,lt);
		p.parentSpec=s;
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

}
