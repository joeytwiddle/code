package jlib;

import java.lang.String;
import jlib.CommandLine;

public class UI {

  public static boolean askYesNo(String question) {
    while (true) {
      String answer=CommandLine.getUserInput(question+" (yes/no)");
      if ( answer.equalsIgnoreCase("y") || answer.equalsIgnoreCase("yes") )
        return true;
      if ( answer.equalsIgnoreCase("n") || answer.equalsIgnoreCase("no") )
        return false;
    }
  }

}