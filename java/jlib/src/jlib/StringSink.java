package jlib;

import java.lang.String;
import java.io.OutputStream;

/** @deprecated until fixed.  Use ByteArrayOutputStream instead. **/

public class StringSink extends OutputStream {

  public String sink="";

  StringSink() {
  }

  // I expected String + byte[] is gonna fail this way! :P

  // The proper solution is to use ByteArrayOutputStream and toString() at the end.
  
  public void write(byte[] bs) {
    sink+=bs;
  }

  public void write(byte[] bs,int off,int len) {
    sink+=bs;
  }

  public void write(int i) {
    sink+=i;
  }

}