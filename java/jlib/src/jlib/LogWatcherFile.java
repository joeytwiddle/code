package jlib;

import java.lang.String;

import LogWatcher;

public class LogWatcherFile {

  String fname;

  public LogWatcherFile(String f) {
    fname=f;
  }

  public void report(String s) {

    String com="echo \""+s+"\" >> "+fname;
    try {
      Runtime.getRuntime().exec(com);
    } catch (Exception e) {
    }

  }

}