package jlib.wrappers;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.db.*;
import jlib.multiui.*;

public class prim_int extends Wrapper {

	int value;

	public prim_int() { }
	
	public prim_int(int v) {
		value=v;
	}

	public String toString() {
		return ""+value;
	}

	public static Object fromString(String s) {
		return new Integer(s);
	}

	// public String SQLtype() {
		// return "int";
	// }

}
