package jlib;

import java.io.*;

public class HTMLPrintWriter extends PrintWriter {

	public HTMLPrintWriter(Writer out) {
		super(out);
	}

	public HTMLPrintWriter(OutputStream out) {
		super(out);
	}

	public void println(String s) {
		super.println(s+"<br>");
	}

}
