package dbmaker;

// Hello

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.db.spec.*;
import jlib.multiui.*;

public class Spec implements Serializable {

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
		// for (int i=0;i<all.size();i++) {
			// Log.report("  "+all.get(i).getClass());
		// }
		return (String[])all.toArray(new String[0]);
		// return (String[])all.toArray();
	}

	private String[] getObjNames() {
		String[] ss=new String[objects.size()];
		for (int i=0;i<objects.size();i++)
			ss[i]=((Obj)objects.get(i)).name;
		return ss;
	}

	public String[] getObjNamesAndnothing() {
		String[] ss=new String[objects.size()+1];
		ss[0]="nothing";
		for (int i=0;i<objects.size();i++)
			ss[i+1]=((Obj)objects.get(i)).name;
		return ss;
	}

	public String getPackage() {
		return "dbmaker.objs."+name;
	}

	private String getPackageDir() {
		return "/home/joey/j/code/java/servlets/"+JString.replace(getPackage(),".",File.separator);
	}

	public void generate(Map gendb,Map genobj,Map gensql,Map genlooknfeel) {
		try {
			String root=getPackageDir();
			Log.report( "Trying to mkdir : " + new File(root).mkdirs() );
			DBSpec db=new DBSpec();
		  for (int i=0;i<objects.size();i++) {
			  Obj o=(Obj)objects.get(i);
				if (((CheckBox)gendb.get(o)).state) { // Generate DB
					o.addToDBSpec(db);
				}
				if (((CheckBox)genobj.get(o)).state) { // Generate Java objects
			  	PrintWriter p=new PrintWriter(new FileWriter(root+"/"+o.name+".java"));
			  	p.println("package "+getPackage()+";");
					p.println();
					o.writeJavaObj(p);
			  	p.close();
				}
				if (((CheckBox)gensql.get(o)).state) { // Generate SQL read/write
				}
				if (((CheckBox)genlooknfeel.get(o)).state) { // Generate default pages
				}
		  }
			PrintWriter p=new PrintWriter(new FileWriter(root+"/"+name+"-gen.sql"));
			db.createDB(p);
			p.close();
		} catch (Exception e) {
			Log.error(e);
		}
	}

}
