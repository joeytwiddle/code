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

public class SpecGenerator {

	DBSpec spec;

  public static String classPath="/home/joey/j/code/java/servlets/";
	public static String dbmakerPath=classPath+"/dbmaker/";
	public static String dataPath=dbmakerPath+"/data/";
	public static String file2classPath(String file) {
		return JString.squeeze(JString.trim(JString.replace(JString.after(file,classPath),"/","."),"."),".");
	}
	public static File class2file(String className) {
		return new File(JString.replace(className,".","/"));
	}

	public SpecGenerator(DBSpec s) {
		spec=s;
	}

	public String extendsWhat(Obj o) {
		String s="";
		for (int i=0;i<o.extendsObjs.size();i++) {
			Type t=(Type)o.extendsObjs.get(i);
			System.out.println(". "+t);
			s=s+t.getJavaType();
			if (t instanceof Obj)
				s+="Interface";
			if (i<o.extendsObjs.size()-1)
				s+=", ";
		}
		return s;
	}

	public void generateJava() {

		Obj[] objs=spec.getObjects();
		for (int i=0;i<objs.length;i++) {

			Obj o=objs[i];
			File intf=new File(classPath+class2file(o.getJavaType())+"Interface.java");
			File basef=new File(classPath+class2file(o.getPackageName()+".Base"+o.getClassName())+".java");
			File develf=new File(classPath+class2file(o.getJavaType())+".java");

				StringBuffer inte=new StringBuffer();
				StringBuffer base=new StringBuffer();
				StringBuffer devel=new StringBuffer();

				inte.append("package "+o.getPackageName()+";\n\n");
				base.append("package "+o.getPackageName()+";\n\n");
				devel.append("package "+o.getPackageName()+";\n\n");

				inte.append(spec.getJavaImport());
				base.append(spec.getJavaImport());
				devel.append(spec.getJavaImport());

				inte.append("\n");
				base.append("\n");
				devel.append("\n");

				inte.append("public interface "+o.getClassName()+"Interface {\n\n");
				String s=extendsWhat(o);
				s+=(s.length()>0 ? ", " : " " )+o.getClassName()+"Interface";
				if (s.length()>0)
					s=" implements "+s;
				base.append("public abstract class Base"+o.getClassName()+" extends jlib.db.ADBObj"+s+" {\n\n");
				devel.append("public class "+o.getClassName()+" extends Base"+o.getClassName()+" {\n\n");

				// Stuff related to properties, for which certain methods are created.
				
				// Interface
				{
					Property[] ps=o.getProperties();
					for (int j=0;j<ps.length;j++) {
						Property p=ps[j];
						inte.append("  public "+p.getJavaType()+" get"+JString.initCap(p.getName())+"();\n");
						inte.append("  public void set"+JString.initCap(p.getName())+"("+p.getJavaType()+" newValue);\n");
					}
				}

				base.append("  // Base class fields\n");
				{
					Property[] ps=o.getAllProperties();
					for (int j=0;j<ps.length;j++) {
						Property p=ps[j];
						base.append("  private "+p.getJavaType()+" "+p.getName()+"=null;\n");
					}
				}
				base.append("\n");

				base.append("  // Base class properties\n");
				{
					Property[] ps=o.getAllProperties();
					for (int j=0;j<ps.length;j++) {
						Property p=ps[j];
						base.append("  public "+p.getJavaType()+" get"+JString.initCap(p.getName())+"() {\n    return "+p.getName()+";\n  }\n");
						base.append("  public void set"+JString.initCap(p.getName())+"("+p.getJavaType()+" newValue) {\n    "+p.getName()+"=newValue;\n  }\n");
					}
				}

				base.append("  // Friends\n");
				{
					Property[] ps=o.getFriends(spec);
					Log.report("Got "+ps.length+" friends for "+o+": "+JList.toList(ps));
					for (int j=0;j<ps.length;j++) {
						Property p=ps[j];
						base.append("  public "+p.getParentObj().getJavaType()+"[] get"+JString.initCap(p.getName())+p.getParentObj().getClassName()+"() {\n    return null; /* ... */\n  }\n");
					}
				}
				
				inte.append("\n}\n");
				base.append("\n}\n");
				devel.append("\n}\n");

				echo("Writing "+o+" to "+intf+","+basef+".");
				intf.getParentFile().mkdirs();
				Files.writeStringtoFile(inte.toString(),intf);
				basef.getParentFile().mkdirs();
				Files.writeStringtoFile(base.toString(),basef);

			// if (develf.exists()) {
				// echo(develf+" exists: not writing.");
			// } else {
				echo("Writing "+o+" to "+develf+".");
				develf.getParentFile().mkdirs();
				Files.writeStringtoFile(devel.toString(),develf);
			// }

		}

	}

  void echo(String s) {
		System.out.println(s);
	}

}
