package jlib;

public class JCurses {

  public static String off=curse("\033[00;00m");

  public static String white=lightcol(7);
  public static String grey=darkcol(7);

  public static String red=col(1);
  public static String green=darkcol(2);
  public static String yellow=col(3);
  public static String blue=darkcol(4);
  public static String magenta=darkcol(5);
  public static String cyan=darkcol(6);

  public static String darkred=darkcol(1);
  public static String darkgreen=darkcol(2);
  public static String darkyellow=darkcol(3);
  public static String darkblue=darkcol(4);
  public static String darkmagenta=darkcol(5);
  public static String darkcyan=darkcol(6);

  public static String lightred=lightcol(1);
  public static String lightgreen=lightcol(2);
  public static String lightyellow=lightcol(3);
  public static String lightblue=lightcol(4);
  public static String lightmagenta=lightcol(5);
  public static String lightcyan=lightcol(6);

  private static boolean inUse=true;

  public static void noCurses() {
    inUse=false;
  }

  public static String curse(String s) {
    return ( inUse ? s : "" );
  }

  public static String col(int i) {
    return lightcol(i);
  }
  public static String lightcol(int i) {
    return curse("\033[01;3"+i+"m");
  }
  public static String darkcol(int i) {
    return curse("\033[00;3"+i+"m");
  }


}