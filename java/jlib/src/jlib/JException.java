package jlib;

import java.lang.Exception;

public class JException extends Exception {

  String message;

  public JException() {
    message="";
  }

  public JException(String s) {
    message=s;
  }

  public String toString() {
    return message;
  }

}