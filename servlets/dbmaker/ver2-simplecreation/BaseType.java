package dbmaker;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.db.*;
import jlib.multiui.*;
import jlib.wrappers.*;

public class BaseType implements Type {

	public static final String[] types={"int","long","float","double","java.lang.String","java.util.Date","java.sql.Date"};

	Wrapper wrapper;

	public BaseType(String type) {
		try {
			Class c=Class.forName("jlib.wrappers."+type);
			wrapper=(Wrapper)c.newInstance();
		} catch (Exception e) {
			Log.error(e);
			wrapper=null;
		}
	}

	public String getName() {
		return JString.after(wrapper.toString(),"jlib.wrappers.");
	}

}
