package jlib;

import java.lang.String;
import java.io.OutputStream;

public class StringSink extends OutputStream {

  public String sink="";

  StringSink() {
  }

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