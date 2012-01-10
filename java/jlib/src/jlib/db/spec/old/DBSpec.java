package jlib.db.spec.old;

import java.lang.*;
import java.sql.*;
import java.io.*;
import java.util.*;
import jlib.*;

public class DBSpec {

	public List tables;

	public DBSpec() {
		tables=new Vector();
	}

	public DBTable getTable(String name) {
		for (int i=0;i<tables.size();i++)
			if (((DBTable)tables.get(i)).name.equals(name))
				return (DBTable)tables.get(i);
		DBTable tab=new DBTable(name);
		tables.add(tab);
		return tab;
	}

	public void createDB(PrintWriter p) {
		for (int i=0;i<tables.size();i++)
			((DBTable)tables.get(i)).createTable(p);
	}

}
