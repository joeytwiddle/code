package jlib.wrappers.java.util;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;
import jlib.wrappers.*;

import jlib.db.*;


public class DateWrapper extends Wrapper {

	private java.util.Date obj=null;

	/****/

	public DateWrapper() { }

	public DateWrapper(java.util.Date s) {
		obj=s;
	}
	
	public java.lang.String SQLtype() {
		return "date";
	}

	public Object getObject() {
		return obj;
	}

	public java.lang.String toString() {
		return ""+obj;
	}

	public static Object fromString(java.lang.String s) {
		return new java.util.Date(s);
	}

	/****/

}
