package appserver;

import java.lang.String;
import javax.servlet.*;
import javax.servlet.http.*;
import java.applet.*;
import java.io.*;
import java.util.*;
import java.text.SimpleDateFormat;
import java.lang.reflect.*;

import jlib.*;
import jlib.db.*;
import jlib.multiui.*;

/** You can get the state of an Object.
		The state is represented by lots of little elements.
		There is a way to integrate the elements into an HTML form so that their state may be altered before...
		The Object may be regenerated from the state.
**/

/** Possible state elements:
			- All: have a path represented by fields and other indexes
			- Trivial: type known and how to gen from String
				done by JTypes
			- Non-trivial: Type and Init known
				either a class initialised with ()
				or an array
**/

class StateData {
	Object src;
	StateElem elem;
	StateData(Object a,StateElem b) {
		src=a; elem=b;
	}
	public String toString() {
		return elem.path+"/ "+elem;
	}
}

class StateElem {
	public String path;
	public Class type;
}

class TrivialStateElem extends StateElem {
	public String string;
	TrivialStateElem(String p,Object o) {
		path=p; type=o.getClass(); string=o.toString();
	}
	public String toString() {
		return "trivial "+type+"=\""+string+"\"";
	}
}

class ClassStateElem extends StateElem {
	// init with Constructor() unless its an array.
	ClassStateElem(String p,Object o) {
		path=p; type=o.getClass();
	}
	public String toString() {
		return "class "+type.getName();
	}
}

class ArrayStateElem extends StateElem {
	int dimensions;
	ArrayStateElem(String p,Object o) {
		path=p; type=o.getClass();
		dimensions=Array.getLength(o);
	}
	public String toString() {
		return "array of ("+type+") size "+dimensions;
	}
}

public class State {

	public List data=new Vector();

	public State(Object p) {
		add("page",p);
	}

	public void add(String path,Object o) { // Check for looping!
		System.out.println(path+"/ "+o);
		Class c=o.getClass();
		if (JArray.contains(JTypes.supported,c.getName())) {
			data.add(new StateData(o,new TrivialStateElem(path,o)));
		} else if (c.isArray()) {
			data.add(new StateData(o,new ArrayStateElem(path,o)));
			for (int i=0;i<Array.getLength(o);i++)
				add(path+"["+i+"]",Array.get(o,i));
		} else {
			data.add(new StateData(o,new ClassStateElem(path,o)));
			Field[] fs=o.getClass().getFields();
			for (int i=0;i<fs.length;i++)
				if ( !Modifier.isStatic(fs[i].getModifiers())
					&& !Modifier.isProtected(fs[i].getModifiers())
					&& !Modifier.isFinal(fs[i].getModifiers()) ) {
				  try {
					  add(path+"."+fs[i].getName(),fs[i].get(o));
				  } catch (Exception e) {
					  System.out.println(this+".add(): "+e);
				  }
				} else {
					System.out.println("Rejected modifiers "+Modifier.toString(fs[i].getModifiers()));
				}
		}
		// forall sub-data add(path+"fieldname")
	}

}
