package jlib.wrappers;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.db.*;
import jlib.multiui.*;

public class prim_boolean extends Wrapper {

	boolean value;

	public prim_boolean() { }
	
	public prim_boolean(boolean v) {
		value=v;
	}

	public String toString() {
		return ""+value;
	}

	public static Object fromString(String s) {
		return new Boolean(s);
	}

	// public String SQLtype() {
		// return "boolean";
	// }

}
