package jlib.strings;

import jlib.JString;

// import javax.servlet.http.HttpSession;

import java.lang.String;
import java.util.jar.*;
import java.net.*;
import java.io.*;

public class FakeWriter extends Writer {

  public StringBuffer store=new StringBuffer();

	public FakeWriter(Writer out) {
		super(out);
	}

	public FakeWriter() {
		super();
	}

	public void close() {
	}

	public void flush() {
	}

	public void write(char[] s) {
    store.append(s);
  }

  public void write(char[] s,int odd,int len) {
    store.append(s,odd,len);
  }

  public void write(int c) {
    store.append((char)c);
  }

  public void write(String s) {
    store.append(s);
  }

  public void write(String s,int off,int len) {
    store.append(s.toCharArray(),off,len);
  }

}

