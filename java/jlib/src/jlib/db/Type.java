package jlib.db;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;
import jlib.*;
import nuju.*;
import jlib.db.*;
import jlib.multiui.*;

public class Type {

	public String name;
	public String className;
	public String tableName;
	public String database;
	public String packageName;

	public List ancestors;
	public List fields;

	public List getDescendents() {
		return null;
	}

	public List getAllFields() {
		return null;
	}

  public static void main(String[] args) {

		ArgParser a=new ArgParser(args);
		a.done();

		System.out.println(">\t<");
	
	}

}
