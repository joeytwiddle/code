package jlib;

import java.lang.String;

public class JHtml {

  public static String removeTags(String s) {

    boolean done=false;

    while (!done) {

      done=true;
      int i=s.indexOf("<");
      if (i>=0) {
        int j=s.indexOf(">",i+1);
        if (j>=0) {
          done=false;
          s=s.substring(0,i)+s.substring(j+1);
        }
      }

    }

    return s;

  }

}