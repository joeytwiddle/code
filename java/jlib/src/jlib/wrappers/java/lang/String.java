package jlib.wrappers.java.lang.String;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.db.*;
import jlib.multiui.*;

public class String {

	private String obj;

	public String(String s) {
		obj=s;
	}
	
	public Object getObject() {
		return obj;
	}

	public String toString() {
		return obj;
	}

	public static Object fromString(String s) {
		return new String(s);
	}

}
