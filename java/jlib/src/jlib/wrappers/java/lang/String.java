package jlib.wrappers.java.lang;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;
import jlib.wrappers.*;
import nuju.*;
import jlib.db.*;
import jlib.multiui.*;

public class String extends Wrapper {

	private java.lang.String obj;

	/****/

	public String() { }

	public java.lang.String SQLtype() {
		return "text";
	}

	public String(java.lang.String s) {
		obj=s;
	}
	
	public Object getObject() {
		return obj;
	}

	public java.lang.String toString() {
		return obj;
	}

	public static Object fromString(java.lang.String s) {
		return new java.lang.String(s);
	}

	/****/

}
