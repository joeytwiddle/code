package jlib.db.spec.reflect;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.net.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.multiui.*;
import jlib.db.spec.*;

public class DBSpec extends jlib.db.spec.DBSpec {

	public static void main(String[] argv) {
		ArgParser args=new ArgParser(argv);
		String initClass=args.get("init class, eg. \"dbmaker.data.init.Generation\"");
		String destPackage=args.get("destination package, eg. \"dbmaker.data.objs.generation\"");
		args.done();

		try {
			// Go through fields in the provided class, adding each object type.
			Class c=Class.forName(initClass);
			System.out.println(""+c);
			Field[] fs=c.getDeclaredFields();
			System.out.println(""+fs.length);
			List cs=new Vector();
			for (int i=0;i<fs.length;i++) {
				cs.add(fs[i].getType());
			}
      jlib.db.spec.reflect.DBSpec d=new jlib.db.spec.reflect.DBSpec(cs,destPackage);
			new SpecGenerator(d).generateJava();
		} catch (Exception e) {
			Log.error(e);
			System.out.println(""+e);
		}
	}

	public DBSpec(List classes,String destPackage) {
		super(initReflect(classes));
		finishReflect(classes);
		Obj[] objs=getObjects();
		for (int i=0;i<objs.length;i++)
			objs[i].setPackage(destPackage);
	}

	static List initReflect(List classes) {
		List objs=new Vector();
		System.out.println(""+objs);
		for (int i=0;i<classes.size();i++) {
			Class c=(Class)classes.get(i);
			Obj o=new Obj(c.getName(),!c.isInterface());
			// Obj o=new Obj(c.getName(),!Modifier.isInterface(c.getModifiers()));
			
			// Existence
			System.out.println("Putting "+o.getName());
			objs.add(o);
		}
		return objs;
	}

	void finishReflect(List classes) {
		for (int i=0;i<classes.size();i++) {
			Class c=(Class)classes.get(i);
			Obj o=getObj(c.getName());

			// Inheritance
			Class sup=c.getSuperclass();
			Class[] inhs=c.getInterfaces();
			System.out.println("sup="+sup+"\ninhs="+inhs.length);
			if (sup!=null)
				if (!sup.getName().equals("java.lang.Object"))
				  o.extendsObjs.add(BaseType.getType(sup,this));
			for (int j=0;j<inhs.length;j++)
				o.extendsObjs.add(BaseType.getType(inhs[j],this));
				// o.extendsObjs.add(BaseType.getType(inhs[i],this));

			// Properties
			Field[] fs=c.getDeclaredFields();
			for (int j=0;j<fs.length;j++) {
				Field f=fs[j];
				Class fc=f.getType();
				Type ft=BaseType.getType(fc,this);
				Property p=new Property(o,ft,f.getName());
				o.properties.add(p);
			}
		}
	}

}
