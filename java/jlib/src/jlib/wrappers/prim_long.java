package jlib.wrappers;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;

import jlib.db.*;


public class prim_long extends Wrapper {

	long value;

	public prim_long() { }
	
	public prim_long(long v) {
		value=v;
	}

	public String toString() {
		return ""+value;
	}

	public static Object fromString(String s) {
		return new Long(s);
	}

	// public String SQLtype() {
		// return "long";
	// }

}
