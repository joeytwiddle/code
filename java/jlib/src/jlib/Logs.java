package jlib;

import java.lang.String;
import jlib.*;

public class Logs implements Logger {

  public void report(String s) {
    Log.report(s);
  }

  public void error(String s) {
    Log.error(s);
  }

}