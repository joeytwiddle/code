package jlib.strings;

import jlib.JString;
import jlib.JLib;
// import javax.servlet.http.HttpSession;
import java.lang.String;
import java.util.jar.*;
import java.net.*;
import java.io.*;

public class FakeOutputStream extends OutputStream {

  public ByteArrayOutputStream store=new ByteArrayOutputStream();

  /*public FakeOutputStream(OutputStream out) {
    super(out);
  }*/

  /*public FakeOutputStream() {
    super(new OutputStream());
  }*/

  public void write(byte[] b) {
		try {
      store.write(b);
		} catch (Exception e) {
			JLib.report(this,e);
		}
  }
  
	public void write(byte[] b,int off,int len) {
		try {
      store.write(b,off,len);
		} catch (Exception e) {
			JLib.report(this,e);
		}
  }
  
	public void write(int b) {
		try {
      store.write((char)b);
		} catch (Exception e) {
			JLib.report(this,e);
		}
  }

}

