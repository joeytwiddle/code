package jlib.db.spec;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.net.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.multiui.*;
import jlib.db.*;

public class Property {

	public final static String SQLIDtype="text";

	Obj parentObj;
	String name;
	Type type;

	public Property(Obj o,Type t,String n) {
		parentObj=o;
		name=n;
		type=t;
	}

	String getName() { return name; }

	Type getType() {
		return type;
	}

	String getJavaType() { return type.getJavaType(); }

	public Obj getParentObj() {
		return parentObj;
	}

	public String toString() {
		return "Property:"+name;
	}
		
}
