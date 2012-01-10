package jlib.db.spec;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;
import jlib.db.*;
import jlib.wrappers.*;

public class BaseType extends Type {

	public static Type getType(Class c,DBSpec spec) {
		Obj o=spec.getObj(c.getName());
		if (o!=null)
			return o;
		BaseType b=new BaseType(c.getName());
		if (b!=null)
		  if (b.wrapper!=null)
			  return b;
		Log.error("BaseType.getType(\""+c.getName()+"\"): got a real Java class.");
		return null;
	}

	public static final String[] types={"int","long","float","double","java.lang.String","java.util.Date","java.sql.Date"};

	Wrapper wrapper;

	public BaseType(String type) {
		try {
			Class c=Wrapper.recommendClassFor(type); // Class.forName("jlib.wrappers."+type);
			System.out.println("BaseType recommended "+c);
			wrapper=(Wrapper)c.newInstance();
		} catch (Exception e) {
			Log.error(e);
			wrapper=null;
		}
	}

	public String getName() {
		return wrapper.javaType();
		// return JString.after(wrapper.toString(),"jlib.wrappers.");
	}

	public String getJavaType() {
		return wrapper.javaType();
	}

	public String getSQLType() {
		return wrapper.SQLtype();
	}

}
