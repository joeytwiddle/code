package jlib.db.spec;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.net.*;
import java.util.*;
import jlib.*;
//import nuju.*;
import jlib.multiui.*;
import jlib.db.*;

public class DBSpec {

	List objs;

	public DBSpec(List os) {
		objs=os;
	}
	
	public Obj[] getObjects() {
		return (Obj[])objs.toArray(new Obj[0]);
	}

	public String getJavaImport() {
		return "import java.lang.*;\nimport java.util.*;\nimport jlib.db.*;\n";
	}

	public Obj getObj(String javaType) {
		for (int i=0;i<objs.size();i++) {
			Obj o=(Obj)objs.get(i);
			if (o.getJavaType().equals(javaType))
				return o;
		}
		// return new Obj("java.lang.String");
		return new Obj(javaType);
		// Log.report("Could not find object "+javaType);
		// return null;
	}

}
