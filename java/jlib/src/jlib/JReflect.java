package jlib;

import jlib.*;
import java.lang.String;
import java.lang.reflect.*;

public class JReflect {

	static String replace(String r,String w, String s) {
		int i=s.indexOf(r);
		if (i>=0)
			s=s.substring(0,i)+w+s.substring(i+r.length());
		return s;
	}

	public static void main(String[] argv) {

		if (argv.length == 0) {
			System.out.println("You should provide one or more absolute class names to be reflected.");
			System.exit(1);
		}

		for (int j=0;j<argv.length;j++) {
			String cls = argv[j];

			try {

				Class c=Class.forName(cls);
				Field[] fs=c.getFields();
				Constructor[] cs=c.getConstructors();
				Method[] ms=c.getDeclaredMethods();
				//      System.out.println(c+"\n"+fs+"\n"+ms);
				String className = c.toString(); className = className.substring(className.indexOf(" ")+1);
				System.out.println("> "+className);
				System.out.print(""+c+" extends "+c.getSuperclass().getName());
				if (c.getInterfaces().length>0) {
					System.out.print(" implements "+JVector.toString(c.getInterfaces()));
				}
				System.out.println(" {");
				System.out.println("  Fields:");
				for (int i=0;i<fs.length;i++) {
					System.out.println("    "+fs[i]);
					// System.out.println("        ( "+fs[i].getType().getName()+" )");
				}
				System.out.println("  Constructors:");
				for (int i=0;i<cs.length;i++)
					System.out.println("    "+cs[i]);
				System.out.println("  Methods:");
				for (int i=0;i<ms.length;i++)
					System.out.println("    "+replace(className+".","",ms[i].toString()));
				System.out.println(" }");
				try {
					System.out.println("Trying to create instance...");
					Object o=c.newInstance();
					System.out.println("Got "+o);
				} catch (java.lang.NoClassDefFoundError e) {
					System.out.println("Could not create an instance :-(");
				}

			} catch (Throwable e) {
				System.err.println(""+e);
			}

		}

	}

	public static Class classcalled(String type) {
		try {
			return Class.forName(type);
		} catch (Exception e) {
			return null;
		}
	}

	public static String classof(Object o) {
		return stripclasspath(o.getClass().getName());
	}

	public static String stripclasspath(String s) {
		int i=s.lastIndexOf(".");
		if (i>-1)
			s=s.substring(i+1);
		return s;
	}

	public static boolean isatypeof(Object o,String type) {
		return canbe(o,classcalled(type));
	}

	public static boolean canbe(Object o,String type) {
		return canbe(o,classcalled(type));
	}

	public static boolean canbe(Object o,Class c) {
		try {
			System.out.println("dc="+c+" oc="+o.getClass());
			if (c.getName().equals("int"))
				c=classcalled("java.lang.Integer");
			return c.isAssignableFrom(o.getClass());
		} catch (Exception e) {
			return false;
		}
	}

}
