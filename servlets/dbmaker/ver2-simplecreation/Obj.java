package dbmaker;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.db.*;
import jlib.multiui.*;

public class Obj implements Type {

	public Spec parentSpec;

	public String name;
	public List properties;

	public Obj(Spec p,String n) {
		parentSpec=p;
		name=n;
		properties=new Vector();
	}

	public String getName() {
		return parentSpec.getPackage()+"."+name;
	}

}
