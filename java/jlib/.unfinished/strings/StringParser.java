package jlib.strings;

import java.lang.String;
import jlib.*;
import jlib.strings.*;

import java.io.*;
import java.lang.*;
import java.lang.reflect.*;
import java.util.*;

public class StringParser {

	BufferedReader reader;

  public StringParser(File file) throws Exception {
		reader = new BufferedReader( new FileReader(file) ) ;
  }

	public boolean advance() {
		return true;
	}

	public boolean seek(String s) throws Exception {
		while (!eof()) {
			String here=peek(s.length());
			if (here.equals(s))
				return true;
			advance();
		}
		return false;
	}

	public String peek(int len) {
		char[] cs=new char[len];
		int got=reader.read(cs,0,len);
		if (got<len)
			return null;
		return new String(cs);
	}

	public boolean eof() {
		return (!reader.ready());
	}

}
