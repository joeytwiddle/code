package jlib.wrappers;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.db.*;
import jlib.multiui.*;

public class prim_float extends Wrapper {

	float value;

	public prim_float() { }
	
	public prim_float(float v) {
		value=v;
	}

	public String toString() {
		return ""+value;
	}

	public static Object fromString(String s) {
		return new Float(s);
	}

	// public String SQLtype() {
		// return "float";
	// }

}
