package changetheworld; import jlib.JString;

import java.lang.String;
import java.util.Vector;

public class ArgParser {
  private Vector args;
  ArgParser(String s) {
    args=JString.splitv(s," ");
  }
  public String get() {
    return get(0);
  }
  public String get(int i) {
    String s=(String)args.get(i);
    args.remove(i);
    return s;
  }
  public boolean contains(String s) {
    int i=find(s);
    if (i>-1) {
      args.remove(i);
      return true;
    }
    return false;
  }
  public int find(String s) {
    for (int i=0;i<args.size();i++)
      if (s.equals((String)args.get(i)))
        return i;
    return -1;
  }
  public String Stringafter(String s,String or) {
    int i=find(s);
    if (i>-1) {
      args.remove(i);
      return get(i);
    }
    return or;
  }
  public int intafter(String s,int or) {
    String x=Stringafter(s,""+or);
    try {
      int i=(new Integer(x)).intValue();
      return i;
    } catch (Exception e) {
    }
    return or;
  }
}