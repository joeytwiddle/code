package jlib;

import java.lang.*;
import java.io.*;

public class SymbolicLink extends java.io.File {

	String destination;

	public SymbolicLink(String s) {
		super(s);
		init();
	}

	private void init() {
		String r=JLib.execute(JLib.JPATH+"/startj justlinks "+this);
		if (r==null)
			Log.error(this+" is not a symbolic link");
		destination=r;
	}

}
