package jlib.wrappers;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.db.*;
import jlib.multiui.*;

public class Wrapper implements Serializable {

	// Unfortunately Object.getClass is not static.

	public final String whatFor=getWrappedClassName();

	// To / from SQL and Java

	/** There must be overwritten **/

	public Wrapper() { } // this must be present but can remain empty
	
	public String SQLtype() {
		return getDePrimWrappedClassName(); // OK so it needn't be overwritten for primitives, int etc.
	}

	public Object getObject() {
		return null;
	}

	public String toString() {
		return null;
	}

	public static Object fromString(String s) {
		return null;
	}

	/** End of methods which must be overwritten **/

	public final static Class recommendClassFor(String javaType) {
		try {
			if (javaType.indexOf(".")>=0)
				return Class.forName("jlib.wrappers."+javaType+"Wrapper");
			else
				return Class.forName("jlib.wrappers.prim_"+javaType);
		} catch (Exception e) {
			Log.error(""+e);
		  return null;
		}
	}
	
	public final static Object wrapperFromString(String s) {
		return getWrapper(fromString(s));
	}

	public final static Wrapper getWrapper(Object o) {
		String wrapperName="jlib.wrappers."+o;
		try {
			Class c=Class.forName(wrapperName);
			Class[] cs={ o.getClass() };
			Constructor cn=c.getConstructor(cs);
			Object[] args={ o };
			Object w=cn.newInstance(args);
			return (Wrapper)w;
		} catch (Exception e) {
			JLib.report("No wrapper for "+o.getClass().getName(),e);
			return null;
		}
	}

	public final String javaType() {
		return getWrappedClassName();
	}

	public final Class getWrappedClass() {
		try {
			return Class.forName(getWrappedClassName());
		} catch (Exception e) {
			JLib.error(e);
			return null;
		}
	}

	public final String getWrappedClassName() {
		return getDePrimWrappedClassName();
	}
	
	private final String getNormWrappedClassName() {
		String s=JString.after(getClass().getName(),"jlib.wrappers.");
		if (s.endsWith("Wrapper"))
			s=JString.beforelast(s,"Wrapper");
		return s;
	}
	
	private final String getDePrimWrappedClassName() {
		String n=getNormWrappedClassName();
		if (n.startsWith("prim_"))
			return JString.after(n,"prim_");
		else
			return n;
	}

  // public static void main(String[] args) {
		// ArgParser a=new ArgParser(args);
		// a.done();
	// }

}
