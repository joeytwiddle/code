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
  
	boolean dataChangeAwareness=true;
	boolean botherWithInterfaces=false;

 	String theBaseObject=( dataChangeAwareness ? "jlib.db.DataChangeAwareDBObj" : "jlib.db.ADBObj" );

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
		System.out.println();
		System.out.println("This is what "+o+" extends:");
		String s="";
		for (int i=0;i<o.extendsObjs.size();i++) {
			Type t=(Type)o.extendsObjs.get(i);
			System.out.println(". "+t);
			s=s+t.getJavaType();
			// if (t instanceof Obj)
				// s+="Interface";
			if (i<o.extendsObjs.size()-1)
				s+=", ";
		}
		System.out.println();
		echo(">>>> "+s);
		return s;
	}

	// Java names from Obj
	public String getInterface(Obj o) {
		return o.getJavaType()+"Interface";
	}
	public String getInterfaceOnly(Obj o) {
		return o.getClassName()+"Interface";
	}

	public String getBase(Obj o) {
		return o.getPackageName()+".base.Base"+o.getClassName();
	}

	public String getObj(Obj o) {
		return o.getJavaType();
	}

	public void generateJava() {

		Obj[] objs=spec.getObjects();
		for (int i=0;i<objs.length;i++) {

			Obj o=objs[i];
			File intf=new File(classPath+class2file(getInterface(o))+".java");
			File basef=new File(classPath+class2file(getBase(o))+".java");
			File develf=new File(classPath+class2file(getObj(o))+".java");

				StringBuffer inte=new StringBuffer();
				StringBuffer base=new StringBuffer();
				StringBuffer devel=new StringBuffer();

				inte.append("package "+o.getPackageName()+";\n\n");
				base.append("package "+o.getPackageName()+".base;\n\n");
				devel.append("package "+o.getPackageName()+";\n\n");

				inte.append(spec.getJavaImport());
				base.append(spec.getJavaImport());
				devel.append(spec.getJavaImport());

				inte.append("\n");
				base.append("\n");
				devel.append("\n");

				inte.append("public interface "+getInterfaceOnly(o)+" {\n\n");
				// String s=" extends "+extendsWhat(o);
				String s=extendsWhat(o);
				// s+=(s.length()>0 ? ", " : " " )+o.getClassName()+"Interface";
				if (s.length()>0)
					s=" extends "+s;
				else
					s=" extends "+theBaseObject+" ";
				if (botherWithInterfaces)
					s+=" implements "+getInterface(o);
				base.append("public abstract class Base"+o.getClassName()+" "+s+" {\n\n");
				String abs=( o.implemented ? " " : " abstract " );
				devel.append("public"+abs+"class "+o.getClassName()+" extends "+getBase(o)+" {\n\n");

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

				// Base
				base.append("  // New fields in this class\n");
				{
					// Property[] ps=o.getAllProperties();
					Property[] ps=o.getProperties();
					for (int j=0;j<ps.length;j++) {
						Property p=ps[j];
						base.append("  private "+p.getJavaType()+" "+p.getName()+"=null;\n");
					}
				}
				base.append("\n");

				base.append("  // Basic IO on new fields\n");
				{
					// Property[] ps=o.getAllProperties();
					Property[] ps=o.getProperties();
					for (int j=0;j<ps.length;j++) {
						Property p=ps[j];
						base.append("  public "+p.getJavaType()+" get"+JString.initCap(p.getName())+"() {\n    return "+p.getName()+";\n  }\n");
						base.append("  public void set"+JString.initCap(p.getName())+"("+p.getJavaType()+" newValue) {\n    "+p.getName()+"=newValue;\n");
						if (dataChangeAwareness)
							base.append("    changed();\n");
						base.append("  }\n");
					}
				}

				base.append("  // Friends\n");
				{
					Property[] ps=o.getFriends(spec);
					Log.report("Got "+ps.length+" friends for "+o+": "+JList.toList(ps));
					for (int j=0;j<ps.length;j++) {
						Property p=ps[j];
						// base.append("  public "+p.getParentObj().getJavaType()+"[] get"+JString.initCap(p.getName())+p.getParentObj().getClassName()+"() {\n    return null; /* ... */\n  }\n");
						base.append("  /** "+p.getParentObj().getJavaType()+"'s which refer to this "+p.getName()+". **/\n");
						// base.append("  /** "+p.getParentObj().getJavaType()+"'s for this "+p.getName()+". **/\n");
						base.append("  public "+p.getParentObj().getJavaType()+"[] "+p.getName()+p.getParentObj().getClassName()+"s() {\n    return null; /* ... */\n  }\n");
					}
				}
				
				inte.append("\n}\n");
				base.append("\n}\n");
				devel.append("\n}\n");

				echo("Writing "+o+" to "+intf+","+basef+".");
				if (botherWithInterfaces) {
					intf.getParentFile().mkdirs();
					Files.writeStringtoFile(inte.toString(),intf);
				}
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
