package dbmaker;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.db.*;
import jlib.multiui.*;

public class Spec {

	public String name;
	public List objects,tables,links;

	// private List baseTypes;

	public Spec(String n) {
		name=n;
		tables=new Vector();
		links=new Vector();
		objects=new Vector();
		// baseTypes=new Vector();
		// for (int i=0;i<BaseType.types.length;i++)
			// baseTypes.add(new BaseType(BaseType.types[i]));
	}

	public Type getType(int i) {
		if (i<objects.size())
			return (Obj)objects.get(i);
		else
			return new BaseType(BaseType.types[i-objects.size()]);
	}

	// public Obj getObj(String s) {
		// return getObj(JVector.toList(getObjNames()).indexOf(s));
	// }

	public String[] getTypeNames() {
		List all=new Vector();
		all.addAll(JList.toList(getObjNames()));
		all.addAll(JList.toList(BaseType.types));
		Log.report(""+all);
		for (int i=0;i<all.size();i++) {
			Log.report("  "+all.get(i).getClass());
		}
		return (String[])all.toArray(new String[0]);
		// return (String[])all.toArray();
	}

	private String[] getObjNames() {
		String[] ss=new String[objects.size()];
		for (int i=0;i<objects.size();i++)
			ss[i]=((Obj)objects.get(i)).name;
		return ss;
	}

	public String getPackage() {
		return "dbmaker.objs."+name;
	}

	private String getPackageDir() {
		return "/home/joey/j/code/java/servlets/"+JString.replace(getPackage(),".",File.separator);
	}

	public void generate() {
		try {
			String root=getPackageDir();
			Log.report( "Trying to mkdir : " + new File(root).mkdirs() );
		  for (int i=0;i<objects.size();i++) {
			  Obj o=(Obj)objects.get(i);
			  PrintWriter p=new PrintWriter(new FileWriter(root+"/"+o.name+".java"));
			  p.println("package "+getPackage()+";");
			  p.close();
		  }
		} catch (Exception e) {
			Log.error(e);
		}
	}

}
