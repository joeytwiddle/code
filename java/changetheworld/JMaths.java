package changetheworld; import jlib.JString;

public class JMaths {
  public static int min(int a,int b) {
    return ( a<b ? a : b);
  }
  public static int minnominus(int a,int b) { // will return b if both < 0
    if (a<0 && b<0)
      return -1;
    if (a<0)
      return b;
    if (b<0)
      return a;
    return min(a,b);
  }
}