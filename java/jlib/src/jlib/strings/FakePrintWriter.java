package jlib.strings;

import jlib.JString;

// import javax.servlet.http.HttpSession;

import java.lang.String;
import java.util.jar.*;
import java.net.*;
import java.io.*;

public class FakePrintWriter extends PrintWriter {

  public StringBuffer store; // =new StringBuffer();

  // ??
	// public FakePrintWriter(Writer out) {
    // super(fake);
		// FakeWriter fake=new FakeWriter(out);
		// store=fake.store;
  // }

  public FakePrintWriter() {
    super(new FakeWriter(new StringWriter()));
  	// FakeWriter fake=;
		// store=fake.store;
  }

/*  public void print(String s) {
    store.append(s);
  }

  public void println(String s) {
    store.append(s);
    store.append("\n");
  } */

}

